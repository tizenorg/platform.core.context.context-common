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

#ifndef __CONTEXT_TYPES_INTERNAL_H__
#define __CONTEXT_TYPES_INTERNAL_H__

#include <tizen_error.h>
#include <dlog.h>

#define EXTAPI __attribute__ ((visibility("default")))
#define EMPTY_STRING	""

/* Internal Error Codes
 * Please define the necessary error codes here.
 * Note that, these error codes must be aligned with the error enums defined in all API headers.
 */
#define ERR_NONE				TIZEN_ERROR_NONE
#define ERR_INVALID_PARAMETER	TIZEN_ERROR_INVALID_PARAMETER
#define ERR_INVALID_OPERATION	TIZEN_ERROR_INVALID_OPERATION
#define ERR_OUT_OF_MEMORY		TIZEN_ERROR_OUT_OF_MEMORY
#define ERR_PERMISSION_DENIED	TIZEN_ERROR_PERMISSION_DENIED
#define ERR_NOT_SUPPORTED		TIZEN_ERROR_NOT_SUPPORTED
#define ERR_NO_DATA				TIZEN_ERROR_NO_DATA
#define ERR_ALREADY_STARTED		(TIZEN_ERROR_CONTEXT | 0x01)
#define ERR_NOT_STARTED			(TIZEN_ERROR_CONTEXT | 0x02)
#define ERR_OUT_OF_RANGE		(TIZEN_ERROR_CONTEXT | 0x03)
#define ERR_OPERATION_FAILED	(TIZEN_ERROR_CONTEXT | 0x04)
#define ERR_RULE_ENABLED		(TIZEN_ERROR_CONTEXT | 0x05)
#define ERR_RULE_NOT_ENABLED	(TIZEN_ERROR_CONTEXT | 0x06)
#define ERR_INVALID_RULE		(TIZEN_ERROR_CONTEXT | 0x07)
#define ERR_RULE_NOT_EXIST		(TIZEN_ERROR_CONTEXT | 0x08)

/* Logging and Error Handling */
#define _I SLOGI
#define _D SLOGD
#define _W SLOGW
#define _E SLOGE
#define _SI SECURE_SLOGI
#define _SD SECURE_SLOGD
#define _SW SECURE_LOGW
#define _SE SECURE_SLOGE

/* Color code for dlog */
#define RED(X)		"\033[0;31m" X "\033[0m"
#define GREEN(X)	"\033[0;32m" X "\033[0m"
#define YELLOW(X)	"\033[0;33m" X "\033[0m"
#define BLUE(X)		"\033[0;34m" X "\033[0m"
#define PURPLE(X)	"\033[0;35m" X "\033[0m"
#define CYAN(X)		"\033[0;36m" X "\033[0m"


#define STR_EQ(X, Y) (g_strcmp0((X), (Y)) == 0)

#define IF_FAIL_RETURN_TAG(cond, ret, tag, fmt, arg...) \
	do { if (!(cond)) { tag(fmt, ##arg); return ret; } } while (0)

#define IF_FAIL_RETURN(cond, ret) \
	do { if (!(cond)) { return ret; } } while (0)

#define IF_FAIL_VOID_TAG(cond, tag, fmt, arg...) \
	do { if (!(cond)) { tag(fmt, ##arg); return; } } while (0)

#define IF_FAIL_VOID(cond) \
	do { if (!(cond)) { return; } } while (0)

#define IF_FAIL_CATCH_TAG(cond, tag, fmt, arg...) \
	do { if (!(cond)) { tag(fmt, ##arg); goto CATCH; } } while (0)

#define IF_FAIL_CATCH(cond) \
	do { if (!(cond)) { goto CATCH; } } while (0)

#define IS_FAILED(X) ((X) != ERR_NONE)

#define ASSERT_ALLOC(X)		IF_FAIL_RETURN_TAG(X, ERR_OUT_OF_MEMORY, _E, "Memory allocation failed")
#define ASSERT_NOT_NULL(X)	IF_FAIL_RETURN_TAG(X, ERR_INVALID_PARAMETER, _E, "Parameter null")

#define HANDLE_GERROR(Err) \
	do { if ((Err)) { _E("GError: %s", Err->message); g_error_free(Err); Err = NULL; } } while (0)


/* DBus */
#define DBUS_DEST		"org.tizen.context"
#define DBUS_PATH		"/org/tizen/context"
#define DBUS_IFACE		"org.tizen.context"
#define DBUS_TIMEOUT	3000000

#define METHOD_REQUEST	"Request"
#define METHOD_RESPOND	"Respond"
#define SIGNAL_LAUNCHED	"Launched"

enum request_type {
	REQ_SUBSCRIBE = 1,
	REQ_UNSUBSCRIBE,
	REQ_READ,
	REQ_READ_SYNC,
	REQ_WRITE,
	REQ_SUPPORT,
};

#define ARG_REQTYPE		"type"
#define ARG_COOKIE		"cookie"
#define ARG_REQID		"req_id"
#define ARG_SUBJECT		"subject"
#define ARG_INPUT		"input"

#define ARG_RESULT_ERR	"r_err"
#define ARG_RESULT_ADD	"r_add"
#define ARG_OUTPUT		"output"


#endif /* __CONTEXT_TYPES_INTERNAL_H__ */
