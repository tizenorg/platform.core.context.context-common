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

#ifndef _MYPLACE_TYPES_H_
#define _MYPLACE_TYPES_H_

#include <string>
#include <ctime>

namespace ctx {

	enum PlaceCategId {
		PLACE_CATEG_ID_NONE = 0,
		PLACE_CATEG_ID_HOME = 1,
		PLACE_CATEG_ID_WORK = 2,
		PLACE_CATEG_ID_OTHER = 3
	};

	struct Location {
		double latitude;
		double longitude;
		double accuracy; // [m]

		Location(double latitude_ = 0.0, double longitude_ = 0.0, double accuracy_ = -1.0)
			: latitude(latitude_), longitude(longitude_), accuracy(accuracy_) {}

	};	/* struct Location */

	typedef float confidence_t;

	class Place {

	public:
		PlaceCategId categId; // category of a place (work/home/other)
		confidence_t categConfidence; // confidence of the above category - between [0,1]
		std::string name; // for now: "work"/"home"/"other"
		bool locationValid;
		Location location; // makes sense if locationValid == true;
		std::string wifiAps; // WiFi APs MAC addresses separated by ","
		time_t createDate; // The last update time of this place

	};	/* class Place */

	bool operator==(const Place &p1, const Place &p2);

}	/* namespace ctx */

#endif /* End of _MYPLACE_TYPES_H_ */
