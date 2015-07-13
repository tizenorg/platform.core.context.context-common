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

#ifndef __CONTEXT_DBUS_LISTENER_INTERFACE_H__
#define __CONTEXT_DBUS_LISTENER_INTERFACE_H__

#include <glib.h>

namespace ctx {

	class dbus_listener_iface {
		public:
			virtual ~dbus_listener_iface() {}

			/**
			 * @brief	Called when receiving a signal.
			 * @param[in]	sender	The unique bus name of the sender of the signal.
			 * @param[in]	path	The object path that the signal was emitted on.
			 * @param[in]	iface	The name of the interface.
			 * @param[in]	name	The name of the signal.
			 * @param[in]	param	A GVariant tuple with parameters of the signal.
			 */
			virtual void on_signal_received(const char* sender, const char* path, const char* iface, const char* name, GVariant* param) = 0;
	};

}	/* namespace ctx */

#endif	/* End of __CONTEXT_DBUS_LISTENER_INTERFACE_H__ */
