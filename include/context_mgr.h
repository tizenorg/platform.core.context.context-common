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

#ifndef __CONTEXT_MANAGER_H__
#define __CONTEXT_MANAGER_H__

namespace ctx {
	/* Forward Declaration */
	class json;
	class context_provider_iface;
	class context_provider_info;

	enum operation_type_e {
		OPS_SUBSCRIBE	= 1,
		OPS_READ		= 2,
		OPS_WRITE		= 4,
	};

	namespace context_manager {
		/*
		 */
		bool register_provider(const char *subject, context_provider_info &provider_info);

		/*
		 */
		bool register_trigger_item(const char *subject, int operation, ctx::json attributes, ctx::json options);

		/*
		 */
		bool publish(const char *subject, ctx::json option, int error, ctx::json data_updated);

		/*
		 */
		bool reply_to_read(const char *subject, ctx::json option, int error, ctx::json data_read);

	}	/* namespace ctx::context_manager */
}	/* namespace ctx */

#endif	/* End of __CONTEXT_MANAGER_H__ */