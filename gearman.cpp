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

#include "hphp/runtime/ext/extension.h"
#include "hphp/runtime/ext/std/ext_std_function.h"
#include "hphp/runtime/base/builtin-functions.h"
#include "hphp/runtime/vm/native-data.h"
#include "hphp/runtime/base/array-init.h"
#include "hphp/system/systemlib.h"

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
			Variant func;
		};

		GearmanWorkerData() {}
		~GearmanWorkerData() {}

		typedef std::shared_ptr<Impl> ImplPtr;
		std::vector<std::shared_ptr<UserDefinedFunc>> m_udfs;
		ImplPtr m_impl;
};

const StaticString s_GearmanJobData("GearmanJobData");
class GearmanJobData {
	public:
		class Impl {
			public:
			Impl() {
			}
			~Impl() {
			}
		};

		GearmanJobData() {}
		~GearmanJobData() {}

		typedef std::shared_ptr<Impl> Implptr;
		Implptr m_impl;
};

const StaticString s_GearmanTaskData("GearmanTaskData");
class GearmanTaskData {
	public:
		class Impl {
			public:
			Impl() {
			}
			~Impl() {
			}
			gearman_task_st task;
		};

		GearmanTaskData() {}
		~GearmanTaskData() {}

		typedef std::shared_ptr<Impl> Implptr;
		Implptr m_impl;
};

void HHVM_METHOD(GearmanClient, __construct) {
	auto data = Native::data<GearmanClientData>(this_);
	data->m_impl.reset(new GearmanClientData::Impl);
}

bool HHVM_METHOD(GearmanClient, addServer, const String& host, int64_t port) {
	auto data = Native::data<GearmanClientData>(this_);
	const char* myHost = host.empty() ? nullptr : host.c_str();
	gearman_return_t ret = gearman_client_add_server(&data->m_impl->client, myHost, port);
	return ret == GEARMAN_SUCCESS;
}

bool HHVM_METHOD(GearmanClient, addServers, const String& servers) {
	auto data = Native::data<GearmanClientData>(this_);
	const char* myServers = servers.empty() ? nullptr : servers.c_str();
	gearman_return_t ret = gearman_client_add_servers(&data->m_impl->client, myServers);
	return ret == GEARMAN_SUCCESS;
}

#define GEARMAN_CLIENT_DO(function_name, workload, unique) \
	auto data = Native::data<GearmanClientData>(this_); \
	const char* myFunctionName = function_name.empty() ? nullptr : function_name.c_str(); \
	const char* myWorkload = workload.empty() ? nullptr : workload.c_str(); \
	size_t workload_size = workload.length(); \
	const char* myUnique = unique.empty() ? NULL : unique.c_str(); \


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
	printf("%s\n", myFunctionName);
	return String();
	gearman_return_t ret = gearman_client_do_high_background(&data->m_impl->client, 
								myFunctionName, myUnique, 
								myWorkload, workload_size, 
								handle);
	if (ret != GEARMAN_SUCCESS) {
		return String();
	}
	return String(handle);
}

bool HHVM_METHOD(GearmanClient, ping, const String& workload) {
	auto data = Native::data<GearmanClientData>(this_);
	const char* myWorkload = workload.empty() ? nullptr : workload.c_str();
	size_t workload_size = workload.length();

	gearman_return_t ret = gearman_client_echo(&data->m_impl->client, myWorkload, workload_size);
	return ret == GEARMAN_SUCCESS;
}

bool HHVM_METHOD(GearmanClient, setTimeout, int64_t timeout) {
	auto data = Native::data<GearmanClientData>(this_);
	gearman_client_set_timeout(&data->m_impl->client, timeout);
	return true;
}

Array HHVM_METHOD(GearmanClient, jobStatus, const String& job_handle) {
	auto data = Native::data<GearmanClientData>(this_);
	bool is_known, is_running;
	uint32_t numerator, denominator;

	gearman_return_t ret = gearman_client_job_status(&data->m_impl->client, job_handle.c_str(), 
				&is_known, &is_running, &numerator, &denominator);
	
	Array status;
	if (ret == GEARMAN_SUCCESS) {
		status.append(is_known);
		status.append(is_running);
		status.append((int64_t) numerator);
		status.append((int64_t) denominator);
	}

	return status;
}

bool HHVM_METHOD(GearmanClient, setOptions, int64_t options) {
	//auto data = Native::data<GearmanClientData>(this_);
	return true;
}

void HHVM_METHOD(GearmanWorker, __construct) {
	auto data = Native::data<GearmanWorkerData>(this_);
	data->m_impl.reset(new GearmanWorkerData::Impl);
}

bool HHVM_METHOD(GearmanWorker, addServer, const String& host, int64_t port) {
	auto data = Native::data<GearmanWorkerData>(this_);
	const char* myHost = host.empty() ? nullptr : host.c_str();
	gearman_return_t ret = gearman_worker_add_server(&data->m_impl->worker, myHost, port);
	return ret == GEARMAN_SUCCESS;
}

