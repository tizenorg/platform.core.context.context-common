/*
 * Copyright (c) 2014 Samsung Electronics Co., Ltd.
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
#include <provider_iface.h>
#include <context_mgr.h>
#include <context_mgr_iface.h>

static ctx::context_manager_iface *_instance = NULL;

void ctx::context_manager::set_instance(context_manager_iface* mgr)
{
	_instance = mgr;
}

bool ctx::context_manager::register_provider(const char* subject, ctx::context_provider_iface* cp)
{
	IF_FAIL_RETURN_TAG(_instance, false, _E, "Not initialized");
	return _instance->register_provider(subject, cp);
}

bool ctx::context_manager::publish(const char* subject, ctx::json option, int error, ctx::json data_updated, const char* zone)
{
	IF_FAIL_RETURN_TAG(_instance, false, _E, "Not initialized");
	return _instance->publish(subject, option, error, data_updated, zone);
}

bool ctx::context_manager::reply_to_read(const char* subject, ctx::json option, int error, ctx::json data_read, const char* zone)
{
	IF_FAIL_RETURN_TAG(_instance, false, _E, "Not initialized");
	return _instance->reply_to_read(subject, option, error, data_read, zone);
}
