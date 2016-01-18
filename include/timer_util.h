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

#ifndef __CONTEXT_TIMER_UTIL_H__
#define __CONTEXT_TIMER_UTIL_H__

#include <string>
#include <timer_types.h>

namespace ctx {

	namespace timer_util {

		std::string convert_day_of_week_int_to_string(int d);
		int convert_day_of_week_string_to_int(std::string d);

	}
}	/* namespace ctx */

#endif	/* __CONTEXT_TIMER_UTIL_H__ */