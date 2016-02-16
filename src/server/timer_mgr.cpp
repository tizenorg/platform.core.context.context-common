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
#include <timer_listener_iface.h>
#include <timer_mgr.h>
#include <timer_mgr_iface.h>

static ctx::timer_manager_iface *_instance = NULL;

void ctx::timer_manager::set_instance(ctx::timer_manager_iface* mgr)
{
	_instance = mgr;
}

int ctx::timer_manager::set_for(int interval, timer_listener_iface* listener, void* user_data)
{
	IF_FAIL_RETURN_TAG(_instance, false, _E, "Not initialized");
	return _instance->set_for(interval, listener, user_data);
}

int ctx::timer_manager::set_at(int hour, int min, int day_of_week, timer_listener_iface* listener, void* user_data)
{
	IF_FAIL_RETURN_TAG(_instance, false, _E, "Not initialized");
	return _instance->set_at(hour, min, day_of_week, listener, user_data);
}

void ctx::timer_manager::remove(int timer_id)
{
	IF_FAIL_VOID_TAG(_instance, _E, "Not initialized");
	_instance->remove(timer_id);
}
