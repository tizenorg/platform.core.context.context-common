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

#ifndef __CONTEXT_DBUS_SERVER_H__
#define __CONTEXT_DBUS_SERVER_H__

#include <sys/types.h>

namespace ctx {
	/* Forward Declaration */
	class dbus_listener_iface;

	namespace dbus_server {
		/**
		 * @brief	Subscribes to signals.
		 * @param[in]	sender		Sensor name to match on. NULL to listen from all senders.
		 * @param[in]	path		Object path to match on. NULL to match on all object paths.
		 * @param[in]	iface		D-Bus interface name to match on. NULL to match on all interfaces.
		 * @param[in]	name		D-Bus signal name to match on. NULL to match on all signals.
		 * @param[in]	listener	Listener object to receive matching signals.
		 * @return	A subscription identifier that can be used with signal_unsubscribe().@n
		 *			If failed, a negative value.
		 */
		int64_t signal_subscribe(const char* sender, const char* path, const char* iface, const char* name, dbus_listener_iface* listener);

		/**
		 * @brief	Unsubscribes from signals.
		 */
		void signal_unsubscribe(int64_t subscription_id);

	}	/* namespace ctx::dbus_server */
}	/* namespace ctx */

#endif	/* End of __CONTEXT_DBUS_SERVER_H__ */
