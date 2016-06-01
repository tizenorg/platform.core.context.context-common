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

#include <MyPlaceTypes.h>
#include <Types.h>

SO_EXPORT bool ctx::operator==(const ctx::Place &p1, const ctx::Place &p2)
{
	bool ret = p1.categId == p2.categId
			&& p1.categConfidence == p2.categConfidence
			&& p1.name == p2.name
			&& p1.createDate == p2.createDate
			&& p1.locationValid == p2.locationValid;
	if (ret && p1.locationValid) {
		// Check location only if it is valid / filled
		if (p1.location.latitude != p2.location.latitude
				|| p1.location.longitude != p2.location.longitude
				|| p1.location.accuracy != p2.location.accuracy) {
			ret = false;
		}
	}
	if (ret) {
		for (std::pair<std::string, std::string> ap : p1.wifiAps) {
			// Check only MAC addresses because network names are only addition.
			if (p2.wifiAps.find(ap.first) == p2.wifiAps.end()) {
				ret = false;
				break;
			}
		}
	}
	return ret;
}
