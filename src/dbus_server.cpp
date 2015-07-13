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
#include <json.h>
#include <dbus_server.h>
#include <dbus_server_iface.h>
#include <dbus_listener_iface.h>

static ctx::dbus_server_iface *_instance = NULL;

void ctx::dbus_server::set_instance(dbus_server_iface* svr)
{
	_instance = svr;
}

int64_t ctx::dbus_server::signal_subscribe(const char* sender, const char* path, const char* iface, const char* name, dbus_listener_iface* listener)
{
	IF_FAIL_RETURN_TAG(_instance, false, _E, "Not initialized");
	return _instance->signal_subscribe(sender, path, iface, name, listener);
}

void ctx::dbus_server::signal_unsubscribe(int64_t subscription_id)
{
	IF_FAIL_VOID_TAG(_instance, _E, "Not initialized");
	_instance->signal_unsubscribe(subscription_id);
}
