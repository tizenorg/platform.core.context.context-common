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

#ifndef __CONTEXT_LIB_DBUS_HANDLER_H__
#define __CONTEXT_LIB_DBUS_HANDLER_H__

#include <string>

namespace ctx {

	class dbus_client {
		public:
			dbus_client();
			~dbus_client();

			bool init();
			void release();

			int request(int type, int req_id, const char* subject, const char* input, std::string* req_result, std::string* data_read);
			int request_with_no_reply(int type, int req_id, const char* subject, const char* input);

	};	/* classs ctx::dbus_client */

}	/* namespace ctx */

#endif // __CONTEXT_LIB_DBUS_HANDLER_H__