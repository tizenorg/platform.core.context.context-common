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

#include <cmath>
#include <map>
#include <alarm.h>
#include <types_internal.h>
#include <ScopeMutex.h>
#include <TimerManager.h>

#define IDENTIFIER "contextd"

using namespace ctx;

unsigned int TimerManager::__instanceCnt = 0;
GMutex TimerManager::__mutex;
std::map<int, std::pair<ITimerListener*, TimerManager*>> TimerManager::__listenerMap;

VISIBLE TimerManager::TimerManager()
{
	ScopeMutex sm(&__mutex);

	if (__instanceCnt++ == 0)
		__init();
}

VISIBLE TimerManager::~TimerManager()
{
	ScopeMutex sm(&__mutex);

	for (int i : __alarms) {
		remove(i);
	}

	if (--__instanceCnt == 0)
		__release();
}

VISIBLE std::string TimerManager::dowToStr(int dow)
{
	static const char *dowStr[] = {
		DOW_SUN, DOW_MON, DOW_TUE, DOW_WED, DOW_THU, DOW_FRI, DOW_SAT
	};

	if (static_cast<DayOfWeek>(dow) == DayOfWeek::WEEKDAY)
		return DOW_WEEKDAY;

	if (static_cast<DayOfWeek>(dow) == DayOfWeek::WEEKEND)
		return DOW_WEEKEND;

	if (static_cast<DayOfWeek>(dow) == DayOfWeek::EVERYDAY)
		return DOW_EVERYDAY;

	int d = log2(static_cast<double>(dow));
	if (d >= 0 && d < DAYS_PER_WEEK)
		return dowStr[d];

	return EMPTY_STRING;
}

VISIBLE int TimerManager::dowToInt(std::string dow)
{
	if (dow == DOW_SUN)			return static_cast<int>(DayOfWeek::SUN);
	if (dow == DOW_MON)			return static_cast<int>(DayOfWeek::MON);
	if (dow == DOW_TUE)			return static_cast<int>(DayOfWeek::TUE);
	if (dow == DOW_WED)			return static_cast<int>(DayOfWeek::WED);
	if (dow == DOW_THU)			return static_cast<int>(DayOfWeek::THU);
	if (dow == DOW_FRI)			return static_cast<int>(DayOfWeek::FRI);
	if (dow == DOW_SAT)			return static_cast<int>(DayOfWeek::SAT);
	if (dow == DOW_WEEKDAY)		return static_cast<int>(DayOfWeek::WEEKDAY);
	if (dow == DOW_WEEKEND)		return static_cast<int>(DayOfWeek::WEEKEND);
	if (dow == DOW_EVERYDAY)	return static_cast<int>(DayOfWeek::EVERYDAY);
	return 0;
}

VISIBLE int TimerManager::setFor(int interval, ITimerListener *listener)
{
	IF_FAIL_RETURN_TAG(interval > 0 && listener, -1, _E, "Invalid parameter");

	int alarmId;
	int result;

	result = alarmmgr_add_periodic_alarm_withcb(interval, QUANTUMIZE, __onAlarmExpired, NULL, &alarmId);
	IF_FAIL_RETURN_TAG(result == ALARMMGR_RESULT_SUCCESS, -1, _E, "Alarm setting failed");

	ScopeMutex sm(&__mutex);
	__listenerMap[alarmId] = std::make_pair(listener, this);
	__alarms.insert(alarmId);

	_D("Timer %d was set for %dm interval", alarmId, interval);
	return alarmId;
}

