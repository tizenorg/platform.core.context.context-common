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

#include <Types.h>
#include <ScopeMutex.h>
#include "DatabaseThread.h"

using namespace ctx;

DatabaseThread::DatabaseThread()
{
	g_mutex_init(&__execMutex);
}

DatabaseThread::~DatabaseThread()
{
	g_mutex_clear(&__execMutex);
}

bool DatabaseThread::open(const char *dbPath)
{
	sqlite3 *db = NULL;
	char *err = NULL;
	int ret;

	ret = sqlite3_open(dbPath, &db);
	IF_FAIL_RETURN_TAG(ret == SQLITE_OK, false, _E, "Path: %s / Error: %s", dbPath, sqlite3_errmsg(db));

	ret = sqlite3_exec(db, "PRAGMA journal_mode = WAL", NULL, NULL, &err);
	if (ret != SQLITE_OK) {
		_E("Setting journal mode failed: %s", err);
		sqlite3_free(err);
	}

	__databaseHandle = db;
	return true;
}

void DatabaseThread::close()
{
	sqlite3_close(__databaseHandle);
}

bool DatabaseThread::createTable(unsigned int queryId, const char *tableName, const char *columns, const char *option, IDatabaseListener *listener)
{
	QueryInfo *info = new(std::nothrow) QueryInfo;
	IF_FAIL_RETURN_TAG(info, false, _E, "Memory allocation failed");

	info->query = __composeCreate(tableName, columns, option);
	info->id = queryId;
	info->listener = listener;

	if (!pushEvent(QUERY_CREATE_TABLE, info)) {
		delete info;
		return false;
	}

	return true;
}

bool DatabaseThread::insert(unsigned int queryId, const char *tableName, Json record, IDatabaseListener *listener)
{
	std::string query = __composeInsert(tableName, record);
	IF_FAIL_RETURN(!query.empty(), false);

	QueryInfo *info = new(std::nothrow) QueryInfo;
	IF_FAIL_RETURN_TAG(info, false, _E, "Memory allocation failed");

	info->query = query;
	info->id = queryId;
	info->listener = listener;

	if (!pushEvent(QUERY_INSERT, info)) {
		delete info;
		return false;
	}

	return true;
}

bool DatabaseThread::execute(unsigned int queryId, const char *query, IDatabaseListener *listener)
{
	IF_FAIL_RETURN(query, false);

	QueryInfo *info = new(std::nothrow) QueryInfo;
	IF_FAIL_RETURN_TAG(info, false, _E, "Memory allocation failed");

	info->id = queryId;
	info->query = query;
	info->listener = listener;

	if (!pushEvent(QUERY_EXECUTE, info)) {
		delete info;
		return false;
	}

	return true;
}

bool DatabaseThread::createTableSync(const char *tableName, const char *columns, const char *option)
{
	std::string query = __composeCreate(tableName, columns, option);
	IF_FAIL_RETURN(!query.empty(), false);
	_SD("SQL: %s", query.c_str());

	char *err = NULL;
	int ret;
	{
		ScopeMutex sm(&__execMutex);
		ret = sqlite3_exec(__databaseHandle, query.c_str(), NULL, NULL, &err);
	}

	if (ret != SQLITE_OK) {
		_E("DB Error: %s", err);
		sqlite3_free(err);
		return false;
	}

	return true;
}

bool DatabaseThread::insertSync(const char *tableName, Json record, int64_t *rowId)
{
	IF_FAIL_RETURN(tableName && rowId, false);

	std::string query = __composeInsert(tableName, record);
	IF_FAIL_RETURN(!query.empty(), false);
	_SD("SQL: %s", query.c_str());

	std::vector<Json> queryResult;
	char *err = NULL;
	int ret;
	{
		ScopeMutex sm(&__execMutex);
		ret = sqlite3_exec(__databaseHandle, query.c_str(), __executionCb, &queryResult, &err);
	}

	if (ret != SQLITE_OK) {
		_E("DB Error: %s", err);
		sqlite3_free(err);
		return false;
	}

	IF_FAIL_RETURN_TAG(!queryResult.empty(), false, _E, "No row id");

	*rowId = -1;
	queryResult.at(0).get(NULL, "seq", rowId);
	_D("RowId: %lld", *rowId);

	return true;
}

bool DatabaseThread::executeSync(const char *query, std::vector<Json> *records)
{
	IF_FAIL_RETURN(query && records, false);

	_SD("SQL: %s", query);

	char *err = NULL;
	int ret;
	{
		ScopeMutex sm(&__execMutex);
		ret = sqlite3_exec(__databaseHandle, query, __executionCb, records, &err);
	}

	if (ret != SQLITE_OK) {
		_E("DB Error: %s", err);
		sqlite3_free(err);
		return false;
	}

	return true;
}

