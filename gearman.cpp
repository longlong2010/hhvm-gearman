/*
   +----------------------------------------------------------------------+
   | HipHop for PHP                                                       |
   +----------------------------------------------------------------------+
   | Copyright (c) 2010-2013 Facebook, Inc. (http://www.facebook.com)     |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
*/

#include "hphp/runtime/base/base-includes.h"
#include "hphp/runtime/vm/native-data.h"
#include "hphp/runtime/vm/runtime.h"
#include "hphp/runtime/base/array-init.h"
#include "hphp/runtime/ext/std/ext_std_function.h"
#include <vector>
#include <memory>
#include <gearman.h>

namespace HPHP {

const StaticString s_GearmanClientData("GearmanClientData");
class GearmanClientData {
	public:
		class Impl {
			public:
			Impl() {
				gearman_client_create(&client);
			}
			~Impl() {
				gearman_client_free(&client);
			}

			gearman_client_st client;
		};
		GearmanClientData() {}
		~GearmanClientData() {}

		typedef std::shared_ptr<Impl> ImplPtr;
		ImplPtr m_impl;
};

const StaticString s_GearmanWorkerData("GearmanWorkerData");
class GearmanWorkerData {
	public:
		class Impl {
			public:
			Impl() {
				gearman_worker_create(&worker);
			}
			~Impl() {
				gearman_worker_free(&worker);
			}

			gearman_worker_st worker;
		};

		struct UserDefinedFunc {
			Variant function;
			Variant context;
		};
		GearmanWorkerData() {}
		~GearmanWorkerData() {}

