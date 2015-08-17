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

#include <new>
#include <glib.h>
#include <map>
#include <string>

#include <types_internal.h>
#include <scope_mutex.h>
#include <request_handler.h>
#include "response_handler.h"
#include "dbus_client.h"

typedef std::map<std::string, ctx::request_handler::subject_response_cb> response_cb_map_t;

static ctx::dbus_client *dbus_handle = NULL;
static response_cb_map_t *response_cb_map = NULL;

namespace {
	class exit_handler {
		public:
			exit_handler() {}
			~exit_handler() { delete dbus_handle; }
	};
}	/* namespace ctx */

static exit_handler _exit_handler;

static int generate_req_id()
{
	static GMutex rid_mutex;
	static int req_id = 0;

	ctx::scope_mutex sm(&rid_mutex);

	// Overflow handling
	if (++req_id < 0) {
		req_id = 1;
	}

	return req_id;
}

static bool initialize()
{
	static GMutex init_mutex;
	ctx::scope_mutex sm(&init_mutex);

	if (dbus_handle) {
		return true;
	}

	response_cb_map = new(std::nothrow) response_cb_map_t;
	IF_FAIL_CATCH_TAG(response_cb_map, _E, "Memory allocation failed");

	dbus_handle = new(std::nothrow) ctx::dbus_client();
	IF_FAIL_CATCH_TAG(dbus_handle, _E, "Memory allocation failed");
	IF_FAIL_CATCH_TAG(dbus_handle->init(), _E, "Dbus initialization failed");

	return true;

CATCH:
	delete dbus_handle;
	delete response_cb_map;
	dbus_handle = NULL;
	response_cb_map = NULL;
	return false;
}

int ctx::request_handler::subscribe(const char* subject, ctx::json* option, int* req_id, ctx::json* request_result)
{
	ASSERT_NOT_NULL(subject);
	ASSERT_NOT_NULL(req_id);
	IF_FAIL_RETURN_TAG(initialize(), ERR_OPERATION_FAILED, _E, "Connection failed");

	*req_id = generate_req_id();

	char *opt_str = NULL;
	if (option) {
		opt_str = option->dup_cstr();
		IF_FAIL_RETURN_TAG(opt_str, ERR_OPERATION_FAILED, _E, "Json string creation failed");
	}

	_I("[Subscribe] ReqId: %d, Subject: %s", *req_id, subject);

	std::string result_str;
	int error = dbus_handle->request(REQ_SUBSCRIBE, *req_id, subject, opt_str, &result_str, NULL);
	g_free(opt_str);

	if (request_result) {
		*request_result = result_str;
	}

	_D("Error: %#x", error);
	_SD("Result: %s", result_str.c_str());

	return error;
}

int ctx::request_handler::unsubscribe(const char* subject, int req_id)
{
	ASSERT_NOT_NULL(subject);
	IF_FAIL_RETURN_TAG(initialize(), false, _E, "Connection failed");
	_I("[Unsubscribe] ReqId: %d, Subject: %s", req_id, subject);
	return dbus_handle->request(REQ_UNSUBSCRIBE, req_id, subject, NULL, NULL, NULL);
}

int ctx::request_handler::read(const char* subject, ctx::json* option, int* req_id, ctx::json* request_result)
{
	ASSERT_NOT_NULL(subject);
	ASSERT_NOT_NULL(req_id);
	IF_FAIL_RETURN_TAG(initialize(), false, _E, "Connection failed");

	*req_id = generate_req_id();

	char *opt_str = NULL;
	if (option) {
		opt_str = option->dup_cstr();
		IF_FAIL_RETURN_TAG(opt_str, ERR_OPERATION_FAILED, _E, "Json string creation failed");
	}

	_I("[Read] ReqId: %d, Subject: %s", *req_id, subject);

	std::string result_str;
	int error = dbus_handle->request(REQ_READ, *req_id, subject, opt_str, &result_str, NULL);
	g_free(opt_str);

	if (request_result) {
		*request_result = result_str;
	}

	_D("Error: %#x", error);
	_SD("Result: %s", result_str.c_str());

	return error;
}

