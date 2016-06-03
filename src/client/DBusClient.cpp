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

#include <Types.h>
#include <DBusTypes.h>
#include <ScopeMutex.h>
#include <DBusClient.h>

#ifdef LEGACY_SECURITY
#include <security-server.h>

static const char* __getCookie()
{
	static char *cookie = NULL;
	static GMutex cookieMutex;

	ctx::ScopeMutex sm(&cookieMutex);

	if (cookie == NULL) {
		int rawSize = security_server_get_cookie_size();
		IF_FAIL_RETURN_TAG(rawSize > 0, NULL, _E, "Invalid cookie size");

		char rawCookie[rawSize];
		int ret = security_server_request_cookie(rawCookie, rawSize);
		IF_FAIL_RETURN_TAG(ret >= 0, NULL, _E, "Failed to get the security cookie");

		cookie = g_base64_encode(reinterpret_cast<guchar*>(rawCookie), rawSize);
		IF_FAIL_RETURN_TAG(cookie, NULL, _E, "Failed to encode the cookie");
	}

	return cookie;
}
#endif

using namespace ctx;

static const gchar __introspection[] =
	"<node>"
	"	<interface name='" DBUS_IFACE "'>"
	"		<method name='" METHOD_RESPOND "'>"
	"			<arg type='i' name='" ARG_REQID "' direction='in'/>"
	"			<arg type='s' name='" ARG_SUBJECT "' direction='in'/>"
	"			<arg type='i' name='" ARG_RESULT_ERR "' direction='in'/>"
	"			<arg type='s' name='" ARG_OUTPUT "' direction='in'/>"
	"		</method>"
	"	</interface>"
	"</node>";

GDBusConnection *DBusClient::__connection = NULL;
GDBusNodeInfo *DBusClient::__nodeInfo = NULL;
std::atomic_int DBusClient::__instanceCount(0);
std::map<std::string, IDBusClientListener*> DBusClient::__listenerMap;

SO_EXPORT DBusClient::DBusClient()
{
	++__instanceCount;
}

SO_EXPORT DBusClient::~DBusClient()
{
	if (--__instanceCount == 0)
		__release();
}

void DBusClient::__onMethodCalled(GDBusConnection *conn, const gchar *sender,
				const gchar *path, const gchar *iface, const gchar *name,
				GVariant *param, GDBusMethodInvocation *invocation, gpointer userData)
{
	IF_FAIL_VOID_TAG(STR_EQ(name, METHOD_RESPOND), _W, "Invalid method: %s", name);

	gint reqId = 0;
	const gchar *subject = NULL;
	gint error = 0;
	const gchar *data = NULL;

	g_variant_get(param, "(i&si&s)", &reqId, &subject, &error, &data);
	_D("[Response] ReqId: %d, Subject: %s, Error: %d", reqId, subject, error);
	IF_FAIL_VOID_TAG(subject && data, _W, "Invalid parameter");

	auto it = __listenerMap.find(subject);
	it->second->onPublish(subject, reqId, error, data);

	g_dbus_method_invocation_return_value(invocation, NULL);
}

bool DBusClient::__init()
{
	static GMutex mutex;
	ScopeMutex sm(&mutex);

	if (__connection)
		return true;

	GError *gerr = NULL;
	gchar *addr = NULL;
	GDBusInterfaceVTable vtable;
	guint regId;

	__nodeInfo = g_dbus_node_info_new_for_xml(__introspection, NULL);
	IF_FAIL_RETURN_TAG(__nodeInfo != NULL, false, _E, "Initialization failed");

#ifdef SYSTEM_SERVICE
	addr = g_dbus_address_get_for_bus_sync(G_BUS_TYPE_SYSTEM, NULL, &gerr);
#else
	addr = g_dbus_address_get_for_bus_sync(G_BUS_TYPE_SESSION, NULL, &gerr);
#endif
	HANDLE_GERROR(gerr);
	IF_FAIL_CATCH_TAG(addr != NULL, _E, "Getting address failed");
	_SD("Address: %s", addr);

	__connection = g_dbus_connection_new_for_address_sync(addr,
			(GDBusConnectionFlags)(G_DBUS_CONNECTION_FLAGS_AUTHENTICATION_CLIENT | G_DBUS_CONNECTION_FLAGS_MESSAGE_BUS_CONNECTION),
			NULL, NULL, &gerr);
	g_free(addr);
	HANDLE_GERROR(gerr);
	IF_FAIL_CATCH_TAG(__connection != NULL, _E, "Connection failed");

	vtable.method_call = __onMethodCalled;
	vtable.get_property = NULL;
	vtable.set_property = NULL;

	regId = g_dbus_connection_register_object(__connection, DBUS_PATH,
			__nodeInfo->interfaces[0], &vtable, NULL, NULL, &gerr);
	HANDLE_GERROR(gerr);
	IF_FAIL_CATCH_TAG(regId > 0, _E, "Object registration failed");

	_I("DBus connection established");
	_D("DBus name: %s", g_dbus_connection_get_unique_name(__connection));
	return true;

CATCH:
	__release();
	return false;
}

