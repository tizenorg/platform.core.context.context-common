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

#ifndef _CONTEXT_I_CONTEXT_MANAGER_H_
#define _CONTEXT_I_CONTEXT_MANAGER_H_

#include <Types.h>
#include <Json.h>
#include <IContextProvider.h>

namespace ctx {

	enum class OperationType {
		SUBSCRIBE	= 1,
		READ		= 2,
		WRITE		= 4
	};

	class SO_EXPORT IContextManager {
	public:
		virtual ~IContextManager();
		virtual bool registerProvider(const char *subject, const char *privilege, IContextProvider *provider) = 0;
		virtual bool unregisterProvider(const char *subject) = 0;
		virtual bool registerTriggerItem(const char *subject, int operation, ctx::Json attributes, ctx::Json options, const char* owner = NULL) = 0;
		virtual bool unregisterTriggerItem(const char *subject) = 0;
		virtual bool publish(const char *subject, ctx::Json &option, int error, ctx::Json &dataUpdated) = 0;
		virtual bool replyToRead(const char *subject, ctx::Json &option, int error, ctx::Json &dataRead) = 0;
	};	/* class IContextManager */

}	/* namespace ctx */

#endif	/* _CONTEXT_I_CONTEXT_MANAGER_H_ */
