/*
 * Copyright (c) 2016 Samsung Electronics Co., Ltd.
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

#include <ICustomRegister.h>
#include <CustomRegister.h>

using namespace ctx;

ICustomRegister *CustomRegister::__customRegister = NULL;

CustomRegister::CustomRegister()
{
}

CustomRegister::~CustomRegister()
{
}

bool CustomRegister::registerCustomProvider(const char* subject, int operation, Json attribute, Json option, const char* owner)
{
	return __customRegister->registerCustomProvider(subject, operation, attribute, option, owner);
}

bool CustomRegister::unregisterCustomProvider(const char* subject)
{
	return __customRegister->unregisterCustomProvider(subject);
}

void CustomRegister::__setCustomRegister(ICustomRegister *customRegister)
{
	__customRegister = customRegister;
}
