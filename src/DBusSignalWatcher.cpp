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

#include <types_internal.h>
#include <scope_mutex.h>
#include <DBusSignalWatcher.h>

using namespace ctx;

unsigned int DBusSignalWatcher::__systemBusCnt = 0;
unsigned int DBusSignalWatcher::__sessionBusCnt = 0;
GDBusConnection *DBusSignalWatcher::__systemBus = NULL;
GDBusConnection *DBusSignalWatcher::__sessionBus = NULL;

static void __signal_cb(GDBusConnection *conn, const gchar *sender,
		const gchar *path, const gchar *iface, const gchar *name,
		GVariant *param, gpointer userdata)
{
	IF_FAIL_VOID_TAG(userdata, _W, "user_data cannot be null");
	IDBusSignalListener *listener = static_cast<IDBusSignalListener*>(userdata);
	listener->onSignal(sender, path, iface, name, param);
}

DBusSignalWatcher::DBusSignalWatcher(DBusType type)
	: __busType(type)
{
	scope_mutex sm(&__mutex);

	if (__busType == DBusType::SYSTEM) {
		if (__systemBusCnt++ == 0)
			__openBus(G_BUS_TYPE_SYSTEM, __systemBus);
	} else {
		if (__sessionBusCnt++ == 0)
			__openBus(G_BUS_TYPE_SESSION, __sessionBus);
	}
}

DBusSignalWatcher::~DBusSignalWatcher()
{
	scope_mutex sm(&__mutex);

	if (__busType == DBusType::SYSTEM) {
		if (--__systemBusCnt == 0)
			__closeBus(__systemBus);
	} else {
		if (--__systemBusCnt == 0)
			__closeBus(__sessionBus);
	}
}

int64_t DBusSignalWatcher::watch(const char *sender, const char *path, const char *iface, const char *name, IDBusSignalListener *listener)
{
	GDBusConnection *bus = (__busType == DBusType::SYSTEM ? __systemBus : __sessionBus);
	IF_FAIL_RETURN_TAG(bus, -1, _E, "Dbus not connected");

	guint sid = g_dbus_connection_signal_subscribe(bus,
			sender, iface, name, path, NULL, G_DBUS_SIGNAL_FLAGS_NONE,
			__signal_cb, listener, NULL);

	return static_cast<int64_t>(sid);
}

void DBusSignalWatcher::unwatch(int64_t signal_id)
{
	IF_FAIL_VOID_TAG(signal_id >= 0, _W, "Invalid parameter");
	GDBusConnection *bus = (__busType == DBusType::SYSTEM ? __systemBus : __sessionBus);
	g_dbus_connection_signal_unsubscribe(bus, static_cast<guint>(signal_id));
}

void DBusSignalWatcher::__openBus(GBusType type, GDBusConnection *&bus)
{
	GError *gerr = NULL;
	gchar *addr = g_dbus_address_get_for_bus_sync(type, NULL, &gerr);
	HANDLE_GERROR(gerr);
	IF_FAIL_VOID(addr);

	bus = g_dbus_connection_new_for_address_sync(addr,
			(GDBusConnectionFlags)(G_DBUS_CONNECTION_FLAGS_AUTHENTICATION_CLIENT | G_DBUS_CONNECTION_FLAGS_MESSAGE_BUS_CONNECTION),
			NULL, NULL, &gerr);
	g_free(addr);
	HANDLE_GERROR(gerr);
	IF_FAIL_VOID(bus);

	_D("%s Bus Connected", type == G_BUS_TYPE_SYSTEM ? "SYSTEM" : "SESSION");
}

void DBusSignalWatcher::__closeBus(GDBusConnection *&bus)
{
	IF_FAIL_VOID(bus);

	g_dbus_connection_close_sync(bus, NULL, NULL);
	g_object_unref(bus);
	bus = NULL;

	_D("%s Bus Closed", __busType == DBusType::SYSTEM ? "SYSTEM" : "SESSION");
}