VISIBLE int TimerManager::setAt(int hour, int min, DayOfWeek dow, ITimerListener *listener)
{
	IF_FAIL_RETURN_TAG(
			hour < 24 && hour >= 0 &&
			min < 60 && min >= 0 &&
			static_cast<int>(dow) > 0 &&
			static_cast<int>(dow) <= static_cast<int>(DayOfWeek::EVERYDAY) &&
			listener, -1, _E, "Invalid parameter");

	int repeat = 0;
	if (static_cast<int>(dow) & static_cast<int>(DayOfWeek::SUN))
		repeat |= ALARM_WDAY_SUNDAY;
	if (static_cast<int>(dow) & static_cast<int>(DayOfWeek::MON))
		repeat |= ALARM_WDAY_MONDAY;
	if (static_cast<int>(dow) & static_cast<int>(DayOfWeek::TUE))
		repeat |= ALARM_WDAY_TUESDAY;
	if (static_cast<int>(dow) & static_cast<int>(DayOfWeek::WED))
		repeat |= ALARM_WDAY_WEDNESDAY;
	if (static_cast<int>(dow) & static_cast<int>(DayOfWeek::THU))
		repeat |= ALARM_WDAY_THURSDAY;
	if (static_cast<int>(dow) & static_cast<int>(DayOfWeek::FRI))
		repeat |= ALARM_WDAY_FRIDAY;
	if (static_cast<int>(dow) & static_cast<int>(DayOfWeek::SAT))
		repeat |= ALARM_WDAY_SATURDAY;

	alarm_entry_t *alarmInfo = alarmmgr_create_alarm();
	IF_FAIL_RETURN_TAG(alarmInfo, -1, _E, "Memory allocation failed");

	time_t currentTime;
	struct tm currentTm;
	time(&currentTime);
	tzset();
	localtime_r(&currentTime, &currentTm);

	alarm_date_t alarmTime;
	alarmTime.year = currentTm.tm_year + 1900;
	alarmTime.month = currentTm.tm_mon + 1;
	alarmTime.day = currentTm.tm_mday;
	alarmTime.hour = hour;
	alarmTime.min = min;
	alarmTime.sec = 0;

	alarmmgr_set_time(alarmInfo, alarmTime);
	alarmmgr_set_repeat_mode(alarmInfo, ALARM_REPEAT_MODE_WEEKLY, repeat);
	alarmmgr_set_type(alarmInfo, ALARM_TYPE_VOLATILE);

	int alarmId;
	int ret = alarmmgr_add_alarm_with_localtime(alarmInfo, NULL, &alarmId);
	alarmmgr_free_alarm(alarmInfo);

	IF_FAIL_RETURN_TAG(ret == ALARMMGR_RESULT_SUCCESS, -1, _E, "Alarm setting failed");

	ScopeMutex sm(&__mutex);
	__listenerMap[alarmId] = std::make_pair(listener, this);
	__alarms.insert(alarmId);

	_D("Timer %d was set at %02d:%02d:00 (day of week: %#x)", alarmId, hour, min, dow);
	return alarmId;
}

VISIBLE void TimerManager::remove(int timerId)
{
	ScopeMutex sm(&__mutex);
	auto it = __listenerMap.find(timerId);
	if (it != __listenerMap.end()) {
		__listenerMap.erase(it);
		__alarms.erase(timerId);
		alarmmgr_remove_alarm(timerId);
		_D("Timer %d was removed", timerId);
	}
}

bool TimerManager::__init()
{
	int result = alarmmgr_init(IDENTIFIER);
	IF_FAIL_RETURN_TAG(result == ALARMMGR_RESULT_SUCCESS, false, _E, "Alarm manager initialization failed");

	result = alarmmgr_set_cb(__onAlarmExpired, NULL);
	if (result != ALARMMGR_RESULT_SUCCESS) {
		alarmmgr_fini();
		_E("Alarm callback registration failed");
		return false;
	}

	alarmmgr_remove_all();
	_D("Timer initialized");
	return true;
}

void TimerManager::__release()
{
	alarmmgr_remove_all();
	alarmmgr_fini();
	__listenerMap.clear();
	_D("Timer released");
}

int TimerManager::__onAlarmExpired(int alarmId, void *userdata)
{
	ITimerListener *listener = NULL;
	TimerManager *timer = NULL;

	{
		ScopeMutex sm(&__mutex);
		auto it = __listenerMap.find(alarmId);
		IF_FAIL_RETURN_TAG(it != __listenerMap.end(), 0, _W, "Unknown Alarm %d", alarmId);
		listener = it->second.first;
		timer = it->second.second;
	}

	_D("Timer %d expired", alarmId);

	if (listener->onTimerExpired(alarmId))
		return 0;

	_D("Stop repeating TimerManager %d", alarmId);
	timer->remove(alarmId);
	return 0;
}
