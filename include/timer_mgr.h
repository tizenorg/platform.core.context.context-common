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

#ifndef __CONTEXT_TIMER_MANAGER_H__
#define __CONTEXT_TIMER_MANAGER_H__

#include <timer_types.h>

namespace ctx {
	/* Forward Declaration */
	class timer_listener_iface;

	namespace timer_manager {

		/**
		 * @brief		Sets a repeated timer for a given interval of time (s).
		 * @details		It is recommended to minize the number of timers initiated, to reduce battery consumptions.
		 *				If it is possible to share a timer for multiple purposes, please do that.
		 * @param[in]	interval	Timer interval. The first timer will be expired after @c interval minutes,
		 *							and will be repeated until the listener returns @c false.
		 * @param[in]	listener	A listner object to be notified.
		 * @return		Timer ID. A negative integer if failed to set a timer.
		 */
		int set_for(int interval, timer_listener_iface* listener, void* user_data = NULL);

		/**
		 * @brief		Sets a timer that will be expired at a specific time, at designated days of week.
		 * @details		It is recommended to minize the number of timers initiated, to reduce battery consumptions.
		 *				If it is possible to share a timer for multiple purposes, please do that.
		 * @param[in]	hour		Hour
		 * @param[in]	min			Minute
		 * @param[in]	day_of_week	The timer will expire at hour:min:00, at every day(s) of week designated here.
		 * @param[in]	listener	Listener object.
		 * @return		Timer ID. A negative integer if failed to set a timer.
		 */
		int set_at(int hour, int min, int day_of_week, timer_listener_iface* listener, void* user_data = NULL);

		/**
		 * @brief		Removes the timer specified by @c timer_id.
		 */
		void remove(int timer_id);
	}
}	/* namespace ctx */

#endif	/* __CONTEXT_TIMER_MANAGER_H__ */
