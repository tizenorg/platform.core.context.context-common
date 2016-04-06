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

#ifndef _CONTEXT_I_DBUS_SIGNAL_LISTENER_H_
#define _CONTEXT_I_DBUS_SIGNAL_LISTENER_H_

#include <glib.h>

#ifndef CTX_COMMON_EXPORT_API
#define CTX_COMMON_EXPORT_API
#endif

namespace ctx {

	class CTX_COMMON_EXPORT_API IDBusSignalListener {
	public:
		virtual ~IDBusSignalListener() {}

		/**
		 * @brief	Called when receiving a signal.
		 * @param[in]	sender	The unique bus name of the sender of the signal.
		 * @param[in]	path	The object path that the signal was emitted on.
		 * @param[in]	iface	The name of the interface.
		 * @param[in]	name	The name of the signal.
		 * @param[in]	param	A GVariant tuple with parameters of the signal.
		 */
		virtual void onSignal(const char *sender, const char *path, const char *iface, const char *name, GVariant *param) = 0;
	};

}	/* namespace ctx */

#endif	/* End of _CONTEXT_I_DBUS_SIGNAL_LISTENER_H_ */
