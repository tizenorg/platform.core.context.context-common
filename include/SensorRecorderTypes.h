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

#ifndef __CONTEXT_SENSOR_RECORDER_TYPES_H__
#define __CONTEXT_SENSOR_RECORDER_TYPES_H__

#include <ProviderTypes.h>

/* Sensor Subjects */
#define SUBJ_SENSOR					"sensor/"
#define SUBJ_SENSOR_HEART_RATE		SUBJ_SENSOR "heart_rate"
#define SUBJ_SENSOR_PEDOMETER		SUBJ_SENSOR "pedometer"
#define SUBJ_SENSOR_SLEEP_MONITOR	SUBJ_SENSOR "sleep_monitor"
#define SUBJ_SENSOR_PRESSURE		SUBJ_SENSOR "pressure"
#define SUBJ_SENSOR_EXERCISE		SUBJ_SENSOR "exercise"

/* Keys */
#define	KEY_SUBJECT		"Subject"
#define	KEY_OPERATION	"Operation"
#define KEY_OPTION		"Option"

#define	KEY_RETENTION	"Retention"
#define KEY_INTERVAL	"Interval"
#define KEY_ANCHOR		"Anchor"

#define KEY_STEPS		"Steps"
#define KEY_WALK_STEPS	"WalkSteps"
#define KEY_RUN_STEPS	"RunSteps"
#define KEY_DISTANCE	"Dist"
#define KEY_CALORIES	"Cal"
#define KEY_PRESSURE	"Pressure"
#define KEY_AVG_PRESSURE	"AvgPressure"
#define KEY_MIN_PRESSURE	"MinPressure"
#define KEY_MAX_PRESSURE	"MaxPressure"
#define KEY_SLEEP_STATE		KEY_STATE
#define KEY_HEART_RATE		"HeartRate"

/* Values */
#define VAL_START		"Start"
#define VAL_STOP		"Stop"

#endif	/* __CONTEXT_SENSOR_RECORDER_TYPES_H__ */