void DBusClient::__release()
{
	if (__connection) {
		g_dbus_connection_flush_sync(__connection, NULL, NULL);
		g_dbus_connection_close_sync(__connection, NULL, NULL);
		g_object_unref(__connection);
		__connection = NULL;
	}

	if (__nodeInfo) {
		g_dbus_node_info_unref(__nodeInfo);
		__nodeInfo = NULL;
	}

	_I("DBus connection released");
}

int DBusClient::__request(int type, int reqId, const char *subject, const char *input,
		std::string *result, std::string *outputData)
{
	_D("Requesting: %d, %d, %s", type, reqId, subject);

	if (input == NULL)
		input = EMPTY_JSON_OBJECT;

	/* FIXME: the second param is the security cookie, which is deprected in 3.0.
	 * We need to completely REMOVE this parameter from the dbus protocol. */
	GVariant *param = g_variant_new("(isiss)", type, "", reqId, subject, input);
	IF_FAIL_RETURN_TAG(param, ERR_OUT_OF_MEMORY, _E, "Memory allocation failed");

	GError *err = NULL;
	GVariant *response = g_dbus_connection_call_sync(__connection, DBUS_DEST, DBUS_PATH, DBUS_IFACE,
			METHOD_REQUEST, param, NULL, G_DBUS_CALL_FLAGS_NONE, DBUS_TIMEOUT, NULL, &err);
	HANDLE_GERROR(err);
	IF_FAIL_RETURN_TAG(response, ERR_OPERATION_FAILED, _E, "Method call failed");

	gint error = ERR_OPERATION_FAILED;
	const gchar *resultStr = NULL;
	const gchar *dataStr = NULL;

	g_variant_get(response, "(i&s&s)", &error, &resultStr, &dataStr);

	if (result && resultStr)
		*result = resultStr;

	if (outputData && dataStr)
		*outputData = dataStr;

	g_variant_unref(response);

	return error;
}

int DBusClient::__request(int type, int reqId, const char* subject, const char* input)
{
	_D("Requesting: %d, %d, %s", type, reqId, subject);

	if (input == NULL)
		input = EMPTY_JSON_OBJECT;

#ifdef LEGACY_SECURITY
	const char *cookie = __getCookie();
	IF_FAIL_RETURN_TAG(cookie, ERR_OPERATION_FAILED, _E, "Cookie generation failed");
	GVariant *param = g_variant_new("(isiss)", type, cookie, reqId, subject, input);
#else
	GVariant *param = g_variant_new("(isiss)", type, "", reqId, subject, input);
#endif
	IF_FAIL_RETURN_TAG(param, ERR_OUT_OF_MEMORY, _E, "Memory allocation failed");

	GError *err = NULL;
	g_dbus_connection_call(__connection, DBUS_DEST, DBUS_PATH, DBUS_IFACE,
			METHOD_REQUEST, param, NULL, G_DBUS_CALL_FLAGS_NONE, DBUS_TIMEOUT, NULL, NULL, &err);

	if (err) {
		HANDLE_GERROR(err);
		return ERR_OPERATION_FAILED;
	}

	return ERR_NONE;
}

int DBusClient::__generateReqId()
{
	static GMutex mutex;
	static int reqId = 0;

	ctx::ScopeMutex sm(&mutex);

	/* Overflow handling */
	if (++reqId < 0)
		reqId = 1;

	return reqId;
}

SO_EXPORT void DBusClient::addListener(std::string subject, IDBusClientListener *listener)
{
	_D("Registering the listener for '%s'", subject.c_str());

	static GMutex mutex;
	ctx::ScopeMutex sm(&mutex);

	__listenerMap[subject] = listener;
}

