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

#ifndef __CONTEXT_PROVIDER_INTERFACE_H__
#define __CONTEXT_PROVIDER_INTERFACE_H__

namespace ctx {

	/* Forward Declaration */
	class json;

	class context_provider_iface {
		public:
			virtual ~context_provider_iface() {}

			virtual bool init() = 0;

			virtual bool is_supported(const char* subject) = 0;

			virtual int subscribe(const char* subject, ctx::json option, ctx::json* request_result) = 0;

			virtual int unsubscribe(const char* subject, ctx::json option) = 0;

			virtual int read(const char* subject, ctx::json option, ctx::json* request_result) = 0;

			virtual int write(const char* subject, ctx::json data, ctx::json* request_result) = 0;

	};	/* class context_provider_iface */

}	/* namespace ctx */

#endif	/* End of __CONTEXT_PROVIDER_INTERFACE_H__ */