void DatabaseThread::onEvent(int type, void* data)
{
	IF_FAIL_VOID(data);
	QueryInfo *info = static_cast<QueryInfo*>(data);

	__execute(static_cast<QueryType>(type), info->id, info->query.c_str(), info->listener);

	deleteEvent(type, data);
}

void DatabaseThread::deleteEvent(int type, void* data)
{
	IF_FAIL_VOID(data);
	QueryInfo *info = static_cast<QueryInfo*>(data);
	delete info;
}

std::string DatabaseThread::__composeCreate(const char *tableName, const char *columns, const char *option)
{
	std::string query;
	query = "CREATE TABLE IF NOT EXISTS ";
	query = query + tableName + " (row_id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT," + columns + ")";
	if (option) {
		query = query + " " + option;
	}
	query += ";";
	return query;
}

std::string DatabaseThread::__composeInsert(const char *tableName, Json record)
{
	std::string cols;
	std::string vals;
	std::list<std::string> keys;

	IF_FAIL_RETURN_TAG(record.getKeys(&keys), "", _E, "Invalid record");

	for (auto it = keys.begin(); it != keys.end(); ++it) {
		std::string s;
		int64_t i;
		if (record.get(NULL, (*it).c_str(), &s)) {
			cols = cols + *it + ",";

			char* buf = sqlite3_mprintf("%Q", s.c_str());
			IF_FAIL_RETURN_TAG(buf, "", _E, "Memory allocation failed");

			vals = vals + buf + ",";
			sqlite3_free(buf);

		} else if (record.get(NULL, (*it).c_str(), &i)) {
			cols = cols + *it + ",";
			vals = vals + std::to_string(i) + ",";
		}
	}

	IF_FAIL_RETURN_TAG(!cols.empty(), "", _E, "Invalid record");

	cols.erase(cols.size() - 1);
	vals.erase(vals.size() - 1);

	std::string query = "INSERT INTO ";
	query = query + tableName + " (" + cols + ") VALUES (" + vals + ");";
	query = query + "SELECT seq FROM sqlite_sequence WHERE name='" + tableName + "';";

	return query;
}

void DatabaseThread::__execute(QueryType type, unsigned int queryId, const char *query, IDatabaseListener *listener)
{
	_SD("SQL(%d): %s", queryId, query);

	std::vector<Json> *queryResult = new(std::nothrow) std::vector<Json>;
	IF_FAIL_VOID_TAG(queryResult, _E, "Memory allocation failed");

	char *err = NULL;
	int ret;

	{
		ScopeMutex sm(&__execMutex);
		ret = sqlite3_exec(__databaseHandle, query, __executionCb, queryResult, &err);
	}

	if (ret != SQLITE_OK) {
		_E("DB Error: %s", err);
		sqlite3_free(err);
		__dispatchResult(type, queryId, listener, ERR_OPERATION_FAILED, queryResult);
		return;
	}

	__dispatchResult(type, queryId, listener, ERR_NONE, queryResult);
	return;
}

void DatabaseThread::__dispatchResult(QueryType type, unsigned int queryId, IDatabaseListener *listener, int error, std::vector<Json> *result)
{
	QueryResult *qr = new(std::nothrow) QueryResult();
	IF_FAIL_VOID_TAG(qr, _E, "Memory allocation failed");

	qr->type = type;
	qr->id = queryId;
	qr->listener = listener;
	qr->error = error;
	qr->result = result;

	g_idle_add(__dispatcher, qr);
}

int DatabaseThread::__executionCb(void *userData, int dim, char **value, char **column)
{
	std::vector<Json> *records = static_cast<std::vector<Json>*>(userData);
	Json row;
	bool columnNull = false;

	for (int i=0; i<dim; ++i) {
		if (!value[i]) {
			columnNull = true;
			_W(RED("Null columns exist"));
			break;
		}

		row.set(NULL, column[i], value[i]);
	}

	if (!columnNull) {
		records->push_back(row);
	}

	return 0;
}

gboolean DatabaseThread::__dispatcher(gpointer data)
{
	QueryResult *qr = static_cast<QueryResult*>(data);

	if (qr->listener) {
		switch (qr->type) {
		case QUERY_CREATE_TABLE:
			qr->listener->onTableCreated(qr->id, qr->error);
			break;
		case QUERY_INSERT:
			{
				int64_t rowId = -1;
				if (qr->error == ERR_NONE && qr->result && !qr->result->empty()) {
					qr->result->at(0).get(NULL, "seq", &rowId);
					_D("RowId: %d", rowId);
				}
				qr->listener->onInserted(qr->id, qr->error, rowId);
			}
			break;
		case QUERY_EXECUTE:
			qr->listener->onExecuted(qr->id, qr->error, *(qr->result));
			break;
		default:
			_W("Unknown query type: %d", qr->type);
		}
	}

	delete qr->result;
	delete qr;
	return FALSE;
}
