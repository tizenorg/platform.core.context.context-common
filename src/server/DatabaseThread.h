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

#ifndef _CONTEXT_DATABASE_THREAD_H_
#define _CONTEXT_DATABASE_THREAD_H_

#include <sqlite3.h>
#include <EventDrivenThread.h>
#include <IDatabaseListener.h>

namespace ctx {

	class DatabaseThread : public EventDrivenThread {
	public:
		DatabaseThread();
		~DatabaseThread();

		bool open(const char *dbPath);
		void close();

		bool createTable(unsigned int queryId, const char *tableName, const char *columns, const char *option = NULL, IDatabaseListener *listener = NULL);
		bool insert(unsigned int queryId, const char *tableName, Json record, IDatabaseListener *listener = NULL);
		bool execute(unsigned int queryId, const char *query, IDatabaseListener *listener = NULL);
		bool createTableSync(const char *tableName, const char *columns, const char *option = NULL);
		bool insertSync(const char *tableName, Json record, int64_t *rowId);
		bool executeSync(const char *query, std::vector<Json> *records);

	private:
		enum QueryType {
			QUERY_CREATE_TABLE = 1,
			QUERY_INSERT,
			QUERY_EXECUTE,
		};

		struct QueryInfo {
			unsigned int id;
			IDatabaseListener *listener;
			std::string query;
		};

		struct QueryResult {
			int type;
			unsigned int id;
			int error;
			IDatabaseListener *listener;
			std::vector<Json> *result;
		};

		void onEvent(int type, void* data);
		void deleteEvent(int type, void* data);

		std::string __composeCreate(const char *tableName, const char *columns, const char *option);
		std::string __composeInsert(const char *tableName, Json record);

		void __execute(QueryType type, unsigned int queryId, const char *query, IDatabaseListener *listener);
		void __dispatchResult(QueryType type, unsigned int queryId, IDatabaseListener *listener, int error, std::vector<Json> *result);

		static int __executionCb(void *userData, int dim, char **value, char **column);
		static gboolean __dispatcher(gpointer data);

		static sqlite3 *__databaseHandle;
		static GMutex __execMutex;
	};

}	/* namespace ctx */

#endif	/* _CONTEXT_DATABASE_THREAD_H_ */
