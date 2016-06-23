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

#ifndef _CONTEXT_I_DBUS_CLIENT_LISTENER_H_
#define _CONTEXT_I_DBUS_CLIENT_LISTENER_H_

#ifndef SO_EXPORT
#define SO_EXPORT __attribute__ ((visibility("default")))
#endif

#include <string>
#include <Json.h>

namespace ctx {

	class SO_EXPORT IDBusClientListener {
	public:
		virtual ~IDBusClientListener();
		virtual void onPublish(std::string subject, int reqId, int error, Json event) = 0;
	};

}	/* namespace ctx */

#endif	/* End of _CONTEXT_I_DBUS_CLIENT_LISTENER_H_ */