int ctx::request_handler::read_sync(const char* subject, ctx::json* option, int* req_id, ctx::json* data_read)
{
	ASSERT_NOT_NULL(subject);
	ASSERT_NOT_NULL(req_id);
	ASSERT_NOT_NULL(data_read);
	IF_FAIL_RETURN_TAG(initialize(), false, _E, "Connection failed");

	*req_id = generate_req_id();

	char *opt_str = NULL;
	if (option) {
		opt_str = option->dup_cstr();
		IF_FAIL_RETURN_TAG(opt_str, ERR_OPERATION_FAILED, _E, "Json string creation failed");
	}

	_I("[ReadSync] ReqId: %d, Subject: %s", *req_id, subject);

	std::string data_str;
	int error = dbus_handle->request(REQ_READ_SYNC, *req_id, subject, opt_str, NULL, &data_str);
	g_free(opt_str);

	*data_read = data_str;

	_D("Error: %#x", error);
	_SD("Data: %s", data_str.c_str());

	return error;
}

int ctx::request_handler::write(const char* subject, ctx::json* data)
{
	ASSERT_NOT_NULL(subject);
	ASSERT_NOT_NULL(data);
	IF_FAIL_RETURN_TAG(initialize(), false, _E, "Connection failed");

	int req_id = generate_req_id();
	char *data_str = data->dup_cstr();
	IF_FAIL_RETURN_TAG(data_str, ERR_OPERATION_FAILED, _E, "Json string creation failed");

	_I("[Write] ReqId: %d, Subject: %s", req_id, subject);
	_SD("Data: %s", data_str);

	int error = dbus_handle->request_with_no_reply(REQ_WRITE, req_id, subject, data_str);
	g_free(data_str);

	_D("Error: %#x", error);

	return error;
}

int ctx::request_handler::write_with_reply(const char* subject, ctx::json* data, ctx::json* request_result)
{
	ASSERT_NOT_NULL(subject);
	ASSERT_NOT_NULL(data);
	IF_FAIL_RETURN_TAG(initialize(), false, _E, "Connection failed");

	int req_id = generate_req_id();
	char *data_str = data->dup_cstr();
	IF_FAIL_RETURN_TAG(data_str, ERR_OPERATION_FAILED, _E, "Json string creation failed");

	_I("[Write with reply] ReqId: %d, Subject: %s", req_id, subject);
	_SD("Data: %s", data_str);

	std::string result_str;
	int error = dbus_handle->request(REQ_WRITE, req_id, subject, data_str, &result_str, NULL);
	g_free(data_str);

	if (request_result) {
		*request_result = result_str;
	}

	_D("Error: %#x", error);
	_SD("Result: %s", result_str.c_str());

	return error;
}

int ctx::request_handler::is_supported(const char* subject)
{
	ASSERT_NOT_NULL(subject);
	IF_FAIL_RETURN_TAG(initialize(), false, _E, "Connection failed");
	return dbus_handle->request(REQ_SUPPORT, generate_req_id(), subject, NULL, NULL, NULL);
}

bool ctx::request_handler::register_callback(const char* subject, subject_response_cb callback)
{
	IF_FAIL_RETURN_TAG(subject && callback, false, _E, "Invalid parameter");
	IF_FAIL_RETURN_TAG(initialize(), false, _E, "Connection failed");

	_D("Registering callback for subject '%s'", subject);

	static GMutex cb_list_mutex;
	ctx::scope_mutex sm(&cb_list_mutex);

	(*response_cb_map)[subject] = callback;

	return true;
}

void ctx::response_handler::deliver(const char* subject, int req_id, int error, const char* data)
{
	response_cb_map_t::iterator it = response_cb_map->find(subject);
	IF_FAIL_VOID_TAG(it != response_cb_map->end(), _E, "Unknown subject '%s'", subject);

	it->second(subject, req_id, error, data);
}
