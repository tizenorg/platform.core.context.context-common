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
#include <db_listener_iface.h>
#include <db_mgr.h>
#include <db_mgr_iface.h>

static ctx::db_manager_iface *_instance = NULL;

void ctx::db_manager::set_instance(ctx::db_manager_iface* mgr)
{
	_instance = mgr;
}

bool ctx::db_manager::create_table(unsigned int query_id, const char* table_name, const char* columns, const char* option, db_listener_iface* listener)
{
	IF_FAIL_RETURN_TAG(_instance, false, _E, "Not initialized");
	return _instance->create_table(query_id, table_name, columns, option, listener);
}

bool ctx::db_manager::insert(unsigned int query_id, const char* table_name, json& record, db_listener_iface* listener)
{
	IF_FAIL_RETURN_TAG(_instance, false, _E, "Not initialized");
	return _instance->insert(query_id, table_name, record, listener);
}

bool ctx::db_manager::execute(unsigned int query_id, const char* query, db_listener_iface* listener)
{
	IF_FAIL_RETURN_TAG(_instance, false, _E, "Not initialized");
	return _instance->execute(query_id, query, listener);
}

bool ctx::db_manager::insert_sync(const char* table_name, json& record, int64_t* row_id)
{
	IF_FAIL_RETURN_TAG(_instance, false, _E, "Not initialized");
	return _instance->insert_sync(table_name, record, row_id);
}

bool ctx::db_manager::execute_sync(const char* query, std::vector<json>* records)
{
	IF_FAIL_RETURN_TAG(_instance, false, _E, "Not initialized");
	return _instance->execute_sync(query, records);
}
