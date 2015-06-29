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

#ifndef __CONTEXT_MANAGER_INTERFACE_H__
#define __CONTEXT_MANAGER_INTERFACE_H__

namespace ctx {
	/* Forward Declaration */
	class json;
	class context_provider_iface;

	class context_manager_iface {
		public:
			virtual ~context_manager_iface() {}
			virtual bool register_provider(const char* subject, ctx::context_provider_iface* cp) = 0;
			virtual bool publish(const char* subject, ctx::json& option, int error, ctx::json& data_updated, const char* zone) = 0;
			virtual bool reply_to_read(const char* subject, ctx::json& option, int error, ctx::json& data_read, const char* zone) = 0;
	};	/* class context_manager_iface */

	namespace context_manager {
		void set_instance(context_manager_iface* mgr);
	}	/* namespace ctx::context_manager */

}	/* namespace ctx */

#endif	/* End of __CONTEXT_MANAGER_INTERFACE_H__ */