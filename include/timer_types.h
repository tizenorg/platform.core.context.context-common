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

#ifndef __CONTEXT_TIMER_TYPES_H__
#define __CONTEXT_TIMER_TYPES_H__

#include <string>

#define MAX_HOUR	24
#define MAX_DAY		7

#define TIMER_TYPES_MON "Mon"
#define TIMER_TYPES_TUE "Tue"
#define TIMER_TYPES_WED "Wed"
#define TIMER_TYPES_THU "Thu"
#define TIMER_TYPES_FRI "Fri"
#define TIMER_TYPES_SAT "Sat"
#define TIMER_TYPES_SUN "Sun"
#define TIMER_TYPES_WEEKDAY "Weekday"
#define TIMER_TYPES_WEEKEND "Weekend"
#define TIMER_TYPES_EVERYDAY "Everyday"

namespace ctx {

	namespace timer_types {

		enum day_of_week_e {
			SUN = 0x01,
			MON = 0x02,
			TUE = 0x04,
			WED = 0x08,
			THU = 0x10,
			FRI = 0x20,
			SAT = 0x40,
			WEEKDAY = MON | TUE | WED | THU | FRI,
			WEEKEND = SAT | SUN,
			EVERYDAY = SUN | MON | TUE | WED | THU | FRI | SAT,
		};

	}
}	/* namespace ctx */

#endif	/* __CONTEXT_TIMER_TYPES_H__ */
