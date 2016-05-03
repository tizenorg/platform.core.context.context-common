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

#ifndef _CONTEXT_CONTEXT_PROVIDER_H_
#define _CONTEXT_CONTEXT_PROVIDER_H_

#include <vector>
#include <Types.h>
#include <Json.h>

#define OPS_SUBSCRIBE	1
#define OPS_READ		2
#define OPS_WRITE		4

namespace ctx {

	class IContextManager;

	class SO_EXPORT ContextProvider {
	public:
		virtual ~ContextProvider();

		const char* getSubject();

		virtual bool isSupported();
		virtual int subscribe(Json option, Json *requestResult);
		virtual int unsubscribe(Json option);
		virtual int read(Json option, Json *requestResult);
		virtual int write(Json data, Json *requestResult);

		virtual void getPrivilege(std::vector<const char*> &privilege);

		virtual bool unloadable();

		bool publish(Json option, int error, Json dataUpdated);
		bool replyToRead(Json option, int error, Json dataRead);

	protected:
		ContextProvider(const char *subject);

	private:
		static void __setContextManager(IContextManager *contextMgr);

		static IContextManager *__contextMgr;

		const char *__subject;

		friend class ContextManager;

	};	/* class ContextProvider */

}	/* namespace ctx */

#endif	/* _CONTEXT_CONTEXT_PROVIDER_H_ */
