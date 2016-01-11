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

#include <timer_util.h>

using namespace ctx::timer_types;

std::string ctx::timer_util::convert_day_of_week_int_to_string(int d)
{
	switch(d) {
	case SUN:
		return TIMER_TYPES_SUN;
	case MON:
		return TIMER_TYPES_MON;
	case TUE:
		return TIMER_TYPES_TUE;
	case WED:
		return TIMER_TYPES_WED;
	case THU:
		return TIMER_TYPES_THU;
	case FRI:
		return TIMER_TYPES_FRI;
	case SAT:
		return TIMER_TYPES_SAT;
	case WEEKDAY:
		return TIMER_TYPES_WEEKDAY;
	case WEEKEND:
		return TIMER_TYPES_WEEKEND;
	case EVERYDAY:
		return TIMER_TYPES_EVERYDAY;
	default:
		break;
	}

	return "";
}

int ctx::timer_util::convert_day_of_week_string_to_int(std::string d)
{
	int day = 0;

	if (d.compare(TIMER_TYPES_SUN) == 0) {
		day = SUN;
	} else if (d.compare(TIMER_TYPES_MON) == 0) {
		day = MON;
	} else if (d.compare(TIMER_TYPES_TUE) == 0) {
		day = TUE;
	} else if (d.compare(TIMER_TYPES_WED) == 0) {
		day = WED;
	} else if (d.compare(TIMER_TYPES_THU) == 0) {
		day = THU;
	} else if (d.compare(TIMER_TYPES_FRI) == 0) {
		day = FRI;
	} else if (d.compare(TIMER_TYPES_SAT) == 0) {
		day = SAT;
	} else if (d.compare(TIMER_TYPES_WEEKDAY) == 0) {
		day = WEEKDAY;
	} else if (d.compare(TIMER_TYPES_WEEKEND) == 0) {
		day = WEEKEND;
	} else if (d.compare(TIMER_TYPES_EVERYDAY) == 0) {
		day = EVERYDAY;
	}

	return day;
}
