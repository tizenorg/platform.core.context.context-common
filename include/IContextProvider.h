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

#ifndef _CONTEXT_I_CONTEXT_PROVIDER_H_
#define _CONTEXT_I_CONTEXT_PROVIDER_H_

#include <Types.h>
#include <Json.h>

namespace ctx {

	class SO_EXPORT IContextProvider {
	public:
		virtual ~IContextProvider();
		virtual int subscribe(const char *subject, ctx::Json option, ctx::Json *requestResult);
		virtual int unsubscribe(const char *subject, ctx::Json option);
		virtual int read(const char *subject, ctx::Json option, ctx::Json *requestResult);
		virtual int write(const char *subject, ctx::Json data, ctx::Json *requestResult);

	protected:
		IContextProvider();

	};	/* class IContextProvider */

}	/* namespace ctx */

#endif	/* _CONTEXT_I_CONTEXT_PROVIDER_H_ */
