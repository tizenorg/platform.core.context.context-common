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

#ifndef __CONTEXT_DATABASE_LISTENER_INTERFACE_H__
#define __CONTEXT_DATABASE_LISTENER_INTERFACE_H__

#include <vector>

namespace ctx {
	/* Forward Declaration */
	class json;

	class db_listener_iface {
		public:
			virtual ~db_listener_iface() {}

			/**
			 * @brief	Called when a table creation is done.
			 * @param[in]	query_id	The query id passed through ctx::db_manager::create_table().
			 * @param[in]	error		Error code. If success, 0. Otherwise, a negative value.
			 */
			virtual void on_creation_result_received(unsigned int query_id, int error) = 0;

			/**
			 * @brief	Called when a record insertion is done.
			 * @param[in]	query_id	The query id passed through ctx::db_manager::insert().
			 * @param[in]	error		Error code. If success, 0. Otherwise, a negative value.
			 * @param[in]	row_id		The row id of the inserted record.
			 */
			virtual void on_insertion_result_received(unsigned int query_id, int error, int64_t row_id) = 0;

			/**
			 * @brief	Called when a query execution is done.
			 * @param[in]	query_id	The query id passed through ctx::db_manager::execute().
			 * @param[in]	error		Error code. If success, 0. Otherwise, a negative value.
			 * @param[in]	records		Data records retreived.
			 */
			virtual void on_query_result_received(unsigned int query_id, int error, std::vector<json>& records) = 0;
	};

}	/* namespace ctx */

#endif	/* End of __CONTEXT_DATABASE_LISTENER_INTERFACE_H__ */