bool HHVM_METHOD(GearmanWorker, addServers, const String& servers) {
	auto data = Native::data<GearmanWorkerData>(this_);
	const char* myServers = servers.empty() ? nullptr : servers.c_str();
	gearman_return_t ret = gearman_worker_add_servers(&data->m_impl->worker, myServers);
	return ret == GEARMAN_SUCCESS;
}

static void* add_function_callback(gearman_job_st* job, void* context, size_t* result_size, gearman_return_t* ret) {
	Array params = Array::Create();
	GearmanWorkerData::UserDefinedFunc* udf = (GearmanWorkerData::UserDefinedFunc*) context;
	is_callable(udf->func);
	//vm_call_user_func(udf->func, params);
	*result_size = 0;
	return NULL;
}

bool HHVM_METHOD(GearmanWorker, addFunction, const String& function_name, 
					const Variant& function, const VRefParam& context, int64_t timeout /* = 0 */) {
	auto data = Native::data<GearmanWorkerData>(this_);
	if (function_name.empty()) {
		return false;
	}
	if (!is_callable(function)) {
		raise_warning("Not a valid callback function %s",
				function.toString().data());
		return false;
	}

	auto udf = std::make_shared<GearmanWorkerData::UserDefinedFunc>();
	if (gearman_worker_add_function(&data->m_impl->worker, function_name.data(), 
									timeout, add_function_callback, 
									(void*) udf.get()) == GEARMAN_SUCCESS) {
		udf->func = function;
		data->m_udfs.push_back(udf);
		return true;
	}
	return false;
}

bool HHVM_METHOD(GearmanWorker, setTimeout, int64_t timeout) {
	auto data = Native::data<GearmanWorkerData>(this_);
	gearman_worker_set_timeout(&data->m_impl->worker, timeout);
	return true;
}


bool HHVM_METHOD(GearmanWorker, work) {
	auto data = Native::data<GearmanWorkerData>(this_);
	gearman_return_t ret = gearman_worker_work(&data->m_impl->worker);
	return ret == GEARMAN_SUCCESS;
}

void HHVM_METHOD(GearmanJob, __construct) {
	auto data = Native::data<GearmanJobData>(this_);
	data->m_impl.reset(new GearmanJobData::Impl);
}

void HHVM_METHOD(GearmanTask, __construct) {
	auto data = Native::data<GearmanTaskData>(this_);
	data->m_impl.reset(new GearmanTaskData::Impl);
}

String HHVM_METHOD(GearmanTask, data) {
	auto data = Native::data<GearmanTaskData>(this_);
	char* result = (char*) gearman_task_data(&data->m_impl->task);
	size_t result_len = gearman_task_data_size(&data->m_impl->task);
	return String(result, result_len, CopyString);
}

int64_t HHVM_METHOD(GearmanTask, dataSize) {
	auto data = Native::data<GearmanTaskData>(this_);
	return gearman_task_data_size(&data->m_impl->task);
}

static class GearmanExtension : public Extension {
	public:
		GearmanExtension() : Extension("gearman") {}
		void moduleInit() override {
			HHVM_ME(GearmanClient, __construct);
			HHVM_ME(GearmanClient, doNormal);
			HHVM_ME(GearmanClient, doHigh);
			HHVM_ME(GearmanClient, doBackground);
			HHVM_ME(GearmanClient, doHighBackground);
			HHVM_ME(GearmanClient, ping);
			HHVM_ME(GearmanClient, addServer);
			HHVM_ME(GearmanClient, addServers);
			HHVM_ME(GearmanClient, setTimeout);
			HHVM_ME(GearmanClient, jobStatus);
			HHVM_ME(GearmanClient, setOptions);

			HHVM_ME(GearmanWorker, __construct);
			HHVM_ME(GearmanWorker, work);
			HHVM_ME(GearmanWorker, addServer);
			HHVM_ME(GearmanWorker, addServers);
			HHVM_ME(GearmanWorker, addFunction);
			HHVM_ME(GearmanWorker, setTimeout);

			HHVM_ME(GearmanJob, __construct);
			
			HHVM_ME(GearmanTask, __construct);
			HHVM_ME(GearmanTask, data);
			HHVM_ME(GearmanTask, dataSize);

			Native::registerNativeDataInfo<GearmanClientData>(s_GearmanClientData.get());
			Native::registerNativeDataInfo<GearmanWorkerData>(s_GearmanWorkerData.get(), Native::NDIFlags::NO_SWEEP);
			Native::registerNativeDataInfo<GearmanJobData>(s_GearmanJobData.get());
			Native::registerNativeDataInfo<GearmanTaskData>(s_GearmanTaskData.get());
			loadSystemlib();
		}
} s_gearman_extension;

HHVM_GET_MODULE(gearman)

} // namespace HPHP
