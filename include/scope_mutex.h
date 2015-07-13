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

#ifndef __CONTEXT_SCOPE_MUTEX_H__
#define __CONTEXT_SCOPE_MUTEX_H__

#include <glib.h>
#include <types_internal.h>

namespace ctx {
	// RAII Class implementing the mutex helper on the base of GLib mutex,
	// which automatically locks mutex during its creation and unlocks while exiting the scope.
	class scope_mutex
	{
		private:
			GMutex *mutex;

		public:
			scope_mutex(GMutex *m);
			~scope_mutex();
	};
}	/* namespace ctx */

#endif // __CONTEXT_SCOPE_MUTEX_H__
