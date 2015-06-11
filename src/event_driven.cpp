/*
 * Copyright (c) 2014 Samsung Electronics Co., Ltd.
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
#include <types_internal.h>
#include <event_driven.h>

struct thread_info_s {
	GThread *thread;
	gint is_running;
	GAsyncQueue *event_queue;
};

struct event_message_s {
	bool is_stop_signal;
	int type;
	void* data;
	event_message_s() : is_stop_signal(false), type(-1), data(NULL) {}
};

event_driven_thread::event_driven_thread()
{
	thread_info = static_cast<thread_info_s*>(g_malloc(sizeof(thread_info_s)));

	if (thread_info) {
		thread_info->thread = NULL;
		thread_info->is_running = 0;
		thread_info->event_queue = NULL;
	}
}

event_driven_thread::~event_driven_thread()
{
	stop();
	g_free(thread_info);
}

bool event_driven_thread::start()
{
	IF_FAIL_RETURN_TAG(thread_info, false, _E, "Memory allocation failed");

	if (!g_atomic_int_get(&(thread_info->is_running))) {

#if 0
		int stack_size = sysconf(_SC_PAGESIZE) * 4;
		void *stack = NULL;
		stack = g_malloc(stack_size);
		IF_FAIL_RETURN_TAG(stack, false, _E, "Memory allocation failed");
#endif

		thread_info->event_queue = g_async_queue_new();
		if (thread_info->event_queue == NULL) {
			_E("Memory allocation failed");
			return false;
		}

		g_atomic_int_set(&thread_info->is_running, 1);

#if 0
		thread_info->pid = clone(&thread_func_wrapper, (char*)stack + stack_size,
				SIGCHLD | CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SYSVSEM, this);

		if (thread_info->pid < 0) {
			_E(RED("Thread creation failed"));
			g_atomic_int_set(&thread_info->is_running, 0);
			g_free(stack);
			g_async_queue_unref(thread_info->event_queue);
			return false;
		}

		_I(PURPLE("PID %d initiated"), thread_info->pid);
#endif

		thread_info->thread = g_thread_new(NULL, thread_func_wrapper, static_cast<gpointer>(this));

		if (thread_info->thread == NULL) {
			_E(RED("Thread creation failed"));
			g_atomic_int_set(&thread_info->is_running, 0);
			g_async_queue_unref(thread_info->event_queue);
			return false;
		}

		_I(PURPLE("A thread initiated"));
	}

	return true;
}

bool event_driven_thread::stop()
{
	if (g_atomic_int_get(&(thread_info->is_running))) {
		event_message_s* event = new(std::nothrow) event_message_s;
		IF_FAIL_RETURN_TAG(event, false, _E, "Memory allocation failed");

		event->is_stop_signal = true;
		g_async_queue_push(thread_info->event_queue, event);
		g_atomic_int_set(&thread_info->is_running, 0);

#if 0
		pid_t pid = waitpid(thread_info->pid, NULL, 0);
		_I(PURPLE("PID %d terminated"), pid);
		if (pid < 0) {
			_E(RED("Failed to join %d from %d"), thread_info->pid, getpid());
		}
#endif
		g_thread_join(thread_info->thread);

		// Free the memory allocated for the event queue
		while ((event = static_cast<event_message_s*>(g_async_queue_try_pop(thread_info->event_queue)))) {
			if (event->data) {
				delete_thread_event(event->type, event->data);
			}
			delete event;
		}
		g_async_queue_unref(thread_info->event_queue);
	}

	return true;
}

bool event_driven_thread::is_running()
{
	if (g_atomic_int_get(&(thread_info->is_running))) {
		return true;
	}
	return false;
}

bool event_driven_thread::push_thread_event(int type, void* data)
{
	if (g_atomic_int_get(&(thread_info->is_running))) {
		event_message_s* event = new(std::nothrow) event_message_s;
		IF_FAIL_RETURN_TAG(event, false, _E, "Memory allocation failed");

		event->type = type;
		event->data = data;
		g_async_queue_push(thread_info->event_queue, event);
		return true;
	}

	return false;
}

gpointer event_driven_thread::thread_func_wrapper(gpointer data)
{
	static_cast<event_driven_thread*>(data)->process_event_queue();
	return NULL;
}

void event_driven_thread::process_event_queue()
{
#if 0
	struct sigaction signal_action;
	signal_action.sa_handler = child_signal_handler;
	sigemptyset(&signal_action.sa_mask);

	sigaction(SIGINT, &signal_action, NULL);
	sigaction(SIGHUP, &signal_action, NULL);
	sigaction(SIGTERM, &signal_action, NULL);
	sigaction(SIGQUIT, &signal_action, NULL);
#endif

	event_message_s *event = NULL;

	while (g_atomic_int_get(&(thread_info->is_running))) {

		event = static_cast<event_message_s*>(g_async_queue_pop(thread_info->event_queue));

		if (event) {
			if (event->is_stop_signal){
				delete event;
				break;
			} else {
				on_thread_event_popped(event->type, event->data);
				delete event;
			}
		}

	}
}
