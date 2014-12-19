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

class GearmanWorkerData {
	public:
		class Impl {
		};
		GearmanWorkerData() {}
		~GearmanWorkerData() {}

		typedef std::shared_ptr<Impl> ImplPtr;
		ImplPtr m_impl;
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

String HHVM_METHOD(GearmanClient, doNormal, const String& function_name,
									   const String& workload,
									   const String& unique /*= null*/) {

	auto data = Native::data<GearmanClientData>(this_);

	const char* myFunctionName = function_name.empty() ? nullptr : function_name.c_str();
	const char* myWorkload = workload.empty() ? nullptr : workload.c_str();
	size_t workload_size = workload.length();
	const char* myUnique = workload.empty() ? nullptr : unique.c_str();
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


void HHVM_METHOD(GearmanWorker, __construct) {

}

bool HHVM_METHOD(GearmanWorker, work) {
	return true;
}

static class GearmanExtension : public Extension {
	public:
		GearmanExtension() : Extension("gearman") {}
		virtual void moduleInit() {
			HHVM_ME(GearmanClient, __construct);
			HHVM_ME(GearmanClient, doNormal);
			HHVM_ME(GearmanClient, addServer);

			HHVM_ME(GearmanWorker, __construct);
			HHVM_ME(GearmanWorker, work);

			Native::registerNativeDataInfo<GearmanClientData>(s_GearmanClientData.get());
			loadSystemlib();
		}
} s_gearman_extension;

HHVM_GET_MODULE(gearman)

} // namespace HPHP
