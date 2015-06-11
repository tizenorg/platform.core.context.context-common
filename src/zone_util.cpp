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
#include <types_internal.h>
#include <zone_util.h>
#include <zone_util_iface.h>

static ctx::zone_util_iface *_instance = NULL;
static GMutex zone_mutex;

void ctx::zone_util::set_instance(ctx::zone_util_iface* zutil)
{
	_instance = zutil;
}

const char* ctx::zone_util::default_zone()
{
	IF_FAIL_RETURN_TAG(_instance, NULL, _E, "Not initialized");
	return _instance->default_zone();
}

ctx::scope_zone_joiner::scope_zone_joiner(const char* z)
{
	g_mutex_lock(&zone_mutex);

	IF_FAIL_VOID_TAG(_instance, _E, "Not initialized");
	prev_zone = _instance->join_by_name(z);
}

ctx::scope_zone_joiner::~scope_zone_joiner()
{
	IF_FAIL_CATCH(_instance);
	IF_FAIL_CATCH(prev_zone);

	_instance->join_to_zone(prev_zone);

CATCH:
	g_mutex_unlock(&zone_mutex);
}
