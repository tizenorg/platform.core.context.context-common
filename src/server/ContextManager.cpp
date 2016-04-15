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
#include <Json.h>
#include <ContextProviderBase.h>
#include <ContextManager.h>
#include <IContextManager.h>

static ctx::IContextManager *__instance = NULL;

VISIBLE void ctx::context_manager::setInstance(IContextManager* mgr)
{
	__instance = mgr;
}

VISIBLE bool ctx::context_manager::registerProvider(const char* subject, ctx::ContextProviderInfo &providerInfo)
{
	IF_FAIL_RETURN_TAG(__instance, false, _E, "Not initialized");
	return __instance->registerProvider(subject, providerInfo);
}

VISIBLE bool ctx::context_manager::unregisterProvider(const char* subject)
{
	IF_FAIL_RETURN_TAG(__instance, false, _E, "Not initialized");
	return __instance->unregisterProvider(subject);
}

VISIBLE bool ctx::context_manager::registerTriggerItem(const char *subject, int operation, ctx::Json attributes, ctx::Json options, const char* owner)
{
	IF_FAIL_RETURN_TAG(__instance, false, _E, "Not initialized");
	return __instance->registerTriggerItem(subject, operation, attributes, options, owner);
}

VISIBLE bool ctx::context_manager::unregisterTriggerItem(const char *subject)
{
	IF_FAIL_RETURN_TAG(__instance, false, _E, "Not initialized");
	return __instance->unregisterTriggerItem(subject);
}

VISIBLE bool ctx::context_manager::publish(const char* subject, ctx::Json option, int error, ctx::Json dataUpdated)
{
	IF_FAIL_RETURN_TAG(__instance, false, _E, "Not initialized");
	return __instance->publish(subject, option, error, dataUpdated);
}

VISIBLE bool ctx::context_manager::replyToRead(const char* subject, ctx::Json option, int error, ctx::Json dataRead)
{
	IF_FAIL_RETURN_TAG(__instance, false, _E, "Not initialized");
	return __instance->replyToRead(subject, option, error, dataRead);
}
