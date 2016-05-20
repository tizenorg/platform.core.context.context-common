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

#ifndef _CONTEXT_DBUS_TYPES_H_
#define _CONTEXT_DBUS_TYPES_H_

#define DBUS_DEST		"org.tizen.context"
#define DBUS_PATH		"/org/tizen/context"
#define DBUS_IFACE		"org.tizen.context"
#define DBUS_TIMEOUT	3000000

#define METHOD_REQUEST	"Request"
#define METHOD_RESPOND	"Respond"
#define SIGNAL_LAUNCHED	"Launched"

#define METHOD_CHK_PRIV	"ChkPriv"
#define METHOD_CHK_PRIV_APPLAUNCH	METHOD_CHK_PRIV "AppLaunch"
#define METHOD_CHK_PRIV_CALL		METHOD_CHK_PRIV "Call"
#define METHOD_CHK_PRIV_NOTIFICATION	METHOD_CHK_PRIV "Notification"

#define ARG_REQTYPE		"type"
#define ARG_COOKIE		"cookie"
#define ARG_REQID		"req_id"
#define ARG_SUBJECT		"subject"
#define ARG_INPUT		"input"

#define ARG_RESULT_ERR	"r_err"
#define ARG_RESULT_ADD	"r_add"
#define ARG_OUTPUT		"output"

enum RequestType {
	REQ_SUBSCRIBE = 1,
	REQ_UNSUBSCRIBE,
	REQ_READ,
	REQ_READ_SYNC,
	REQ_WRITE,
	REQ_SUPPORT,
};

#endif	/* _CONTEXT_DBUS_TYPES_H_ */
