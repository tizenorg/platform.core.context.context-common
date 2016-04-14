/*
 * Copyright (c) 2016 Samsung Electronics Co., Ltd.
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

#ifndef _CONTEXT_DATABASE_MANAGER_H_
#define _CONTEXT_DATABASE_MANAGER_H_

#include <IDatabaseListener.h>

namespace ctx {

	class DatabaseManager {
	public:
		DatabaseManager();
		~DatabaseManager();

		/**
		 * @brief		Creates a table if not exists. Async.
		 * @details		The column @c row_id is created by default, thus do not use the same column name.
		 *				It is the primary of the auto-increment integer type.
		 * @param[in]	queryId		This number will be returned through IDatabaseListener::on_creation_result_received().
		 * @param[in]	tableName	A table name to be created.
		 * @param[in]	columns		Columns. In SQL format. INTEGER, REAL, and TEXT types are allowed.
		 * @param[in]	option		Additional options. Allows NULL.
		 * @param[in]	listener	A listner object to receive the result. Allows NULL.
		 */
		bool createTable(unsigned int queryId, const char *tableName, const char *columns, const char *option = NULL, IDatabaseListener *listener = NULL);

		/**
		 * @brief		Inserts a record to a table. Async.
		 * @param[in]	queryId		This number will be returned through IDatabaseListener::on_insertion_result_received().
		 * @param[in]	tableName	A table name in which the record is inserted.
		 * @param[in]	record		A json object containing key, value pairs.
		 * @param[in]	listener	A listner object to receive the result. Allows NULL.
		 */
		bool insert(unsigned int queryId, const char *tableName, Json record, IDatabaseListener *listener = NULL);

		/**
		 * @brief		Executes a SQL query. Async.
		 * @param[in]	queryId		This number will be returned through IDatabaseListener::on_query_result_received().
		 * @param[in]	query		A query to be executed.
		 * @param[in]	listener	A listner object to receive the result.
		 */
		bool execute(unsigned int queryId, const char *query, IDatabaseListener *listener = NULL);

		/**
		 * @brief		creates a table if not exists. Async.
		 * @param[in]	tableName	A table name to be created.
		 * @param[in]	columns		Columns. In SQL format. INTEGER, REAL, and TEXT types are allowed.
		 * @param[in]	option		Additional options. Allows NULL.
		 */
		bool createTableSync(const char *tableName, const char *columns, const char *option = NULL);

		/**
		 * @brief		Inserts a record to a table. Sync.
		 * @attention	This blocks the current thread.
		 * @param[in]	tableName	A table name in which the record is inserted.
		 * @param[in]	record		A json object containing key, value pairs.
		 * @param[out]	rowId		The row id of the inserted record. If fails, a negative integer.
		 */
		bool insertSync(const char *tableName, Json record, int64_t *rowId);

		/**
		 * @brief		Executes a SQL query. Sync.
		 * @attention	This blocks the current thread.
		 * @param[in]	query		A query to be executed.
		 * @param[out]	records		Query result.
		 */
		bool executeSync(const char *query, std::vector<Json> *records);
	};

}	/* namespace ctx */

#endif	/* _CONTEXT_DATABASE_MANAGER_H_ */
