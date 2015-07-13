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

class event_driven_thread {
	private:
		typedef struct thread_info_s* thread_info_t;
		thread_info_t thread_info;

		static gpointer thread_func_wrapper(gpointer data);
		void process_event_queue();
		virtual void on_thread_event_popped(int type, void* data) = 0;
		virtual void delete_thread_event(int type, void* data) = 0;

	protected:
		event_driven_thread();
		bool push_thread_event(int type, void* data = NULL);

	public:
		virtual ~event_driven_thread();
		bool start();
		bool stop();
		bool is_running();

};	/* class event_driven_thread */

#endif /* _EVENT_DRIVEN_THREAD_H_ */
