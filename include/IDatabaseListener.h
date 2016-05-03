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

#ifndef _CONTEXT_I_DATABASE_LISTENER_H_
#define _CONTEXT_I_DATABASE_LISTENER_H_

#include <vector>
#include <Json.h>
#include <Types.h>

namespace ctx {

	class SO_EXPORT IDatabaseListener {
	public:
		virtual ~IDatabaseListener();

		/**
		 * @brief	Called when a table creation is done.
		 * @param[in]	queryId		The query id passed through ctx::DatabaseManager::createTable().
		 * @param[in]	error		Error code. If success, 0. Otherwise, a negative value.
		 */
		virtual void onTableCreated(unsigned int queryId, int error) = 0;

		/**
		 * @brief	Called when a record insertion is done.
		 * @param[in]	queryId		The query id passed through ctx::DatabaseManager::insert().
		 * @param[in]	error		Error code. If success, 0. Otherwise, a negative value.
		 * @param[in]	rowId		The row id of the inserted record.
		 */
		virtual void onInserted(unsigned int queryId, int error, int64_t rowId) = 0;

		/**
		 * @brief	Called when a query execution is done.
		 * @param[in]	queryId		The query id passed through ctx::DatabaseManager::execute().
		 * @param[in]	error		Error code. If success, 0. Otherwise, a negative value.
		 * @param[in]	records		Data records retreived.
		 */
		virtual void onExecuted(unsigned int queryId, int error, std::vector<Json>& records) = 0;
	};

}	/* namespace ctx */

#endif	/* _CONTEXT_I_DATABASE_LISTENER_H_ */
