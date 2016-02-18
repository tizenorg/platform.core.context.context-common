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

#ifndef _CONTEXT_SHARED_VARS_H_
#define _CONTEXT_SHARED_VARS_H_

#include <glib.h>
#include <string>
#include <map>

namespace ctx {

	/*
	 * TODO: Maybe later, it would be possible to extend this to support a sort of
	 *       'update observation' feature, i.e., getting notifications when a variable is updated.
	 */
	class SharedVars {
	public:
		enum VarId {
			WIFI_BSSID,
		};

		SharedVars();
		~SharedVars();

		const std::string& set(VarId id, std::string value) const;
		std::string get(VarId id);
		void clear(VarId id);

	private:
		static GMutex __mutex;
		static std::map<VarId, std::string> __varsMap;
	};

}

#endif
