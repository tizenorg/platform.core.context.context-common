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

#ifndef __CONTEXT_TIMER_MANAGER_INTERFACE_H__
#define __CONTEXT_TIMER_MANAGER_INTERFACE_H__

namespace ctx {
	/* Forward Declaration */
	class timer_listener_iface;

	class timer_manager_iface {
		public:
			virtual ~timer_manager_iface() {}

			/**
			 * @brief	Sets a timer notifying at every @c interval minutes.
			 * @param[in]	interval	Repeating interval. Seconds.
			 * @param[in]	listener	Listener object.
			 * @return	A timer identifier that can be used to distinguish timers.
			 *			The same ID will be delivered whenever the timer is expired.
			 */
			virtual int set_for(int interval, timer_listener_iface* listener, void* user_data) = 0;

			/**
			 * @brief	Sets a timer notifying at specific time at every days of week designated.
			 * @param[in]	hour		Hour
			 * @param[in]	min			Minute
			 * @param[in]	day_of_week	The timer will expire at hour:min:00, at every day(s) of week designated here.
			 * @param[in]	listener	Listener object.
			 * @return	A timer identifier that can be used to distinguish timers.
			 *			The same ID will be delivered whenever the timer is expired.
			 */
			virtual int set_at(int hour, int min, int day_of_week, timer_listener_iface* listener, void* user_data) = 0;

			virtual void remove(int timer_id) = 0;
	};

	namespace timer_manager {
		void set_instance(timer_manager_iface* mgr);
	}
}	/* namespace ctx */

#endif	/* __CONTEXT_TIMER_MANAGER_INTERFACE_H__ */
