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

#include <glib.h>
#include <gio/gio.h>

#include <types_internal.h>
#include <scope_mutex.h>
#include "response_handler.h"
#include "dbus_client.h"

static GDBusConnection *dbus_connection = NULL;
static GDBusNodeInfo *dbus_node_info = NULL;

static const gchar introspection_xml[] =
	"<node>"
	"	<interface name='"DBUS_IFACE"'>"
	"		<method name='"METHOD_RESPOND"'>"
	"			<arg type='i' name='"ARG_REQID"' direction='in'/>"
	"			<arg type='s' name='"ARG_SUBJECT"' direction='in'/>"
	"			<arg type='i' name='"ARG_RESULT_ERR"' direction='in'/>"
	"			<arg type='s' name='"ARG_OUTPUT"' direction='in'/>"
	"		</method>"
	"	</interface>"
	"</node>";

static void handle_response(const gchar *sender, GVariant *param, GDBusMethodInvocation *invocation)
{
	gint req_id = 0;
	const gchar *subject = NULL;
	gint error = 0;
	const gchar *data = NULL;

	g_variant_get(param, "(i&si&s)", &req_id, &subject, &error, &data);
	_D("[Response] ReqId: %d, Subject: %s, Error: %d", req_id, subject, error);

	ctx::response_handler::deliver(subject, req_id, error, data);
	g_dbus_method_invocation_return_value(invocation, NULL);
}

static void handle_method_call(GDBusConnection *conn, const gchar *sender,
		const gchar *obj_path, const gchar *iface, const gchar *method_name,
		GVariant *param, GDBusMethodInvocation *invocation, gpointer user_data)
{
	IF_FAIL_VOID_TAG(STR_EQ(obj_path, DBUS_PATH), _W, "Invalid path: %s", obj_path);
	IF_FAIL_VOID_TAG(STR_EQ(iface, DBUS_IFACE), _W, "Invalid interface: %s", obj_path);

	if (STR_EQ(method_name, METHOD_RESPOND)) {
		handle_response(sender, param, invocation);
	} else {
		_W("Invalid method: %s", method_name);
	}
}

ctx::dbus_client::dbus_client()
{
}

ctx::dbus_client::~dbus_client()
{
	release();
}

bool ctx::dbus_client::init()
{
	static GMutex connection_mutex;
	ctx::scope_mutex sm(&connection_mutex);

	if (dbus_connection) {
		return true;
	}

	GError *gerr = NULL;

	dbus_node_info = g_dbus_node_info_new_for_xml(introspection_xml, NULL);
	IF_FAIL_RETURN_TAG(dbus_node_info != NULL, false, _E, "Initialization failed");

	gchar *addr = g_dbus_address_get_for_bus_sync(G_BUS_TYPE_SYSTEM, NULL, &gerr);
	HANDLE_GERROR(gerr);
	IF_FAIL_RETURN_TAG(addr != NULL, false, _E, "Getting address failed");
	_SD("Address: %s", addr);

	dbus_connection = g_dbus_connection_new_for_address_sync(addr,
			(GDBusConnectionFlags)(G_DBUS_CONNECTION_FLAGS_AUTHENTICATION_CLIENT | G_DBUS_CONNECTION_FLAGS_MESSAGE_BUS_CONNECTION),
			NULL, NULL, &gerr);
	g_free(addr);
	HANDLE_GERROR(gerr);
	IF_FAIL_RETURN_TAG(dbus_connection != NULL, false, _E, "Connection failed");

	GDBusInterfaceVTable vtable;
	vtable.method_call = handle_method_call;
	vtable.get_property = NULL;
	vtable.set_property = NULL;

	guint reg_id = g_dbus_connection_register_object(dbus_connection, DBUS_PATH,
			dbus_node_info->interfaces[0], &vtable, NULL, NULL, &gerr);
	HANDLE_GERROR(gerr);
	IF_FAIL_RETURN_TAG(reg_id>0, false, _E, "Object registration failed");

	_I("Dbus connection established: %s", g_dbus_connection_get_unique_name(dbus_connection));
	return true;
}

void ctx::dbus_client::release()
{
	if (dbus_connection) {
		g_dbus_connection_flush_sync(dbus_connection, NULL, NULL);
		g_dbus_connection_close_sync(dbus_connection, NULL, NULL);
		g_object_unref(dbus_connection);
		dbus_connection = NULL;
	}

	if (dbus_node_info) {
		g_dbus_node_info_unref(dbus_node_info);
		dbus_node_info = NULL;
	}
}

int ctx::dbus_client::request(
		int type, int req_id, const char* subject, const char* input,
		std::string* req_result, std::string* data_read)
{
	_D("Requesting: %d, %d, %s", type, req_id, subject);

	if (subject == NULL) {
		subject = EMPTY_STRING;
	}

	if (input == NULL) {
		input = EMPTY_JSON_OBJECT;
	}

	/* FIXME: the second param is the security cookie, which is deprected in 3.0.
	 * We need to completely REMOVE this parameter from the dbus protocol. */
	GVariant *param = g_variant_new("(isiss)", type, "", req_id, subject, input);
	IF_FAIL_RETURN_TAG(param, ERR_OUT_OF_MEMORY, _E, "Memory allocation failed");

	GError *err = NULL;
	GVariant *response = g_dbus_connection_call_sync(dbus_connection, DBUS_DEST, DBUS_PATH, DBUS_IFACE,
			METHOD_REQUEST, param, NULL, G_DBUS_CALL_FLAGS_NONE, DBUS_TIMEOUT, NULL, &err);
	HANDLE_GERROR(err);
	IF_FAIL_RETURN_TAG(response, ERR_OPERATION_FAILED, _E, "Method call failed");

	gint _error = ERR_OPERATION_FAILED;
	const gchar *_req_result = NULL;
	const gchar *_data_read = NULL;

	g_variant_get(response, "(i&s&s)", &_error, &_req_result, &_data_read);
	if (req_result) {
		*req_result = _req_result;
	}
	if (data_read) {
		*data_read = _data_read;
	}

	g_variant_unref(response);

	return _error;
}

int ctx::dbus_client::request_with_no_reply(
		int type, int req_id, const char* subject, const char* input)
{
	_D("Requesting: %d, %d, %s", type, req_id, subject);

	if (subject == NULL) {
		subject = EMPTY_STRING;
	}

	if (input == NULL) {
		input = EMPTY_JSON_OBJECT;
	}

	/* FIXME: the second param is the security cookie, which is deprected in 3.0.
	 * We need to completely REMOVE this parameter from the dbus protocol. */
	GVariant *param = g_variant_new("(isiss)", type, "", req_id, subject, input);
	IF_FAIL_RETURN_TAG(param, ERR_OUT_OF_MEMORY, _E, "Memory allocation failed");

	GError *err = NULL;
	g_dbus_connection_call(dbus_connection, DBUS_DEST, DBUS_PATH, DBUS_IFACE,
			METHOD_REQUEST, param, NULL, G_DBUS_CALL_FLAGS_NONE, DBUS_TIMEOUT, NULL, NULL, &err);

	if (err) {
		HANDLE_GERROR(err);
		return ERR_OPERATION_FAILED;
	}

	return ERR_NONE;
}
