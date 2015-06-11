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

#ifndef __CONTEXT_DB_MANAGER_H__
#define __CONTEXT_DB_MANAGER_H__

#include <cstddef>
#include <sys/types.h>
#include <vector>

namespace ctx {
	/* Forward Declaration */
	class json;
	class db_listener_iface;

	namespace db_manager {
		/**
		 * @brief		Create a table if not exists. Async.
		 * @details		The column @c row_id is created by default, thus do not use the same column name.
		 *				It is the primary of the auto-increment integer type.
		 * @param[in]	query_id	This number will be returned through db_listener_iface::on_creation_result_received().
		 * @param[in]	table_name	A table name to be created.
		 * @param[in]	columns		Columns. In SQL format. INTEGER, REAL, and TEXT types are allowed.
		 * @param[in]	option		Additional options. Allows NULL.
		 * @param[in]	listener	A listner object to receive the result. Allows NULL.
		 */
		bool create_table(unsigned int query_id, const char* table_name, const char* columns, const char* option = NULL, db_listener_iface* listener = NULL);

		/**
		 * @brief		Insert a record to a table. Async.
		 * @param[in]	query_id	This number will be returned through db_listener_iface::on_insertion_result_received().
		 * @param[in]	table_name	A table name in which the record is inserted.
		 * @param[in]	record		A json object containing key, value pairs.
		 * @param[in]	listener	A listner object to receive the result. Allows NULL.
		 */
		bool insert(unsigned int query_id, const char* table_name, json& record, db_listener_iface* listener = NULL);

		/**
		 * @brief		Execute a SQL query. Async.
		 * @param[in]	query_id	This number will be returned through db_listener_iface::on_query_result_received().
		 * @param[in]	query		A query to be executed.
		 * @param[in]	listener	A listner object to receive the result.
		 */
		bool execute(unsigned int query_id, const char* query, db_listener_iface* listener);

		/**
		 * @brief		Insert a record to a table. Sync.
		 * @attention	This cannot be used in the main thread.
		 * @param[in]	table_name	A table name in which the record is inserted.
		 * @param[in]	record		A json object containing key, value pairs.
		 * @param[out]	row_id		The row id of the inserted record. If fails, a negative integer.
		 */
		bool insert_sync(const char* table_name, json& record, int64_t* row_id);

		/**
		 * @brief		Execute a SQL query. Sync.
		 * @attention	This cannot be used in the main thread.
		 * @param[in]	query		A query to be executed.
		 * @param[out]	records		Query result.
		 */
		bool execute_sync(const char* query, std::vector<json>* records);

	}	/* namespace ctx::db_manager */
}	/* namespace ctx */

#endif	/* __CONTEXT_DB_MANAGER_H__ */
