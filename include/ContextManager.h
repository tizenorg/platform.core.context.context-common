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

#ifndef _CONTEXT_MANAGER_H_
#define _CONTEXT_MANAGER_H_

#include <stdlib.h>

namespace ctx {
	/* Forward Declaration */
	class Json;
	class ContextProviderBase;
	class ContextProviderInfo;

	enum OperationType {
		OPS_SUBSCRIBE	= 1,
		OPS_READ		= 2,
		OPS_WRITE		= 4,
	};

	namespace context_manager {
		/*
		 */
		bool registerProvider(const char *subject, ContextProviderInfo &providerInfo);

		/*
		 */
		bool unregisterProvider(const char *subject);

		/*
		 */
		bool registerTriggerItem(const char *subject, int operation, ctx::Json attributes, ctx::Json options, const char* owner = NULL);

		/*
		 */
		bool unregisterTriggerItem(const char *subject);

		/*
		 */
		bool publish(const char *subject, ctx::Json option, int error, ctx::Json dataUpdated);

		/*
		 */
		bool replyToRead(const char *subject, ctx::Json option, int error, ctx::Json dataRead);

	}	/* namespace context_manager */
}	/* namespace ctx */

#endif	/* End of _CONTEXT_MANAGER_H_ */
