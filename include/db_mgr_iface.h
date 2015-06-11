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

#ifndef __CONTEXT_DB_MANAGER_INTERFACE_H__
#define __CONTEXT_DB_MANAGER_INTERFACE_H__

#include <sys/types.h>

namespace ctx {
	/* Forward Declaration */
	class json;
	class db_listener_iface;

	class db_manager_iface {
		public:
			virtual ~db_manager_iface() {}
			virtual bool create_table(unsigned int query_id, const char* table_name, const char* columns, const char* option = NULL, db_listener_iface* listener = NULL) = 0;
			virtual bool insert(unsigned int query_id, const char* table_name, json& record, db_listener_iface* listener = NULL) = 0;
			virtual bool execute(unsigned int query_id, const char* query, db_listener_iface* listener) = 0;
			virtual bool insert_sync(const char* table_name, json& record, int64_t* row_id) = 0;
			virtual bool execute_sync(const char* query, std::vector<json>* records) = 0;
	};	/* class db_manager */

	namespace db_manager {
		void set_instance(db_manager_iface* mgr);
	}	/* namespace ctx::db_manager */
}	/* namespace ctx */

#endif	/* __CONTEXT_DB_MANAGER_INTERFACE_H__ */
