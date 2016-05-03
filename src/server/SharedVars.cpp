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

#include <Types.h>
#include <ScopeMutex.h>
#include <SharedVars.h>

#define EMPTY_STRING ""

using namespace ctx;

GMutex SharedVars::__mutex;
std::map<SharedVars::VarId, std::string> SharedVars::__varsMap;

SO_EXPORT SharedVars::SharedVars()
{
}

SO_EXPORT SharedVars::~SharedVars()
{
}

SO_EXPORT const std::string& SharedVars::set(SharedVars::VarId id, std::string value) const
{
	ScopeMutex sm(&__mutex);
	_D("var[%d] = %s", id, value.c_str());
	__varsMap[id] = value;
	return __varsMap[id];
}

SO_EXPORT std::string SharedVars::get(SharedVars::VarId id)
{
	ScopeMutex sm(&__mutex);
	auto it = __varsMap.find(id);
	if (it != __varsMap.end())
		return it->second;
	else
		return EMPTY_STRING;
}

SO_EXPORT void SharedVars::clear(SharedVars::VarId id)
{
	ScopeMutex sm(&__mutex);
	_D("Remove var[%d]", id);
	__varsMap.erase(id);
}
