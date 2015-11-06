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

#include <glib.h>
#include "phn-log.h"
#include "phnd.h"
#include "phnd-utils.h"

#define PHN_DAEMON_DEFAULT_TIMEOUT 90

static int _phnd_timeout_sec = PHN_DAEMON_DEFAULT_TIMEOUT;
static guint _phnd_timeout_handle = 0;

static gboolean _phnd_timeout_cb(gpointer user_data)
{
	phnd_daemon_quit();
	return FALSE;
}

void phnd_utils_start_timeout()
{
	DBG("start timeout = %d", _phnd_timeout_sec);
	if (_phnd_timeout_sec < 1)
		return;

	if (_phnd_timeout_handle)
		g_source_remove(_phnd_timeout_handle);

	_phnd_timeout_handle = g_timeout_add_seconds(_phnd_timeout_sec, _phnd_timeout_cb, NULL);
}

void phnd_utils_stop_timeout()
{
	if (_phnd_timeout_sec < 1)
		return;

	if (_phnd_timeout_handle)
		g_source_remove(_phnd_timeout_handle);
	_phnd_timeout_handle = 0;
}
