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

#ifndef _CONTEXT_I_TIMER_LISTENER_H_
#define _CONTEXT_I_TIMER_LISTENER_H_

#include <types_internal.h>

namespace ctx {

	class VISIBLE ITimerListener {
	public:
		virtual ~ITimerListener();

		/**
		 * @brief		Called when a timer is expired.
		 * @param[in]	timer_id	The expired timers' ID
		 * @return		@c true, if the timer needs to be repeated.@n
		 *				@c false, if the timer does not need to be repeated anymore.
		 */
		virtual bool onTimerExpired(int timerId) = 0;
	};

}	/* namespace ctx */

#endif	/* _CONTEXT_I_TIMER_LISTENER_H_ */
