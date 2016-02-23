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
#include <Json.h>
#include <provider_iface.h>

int ctx::context_provider_iface::subscribe(const char *subject, ctx::Json option, ctx::Json *request_result)
{
	return ERR_NOT_SUPPORTED;
}

int ctx::context_provider_iface::unsubscribe(const char *subject, ctx::Json option)
{
	return ERR_NOT_SUPPORTED;
}

int ctx::context_provider_iface::read(const char *subject, ctx::Json option, ctx::Json *request_result)
{
	return ERR_NOT_SUPPORTED;
}

int ctx::context_provider_iface::write(const char *subject, ctx::Json data, ctx::Json *request_result)
{
	return ERR_NOT_SUPPORTED;
}

ctx::context_provider_info::context_provider_info()
	: create(NULL)
	, destroy(NULL)
	, data(NULL)
	, privilege(NULL)
{
}

ctx::context_provider_info::context_provider_info(
		ctx::context_provider_info::creator_t cr,
		ctx::context_provider_info::destroyer_t des, void *dat, const char *priv)
	: create(cr)
	, destroy(des)
	, data(dat)
	, privilege(priv)
{
}
