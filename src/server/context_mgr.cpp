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
#include <provider_iface.h>
#include <context_mgr.h>
#include <context_mgr_iface.h>

static ctx::context_manager_iface *_instance = NULL;

void ctx::context_manager::set_instance(context_manager_iface* mgr)
{
	_instance = mgr;
}

bool ctx::context_manager::register_provider(const char* subject, ctx::context_provider_info &provider_info)
{
	IF_FAIL_RETURN_TAG(_instance, false, _E, "Not initialized");
	return _instance->register_provider(subject, provider_info);
}

bool ctx::context_manager::unregister_provider(const char* subject)
{
	IF_FAIL_RETURN_TAG(_instance, false, _E, "Not initialized");
	return _instance->unregister_provider(subject);
}

bool ctx::context_manager::register_trigger_item(const char *subject, int operation, ctx::json attributes, ctx::json options, const char* owner)
{
	IF_FAIL_RETURN_TAG(_instance, false, _E, "Not initialized");
	return _instance->register_trigger_item(subject, operation, attributes, options, owner);
}

bool ctx::context_manager::unregister_trigger_item(const char *subject)
{
	IF_FAIL_RETURN_TAG(_instance, false, _E, "Not initialized");
	return _instance->unregister_trigger_item(subject);
}

bool ctx::context_manager::publish(const char* subject, ctx::json option, int error, ctx::json data_updated)
{
	IF_FAIL_RETURN_TAG(_instance, false, _E, "Not initialized");
	return _instance->publish(subject, option, error, data_updated);
}

bool ctx::context_manager::reply_to_read(const char* subject, ctx::json option, int error, ctx::json data_read)
{
	IF_FAIL_RETURN_TAG(_instance, false, _E, "Not initialized");
	return _instance->reply_to_read(subject, option, error, data_read);
}
