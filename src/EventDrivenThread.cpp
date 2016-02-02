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

#include <new>
#include <atomic>
#include <types_internal.h>
#include <EventDrivenThread.h>

struct thread_info_s {
	GThread *thread;
	std::atomic_bool isRunning;
	GAsyncQueue *eventQueue;
	thread_info_s() : thread(NULL), isRunning(false), eventQueue(NULL) {}
};

struct event_message_s {
	int type;
	void* data;
	bool term;
	event_message_s() : type(-1), data(NULL), term(false) {}
};

EventDrivenThread::EventDrivenThread()
	: __threadInfo(NULL)
{
}

EventDrivenThread::~EventDrivenThread()
{
	stop();
	delete __threadInfo;
}

bool EventDrivenThread::start()
{
	if (!__threadInfo) {
		__threadInfo = new(std::nothrow) thread_info_s;
	}

	IF_FAIL_RETURN_TAG(__threadInfo, false, _E, "Memory allocation failed");

	if (!__threadInfo->isRunning.load()) {

		__threadInfo->eventQueue = g_async_queue_new();

		if (__threadInfo->eventQueue == NULL) {
			_E("Memory allocation failed");
			return false;
		}

		__threadInfo->isRunning = true;

		__threadInfo->thread = g_thread_new(NULL, __threadFunc, static_cast<gpointer>(this));

		if (__threadInfo->thread == NULL) {
			_E(RED("Thread creation failed"));
			__threadInfo->isRunning = false;
			g_async_queue_unref(__threadInfo->eventQueue);
			return false;
		}

		_I(PURPLE("A thread initiated"));
	}

	return true;
}

bool EventDrivenThread::stop()
{
	if (__threadInfo->isRunning.load()) {
		event_message_s* event = new(std::nothrow) event_message_s;
		IF_FAIL_RETURN_TAG(event, false, _E, "Memory allocation failed");

		event->term = true;
		g_async_queue_push(__threadInfo->eventQueue, event);
		__threadInfo->isRunning = false;
		g_thread_join(__threadInfo->thread);

		/* Free the memory allocated for the event queue */
		while ((event = static_cast<event_message_s*>(g_async_queue_try_pop(__threadInfo->eventQueue)))) {
			if (event->data) {
				deleteEvent(event->type, event->data);
			}
			delete event;
		}
		g_async_queue_unref(__threadInfo->eventQueue);
	}

	return true;
}

bool EventDrivenThread::isRunning()
{
	return __threadInfo->isRunning.load();
}

bool EventDrivenThread::pushEvent(int type, void* data)
{
	if (__threadInfo->isRunning.load()) {
		event_message_s* event = new(std::nothrow) event_message_s;
		IF_FAIL_RETURN_TAG(event, false, _E, "Memory allocation failed");

		event->type = type;
		event->data = data;
		g_async_queue_push(__threadInfo->eventQueue, event);
		return true;
	}

	return false;
}

gpointer EventDrivenThread::__threadFunc(gpointer data)
{
	static_cast<EventDrivenThread*>(data)->__run();
	return NULL;
}

void EventDrivenThread::__run()
{
	event_message_s *event = NULL;

	while (__threadInfo->isRunning.load()) {

		event = static_cast<event_message_s*>(g_async_queue_pop(__threadInfo->eventQueue));

		if (event) {
			if (event->term){
				delete event;
				break;
			} else {
				onEvent(event->type, event->data);
				delete event;
			}
		}

	}
}
