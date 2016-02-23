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
	class Json;

	class context_provider_iface {
	public:
		virtual ~context_provider_iface() {}
		virtual int subscribe(const char *subject, ctx::Json option, ctx::Json *request_result);
		virtual int unsubscribe(const char *subject, ctx::Json option);
		virtual int read(const char *subject, ctx::Json option, ctx::Json *request_result);
		virtual int write(const char *subject, ctx::Json data, ctx::Json *request_result);

	protected:
		context_provider_iface() {}

	};	/* class context_provider_iface */

	class context_provider_info {
		typedef context_provider_iface *(*creator_t)(void *data);
		typedef void (*destroyer_t)(void *data);

	public:
		creator_t create;
		destroyer_t destroy;
		void *data;
		const char *privilege;

		context_provider_info();
		context_provider_info(creator_t cr, destroyer_t des, void *dat = NULL, const char *priv = NULL);
	};

}	/* namespace ctx */

#endif	/* End of __CONTEXT_PROVIDER_INTERFACE_H__ */