SO_EXPORT int DBusClient::isSupported(std::string subject)
{
	IF_FAIL_RETURN(__init(), ERR_OPERATION_FAILED);

	return __request(REQ_SUPPORT, __generateReqId(), subject.c_str(), NULL, NULL, NULL);
}

SO_EXPORT int DBusClient::subscribe(std::string subject, Json option, int *reqId, Json *result)
{
	IF_FAIL_RETURN(__init(), ERR_OPERATION_FAILED);
	ASSERT_NOT_NULL(reqId);

	*reqId = __generateReqId();

	_I("[Subscribe] ReqId: %d, Subject: %s", *reqId, subject.c_str());

	std::string resultStr;
	int error = __request(REQ_SUBSCRIBE, *reqId, subject.c_str(), option.str().c_str(), &resultStr, NULL);

	if (result)
		*result = resultStr;

	_D("Error: %#x", error);
	_SD("Result: %s", resultStr.c_str());

	return error;
}

SO_EXPORT int DBusClient::unsubscribe(std::string subject, int reqId)
{
	IF_FAIL_RETURN(__init(), ERR_OPERATION_FAILED);

	_I("[Unsubscribe] ReqId: %d, Subject: %s", reqId, subject.c_str());

	return __request(REQ_UNSUBSCRIBE, reqId, subject.c_str(), NULL, NULL, NULL);
}

SO_EXPORT int DBusClient::read(std::string subject, Json option, int *reqId, Json *result)
{
	IF_FAIL_RETURN(__init(), ERR_OPERATION_FAILED);
	ASSERT_NOT_NULL(reqId);

	*reqId = __generateReqId();

	_I("[Read] ReqId: %d, Subject: %s", *reqId, subject.c_str());

	std::string resultStr;
	int error = __request(REQ_READ, *reqId, subject.c_str(), option.str().c_str(), &resultStr, NULL);

	if (result)
		*result = resultStr;

	_D("Error: %#x", error);
	_SD("Result: %s", resultStr.c_str());

	return error;
}

SO_EXPORT int DBusClient::readSync(std::string subject, Json option, int *reqId, Json *outputData)
{
	IF_FAIL_RETURN(__init(), ERR_OPERATION_FAILED);
	ASSERT_NOT_NULL(reqId);
	ASSERT_NOT_NULL(outputData);

	*reqId = __generateReqId();

	_I("[ReadSync] ReqId: %d, Subject: %s", *reqId, subject.c_str());

	std::string output;
	int error = __request(REQ_READ_SYNC, *reqId, subject.c_str(), option.str().c_str(), NULL, &output);

	*outputData = output;

	_D("Error: %#x", error);
	_SD("Data: %s", output.c_str());

	return error;
}

SO_EXPORT int DBusClient::write(std::string subject, Json inputData)
{
	IF_FAIL_RETURN(__init(), ERR_OPERATION_FAILED);

	int reqId = __generateReqId();

	_I("[Write] ReqId: %d, Subject: %s", reqId, subject.c_str());
	_SD("Data: %s", inputData.str().c_str());

	int error = __request(REQ_WRITE, reqId, subject.c_str(), inputData.str().c_str());
	_D("Error: %#x", error);

	return error;
}

SO_EXPORT int DBusClient::write(std::string subject, Json inputData, Json *result)
{
	IF_FAIL_RETURN(__init(), ERR_OPERATION_FAILED);

	int reqId = __generateReqId();

	_I("[Write with reply] ReqId: %d, Subject: %s", reqId, subject.c_str());
	_SD("Data: %s", inputData.str().c_str());

	std::string resultStr;
	int error = __request(REQ_WRITE, reqId, subject.c_str(), inputData.str().c_str(), &resultStr, NULL);

	if (result)
		*result = resultStr;

	_D("Error: %#x", error);
	_SD("Result: %s", resultStr.c_str());

	return error;
}

SO_EXPORT int DBusClient::call(const char *method)
{
	int ret = ERR_NONE;
	GError *err = NULL;

	GVariant *response = g_dbus_connection_call_sync(__connection, DBUS_DEST, DBUS_PATH, DBUS_IFACE,
			method, NULL, NULL, G_DBUS_CALL_FLAGS_NONE, DBUS_TIMEOUT, NULL, &err);

	if (response) {
		g_variant_unref(response);
		return ERR_NONE;
	}

	ret = ERR_OPERATION_FAILED;
	if (err->code == G_DBUS_ERROR_ACCESS_DENIED)
		ret = ERR_PERMISSION_DENIED;

	HANDLE_GERROR(err);
	return ret;
}
