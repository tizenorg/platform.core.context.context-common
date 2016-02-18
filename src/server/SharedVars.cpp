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

#include <types_internal.h>
#include <ScopeMutex.h>
#include <SharedVars.h>

using namespace ctx;

GMutex SharedVars::__mutex;
std::map<SharedVars::VarName, std::string> SharedVars::__varsMap;

SharedVars::SharedVars()
{
}

SharedVars::~SharedVars()
{
}

const std::string& SharedVars::set(SharedVars::VarName name, std::string value) const
{
	ScopeMutex sm(&__mutex);
	__varsMap[name] = value;
	return __varsMap[name];
}

std::string SharedVars::get(SharedVars::VarName name)
{
	ScopeMutex sm(&__mutex);
	auto it = __varsMap.find(name);
	if (it != __varsMap.end())
		return it->second;
	else
		return EMPTY_STRING;
}

void SharedVars::clear(SharedVars::VarName name)
{
	ScopeMutex sm(&__mutex);
	__varsMap.erase(name);
}
