/*
 * Phonenumber Utils
 *
 * Copyright (c) 2015 Samsung Electronics Co., Ltd. All rights reserved.
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
 *
 */
#include <stdlib.h>
#include <pthread.h>
#include <glib.h>

#include "phnd.h"
#include "phnd-dbus.h"

static GMainLoop *_main_loop;

/* LCOV_EXCL_START */
void phnd_daemon_quit()
{
	INFO("phonenumber-utils daemon is quit by timeout.");
	g_main_loop_quit(_main_loop);
	_main_loop = NULL;
}
/* LCOV_EXCL_STOP */

int main(int argc, char **argv)
{
	guint id;

	INFO("start phonenumber utils daemon");

	_main_loop = g_main_loop_new(NULL, FALSE);

	id = phnd_dbus_init();

	g_main_loop_run(_main_loop);

	phnd_dbus_deinit(id);
	g_main_loop_unref(_main_loop);

	return 0;
}



