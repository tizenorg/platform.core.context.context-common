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

#ifndef __CONTEXT_VASUM_ZONE_UTIL_IFACE_H__
#define __CONTEXT_VASUM_ZONE_UTIL_IFACE_H__

#include <string>
#include <sys/types.h>

namespace ctx {

	class zone_util_iface {
		public:
			virtual ~zone_util_iface() {}
			virtual void* join_by_name(const char* name) = 0;
			virtual void* join_to_zone(void* zone) = 0;
			virtual const char* default_zone() = 0;
	};

	namespace zone_util {
		void set_instance(zone_util_iface* zutil);
	}

}	/* namespace ctx */

#endif	/* End of __CONTEXT_VASUM_ZONE_UTIL_IFACE_H__ */
