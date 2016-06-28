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

#ifndef _CONTEXT_ERROR_TYPES_H_
#define _CONTEXT_ERROR_TYPES_H_

#include <tizen_error.h>

/* Internal Error Codes
 * Please define the necessary error codes here.
 * Note that, these error codes must be aligned with the error enums defined in all API headers.
 */
#define ERR_NONE				TIZEN_ERROR_NONE
#define ERR_INVALID_PARAMETER	TIZEN_ERROR_INVALID_PARAMETER
#define ERR_INVALID_OPERATION	TIZEN_ERROR_INVALID_OPERATION
#define ERR_OUT_OF_MEMORY		TIZEN_ERROR_OUT_OF_MEMORY
#define ERR_PERMISSION_DENIED	TIZEN_ERROR_PERMISSION_DENIED
#define ERR_NOT_SUPPORTED		TIZEN_ERROR_NOT_SUPPORTED
#define ERR_NO_DATA				TIZEN_ERROR_NO_DATA
#define ERR_ALREADY_STARTED		(TIZEN_ERROR_CONTEXT | 0x01)
#define ERR_NOT_STARTED			(TIZEN_ERROR_CONTEXT | 0x02)
#define ERR_OUT_OF_RANGE		(TIZEN_ERROR_CONTEXT | 0x03)
#define ERR_OPERATION_FAILED	(TIZEN_ERROR_CONTEXT | 0x04)
#define ERR_RULE_ENABLED		(TIZEN_ERROR_CONTEXT | 0x05)
#define ERR_RULE_NOT_ENABLED	(TIZEN_ERROR_CONTEXT | 0x06)
#define ERR_INVALID_RULE		(TIZEN_ERROR_CONTEXT | 0x07)
#define ERR_RULE_NOT_EXIST		(TIZEN_ERROR_CONTEXT | 0x08)
#define ERR_DATA_EXIST			(TIZEN_ERROR_CONTEXT | 0X09)
#define ERR_INVALID_DATA		(TIZEN_ERROR_CONTEXT | 0X0a)

#endif /* _CONTEXT_ERROR_TYPES_H_ */
