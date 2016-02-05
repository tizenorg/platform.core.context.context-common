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

#ifndef _CONTEXT_DBUS_SIGNAL_WATCHER_H_
#define _CONTEXT_DBUS_SIGNAL_WATCHER_H_

#include <sys/types.h>
#include <gio/gio.h>
#include <IDBusSignalListener.h>

namespace ctx {

	enum class DBusType {
		SYSTEM,
		SESSION
	};

	class DBusSignalWatcher {
	public:
		DBusSignalWatcher(DBusType type);
		~DBusSignalWatcher();

		/**
		 * @brief	Subscribes to signals.
		 * @param[in]	sender		Sensor name to match on. NULL to listen from all senders.
		 * @param[in]	path		Object path to match on. NULL to match on all object paths.
		 * @param[in]	iface		D-Bus interface name to match on. NULL to match on all interfaces.
		 * @param[in]	name		D-Bus signal name to match on. NULL to match on all signals.
		 * @param[in]	listener	Listener object to receive matching signals.
		 * @return	A subscription identifier that can be used with signal_unsubscribe().
		 *			If failed, a negative value.
		 */
		int64_t watch(const char *sender, const char *path, const char *iface, const char *name, IDBusSignalListener *listener);
		void unwatch(int64_t signal_id);

	private:
		void __openBus(GBusType type, GDBusConnection *&bus);
		void __closeBus(GDBusConnection *&bus);

		static unsigned int __systemBusCnt;
		static unsigned int __sessionBusCnt;
		static GDBusConnection *__systemBus;
		static GDBusConnection *__sessionBus;

		DBusType __busType;
	};

}	/* namespace ctx */

#endif	/* End of _CONTEXT_DBUS_SIGNAL_WATCHER_H_ */
