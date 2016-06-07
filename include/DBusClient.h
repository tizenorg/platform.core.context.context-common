/*
 * Copyright (c) 2015 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __CONTEXT_DBUS_CLIENT_H__
#define __CONTEXT_DBUS_CLIENT_H__

#include <glib.h>
#include <gio/gio.h>
#include <string>
#include <map>
#include <atomic>
#include <IDBusClientListener.h>

namespace ctx {

	class DBusClient {
	public:
		DBusClient();
		~DBusClient();

		void addListener(std::string subject, IDBusClientListener *listener);

		int isSupported(std::string subject);

		int subscribe(std::string subject, Json option, int *reqId, Json *result);
		int unsubscribe(std::string subject, int reqId);
		int read(std::string subject, Json option, int *reqId, Json *result);
		int readSync(std::string subject, Json option, int *reqId, Json *outputData);
		int write(std::string subject, Json inputData);
		int write(std::string subject, Json inputData, Json *result);

		int call(const char *method);

	private:
		static void __onMethodCalled(GDBusConnection *conn, const gchar *sender,
				const gchar *path, const gchar *iface, const gchar *name,
				GVariant *param, GDBusMethodInvocation *invocation, gpointer userData);

		bool __init();
		void __release();
		int __request(int type, int reqId, const char *subject, const char *input,
				std::string *result, std::string *outputData);
		int __request(int type, int reqId, const char *subject, const char *input);
		int __generateReqId();


		static GDBusConnection *__connection;
		static GDBusNodeInfo *__nodeInfo;
		static std::atomic_int __instanceCount;
		static std::map<std::string, IDBusClientListener*> __listenerMap;

	};	/* class ctx::DBusClient */

}	/* namespace ctx */

#endif // __CONTEXT_DBUS_CLIENT_H__