		typedef std::shared_ptr<Impl> ImplPtr;
		ImplPtr m_impl;
		std::vector<std::shared_ptr<UserDefinedFunc>> m_functions;
		//std::vector<Variant> m_functions;
};

void HHVM_METHOD(GearmanClient, __construct) {
	auto data = Native::data<GearmanClientData>(this_);
	data->m_impl.reset(new GearmanClientData::Impl);
}

bool HHVM_METHOD(GearmanClient, addServer, String& host, int64_t port) {
	auto data = Native::data<GearmanClientData>(this_);
	const char* myHost = host.empty() ? nullptr : host.c_str();
	gearman_return_t ret = gearman_client_add_server(&data->m_impl->client, myHost, port);
	return ret == GEARMAN_SUCCESS;
}


#define GEARMAN_CLIENT_DO(function_name, workload, unique) \
	auto data = Native::data<GearmanClientData>(this_); \
	const char* myFunctionName = function_name.empty() ? nullptr : function_name.c_str(); \
	const char* myWorkload = workload.empty() ? nullptr : workload.c_str(); \
	size_t workload_size = workload.length(); \
	const char* myUnique = workload.empty() ? nullptr : unique.c_str(); \


String HHVM_METHOD(GearmanClient, doNormal, const String& function_name,
									   const String& workload,
									   const String& unique /*= null*/) {

	GEARMAN_CLIENT_DO(function_name, workload, unique);	
	size_t result_size;
	gearman_return_t ret;

	char* result = (char*) gearman_client_do(&data->m_impl->client, 
								myFunctionName, myUnique, 
								myWorkload, workload_size, 
								&result_size, &ret);
	String myResult = String(result, result_size, CopyString);	
	delete result;
	return myResult;
}

String HHVM_METHOD(GearmanClient, doHigh, const String& function_name,
									   const String& workload,
									   const String& unique /*= null*/) {

	GEARMAN_CLIENT_DO(function_name, workload, unique);	
	size_t result_size;
	gearman_return_t ret;
	
	char* result = (char*) gearman_client_do_high(&data->m_impl->client, 
								myFunctionName, myUnique, 
								myWorkload, workload_size, 
								&result_size, &ret);
	String myResult = String(result, result_size, CopyString);	
	delete result;
	return myResult;
}

String HHVM_METHOD(GearmanClient, doBackground, const String& function_name,
									   const String& workload,
									   const String& unique /*= null*/) {

	GEARMAN_CLIENT_DO(function_name, workload, unique);

	gearman_job_handle_t handle;
	gearman_return_t ret = gearman_client_do_background(&data->m_impl->client, 
								myFunctionName, myUnique, 
								myWorkload, workload_size, 
								handle);
	if (ret != GEARMAN_SUCCESS) {
		return String();
	}
	return String(handle);
}

String HHVM_METHOD(GearmanClient, doHighBackground, const String& function_name,
									   const String& workload,
									   const String& unique /*= null*/) {

	GEARMAN_CLIENT_DO(function_name, workload, unique);

	gearman_job_handle_t handle;
	gearman_return_t ret = gearman_client_do_high_background(&data->m_impl->client, 
								myFunctionName, myUnique, 
								myWorkload, workload_size, 
								handle);
	if (ret != GEARMAN_SUCCESS) {
		return String();
	}
	return String(handle);
}

void HHVM_METHOD(GearmanWorker, __construct) {
	auto data = Native::data<GearmanWorkerData>(this_);
	data->m_impl.reset(new GearmanWorkerData::Impl);
}

bool HHVM_METHOD(GearmanWorker, addServer, String& host, int64_t port) {
	auto data = Native::data<GearmanWorkerData>(this_);
	const char* myHost = host.empty() ? nullptr : host.c_str();
	gearman_return_t ret = gearman_worker_add_server(&data->m_impl->worker, myHost, port);
	return ret == GEARMAN_SUCCESS;
}

static void* add_function_callback(gearman_job_st* job, void* context, size_t* result_size, gearman_return_t* ret) {
	GearmanWorkerData::UserDefinedFunc* callback = (GearmanWorkerData::UserDefinedFunc*) context;
	//Array params(PackedArrayInit(1).appendRef(callback->context).toArray());
	Array params = Array::Create();
	f_is_callable(callback->function);
	//vm_call_user_func(callback->function, params);
	*result_size = 0;
	return NULL;
}

bool HHVM_METHOD(GearmanWorker, addFunction, String function_name, const Variant& function, VRefParam context, int64_t timeout) {
	auto data = Native::data<GearmanWorkerData>(this_);
	const char* functionName = function_name.empty() ? nullptr : function_name.c_str();
	if (!f_is_callable(function)) {
		raise_warning("Not a valid callback function %s",
				function.toString().data());
		return false;
	}
	
	auto callback = std::make_shared<GearmanWorkerData::UserDefinedFunc>();
	gearman_return_t ret = gearman_worker_add_function(&data->m_impl->worker, functionName, timeout, add_function_callback, callback.get());
	if (ret == GEARMAN_SUCCESS) {
		callback->function = function;
		data->m_functions.push_back(callback);
		return true;
	}
	return false;
}

bool HHVM_METHOD(GearmanWorker, work) {
	auto data = Native::data<GearmanWorkerData>(this_);
	gearman_return_t ret = gearman_worker_work(&data->m_impl->worker);
	return ret == GEARMAN_SUCCESS;
}

static class GearmanExtension : public Extension {
	public:
		GearmanExtension() : Extension("gearman") {}
		virtual void moduleInit() {
			HHVM_ME(GearmanClient, __construct);
			HHVM_ME(GearmanClient, doNormal);
			HHVM_ME(GearmanClient, doHigh);
			HHVM_ME(GearmanClient, doBackground);
			HHVM_ME(GearmanClient, doHighBackground);
			HHVM_ME(GearmanClient, addServer);

			HHVM_ME(GearmanWorker, __construct);
			HHVM_ME(GearmanWorker, work);
			HHVM_ME(GearmanWorker, addServer);
			HHVM_ME(GearmanWorker, addFunction);

			Native::registerNativeDataInfo<GearmanClientData>(s_GearmanClientData.get());
			Native::registerNativeDataInfo<GearmanWorkerData>(s_GearmanWorkerData.get());
			loadSystemlib();
		}
} s_gearman_extension;

HHVM_GET_MODULE(gearman)

} // namespace HPHP
