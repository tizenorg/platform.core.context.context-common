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

#ifndef _EVENT_DRIVEN_THREAD_H_
#define _EVENT_DRIVEN_THREAD_H_

#include <glib.h>
#include <types_internal.h>

class VISIBLE EventDrivenThread {

	typedef struct thread_info_s* thread_info_t;

public:
	virtual ~EventDrivenThread();

	bool start();
	bool stop();
	bool isRunning();

protected:
	EventDrivenThread();

	virtual void onEvent(int type, void* data) = 0;

	bool pushEvent(int type, void* data);

private:
	static gpointer __threadFunc(gpointer data);

	void __run();

	thread_info_t __threadInfo;

};	/* class EventDrivenThread */

#endif /* _EVENT_DRIVEN_THREAD_H_ */
