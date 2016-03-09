/*
 * Copyright (c) 2016 Samsung Electronics Co., Ltd.
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

#ifndef _CONTEXT_TIMER_H_
#define _CONTEXT_TIMER_H_

#include <glib.h>
#include <string>
#include <set>
#include <map>
#include <ITimerListener.h>

#define DAYS_PER_WEEK	7
#define DOW_MON		"Mon"
#define DOW_TUE		"Tue"
#define DOW_WED		"Wed"
#define DOW_THU		"Thu"
#define DOW_FRI		"Fri"
#define DOW_SAT		"Sat"
#define DOW_SUN		"Sun"
#define DOW_WEEKDAY	"Weekday"
#define DOW_WEEKEND	"Weekend"
#define DOW_EVERYDAY	"Everyday"

namespace ctx {

	enum class DayOfWeek {
		SUN = 0x01,
		MON = 0x02,
		TUE = 0x04,
		WED = 0x08,
		THU = 0x10,
		FRI = 0x20,
		SAT = 0x40,
		WEEKDAY = MON | TUE | WED | THU | FRI,
		WEEKEND = SAT | SUN,
		EVERYDAY = SUN | MON | TUE | WED | THU | FRI | SAT
	};

	class TimerManager {
	public:
		TimerManager();
		~TimerManager();

		static std::string dowToStr(int dow);
		static int dowToInt(std::string dow);

		/**
		 * @brief		Sets a repeated timer for a given interval of time (s).
		 * @details		It is recommended to minize the number of timers initiated, to reduce battery consumptions.
		 *				If it is possible to share a timer for multiple purposes, please do that.
		 * @param[in]	interval	Timer interval. The first timer will be expired after @c interval minutes,
		 *							and will be repeated until the listener returns @c false.
		 * @param[in]	listener	A listner object to be notified.
		 * @return		Timer ID. A negative integer if failed to set a timer.
		 */
		int setFor(int interval, ITimerListener *listener);

		/**
		 * @brief		Sets a timer that will be expired at a specific time, at designated days of week.
		 * @details		It is recommended to minize the number of timers initiated, to reduce battery consumptions.
		 *				If it is possible to share a timer for multiple purposes, please do that.
		 * @param[in]	hour		Hour
		 * @param[in]	min			Minute
		 * @param[in]	dow			The timer will expire at hour:min:00, at every day(s) of week designated here.
		 * @param[in]	listener	Listener object.
		 * @return		Timer ID. A negative integer if failed to set a timer.
		 */
		int setAt(int hour, int min, DayOfWeek dow, ITimerListener *listener);

		/**
		 * @brief		Removes the timer specified by @c timerId.
		 */
		void remove(int timerId);

	private:
		static int __onAlarmExpired(int alarmId, void *userdata);

		bool __init();
		void __release();

		static unsigned int __instanceCnt;
		static GMutex __mutex;
		static std::map<int, std::pair<ITimerListener*, TimerManager*>> __listenerMap;

		std::set<int> __alarms;
	};

}	/* namespace ctx */

#endif	/* _CONTEXT_TIMER_H_ */
