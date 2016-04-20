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

#include <signal.h>
#include <tzplatform_config.h>
#include <ScopeMutex.h>
#include <DatabaseManager.h>
#include "DatabaseThread.h"

#define CONTEXT_DB_PATH tzplatform_mkpath(TZ_USER_DB, ".context-service.db")

using namespace ctx;

static unsigned int __refCount = 0;
static GMutex __mutex;
static DatabaseThread *__databaseThread = NULL;

SO_EXPORT DatabaseManager::DatabaseManager()
{
	ScopeMutex sm(&__mutex);

	if (++__refCount > 1)
		return;

	__databaseThread = new(std::nothrow) DatabaseThread();
	if (!__databaseThread) {
		_E("Memory allocation failed");
		raise(SIGTERM);
	}

	if (!__databaseThread->open(CONTEXT_DB_PATH) || !__databaseThread->start()) {
		_E("Database initialization failed");
		raise(SIGTERM);
	}
}

SO_EXPORT DatabaseManager::~DatabaseManager()
{
	ScopeMutex sm(&__mutex);

	if (--__refCount > 0)
		return;

	if (!__databaseThread)
		return;

	__databaseThread->stop();
	__databaseThread->close();

	delete __databaseThread;
	__databaseThread = NULL;
}

SO_EXPORT bool DatabaseManager::createTable(unsigned int queryId, const char *tableName, const char *columns, const char *option, IDatabaseListener *listener)
{
	return __databaseThread->createTable(queryId, tableName, columns, option, listener);
}

SO_EXPORT bool DatabaseManager::insert(unsigned int queryId, const char *tableName, Json record, IDatabaseListener *listener)
{
	return __databaseThread->insert(queryId, tableName, record, listener);
}

SO_EXPORT bool DatabaseManager::execute(unsigned int queryId, const char *query, IDatabaseListener *listener)
{
	return __databaseThread->execute(queryId, query, listener);
}

SO_EXPORT bool DatabaseManager::createTableSync(const char *tableName, const char *columns, const char *option)
{
	return __databaseThread->createTableSync(tableName, columns, option);
}

SO_EXPORT bool DatabaseManager::insertSync(const char *tableName, Json record, int64_t *rowId)
{
	return __databaseThread->insertSync(tableName, record, rowId);
}

SO_EXPORT bool DatabaseManager::executeSync(const char *query, std::vector<Json> *records)
{
	return __databaseThread->executeSync(query, records);
}
