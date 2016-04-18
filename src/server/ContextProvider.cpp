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

#include <IContextManager.h>
#include <ContextProvider.h>

using namespace ctx;

IContextManager *ContextProvider::__contextMgr = NULL;

ContextProvider::ContextProvider(const char *subject) :
	__subject(subject)
{
}

ContextProvider::~ContextProvider()
{
}

bool ContextProvider::isSupported()
{
	return true;
}

int ContextProvider::subscribe(Json option, Json *requestResult)
{
	return ERR_NOT_SUPPORTED;
}

int ContextProvider::unsubscribe(Json option)
{
	return ERR_NOT_SUPPORTED;
}

int ContextProvider::read(Json option, Json *requestResult)
{
	return ERR_NOT_SUPPORTED;
}

int ContextProvider::write(Json data, Json *requestResult)
{
	return ERR_NOT_SUPPORTED;
}

bool ContextProvider::registerProvider(const char *privilege, ContextProvider *provider)
{
	return __contextMgr->registerProvider(__subject, privilege, provider);
}

bool ContextProvider::unregisterProvider()
{
	return __contextMgr->unregisterProvider(__subject);
}

bool ContextProvider::registerTriggerItem(int operation, Json attributes, Json options, const char *owner)
{
	return __contextMgr->registerTriggerItem(__subject, operation, attributes, options, owner);
}

bool ContextProvider::unregisterTriggerItem()
{
	return __contextMgr->unregisterTriggerItem(__subject);
}

bool ContextProvider::publish(Json option, int error, Json dataUpdated)
{
	return __contextMgr->publish(__subject, option, error, dataUpdated);
}

bool ContextProvider::replyToRead(Json option, int error, Json dataRead)
{
	return __contextMgr->replyToRead(__subject, option, error, dataRead);
}

void ContextProvider::__setContextManager(IContextManager *contextMgr)
{
	__contextMgr = contextMgr;
}
