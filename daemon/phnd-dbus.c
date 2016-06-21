/*
 * Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
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
#include <stdlib.h>
#include <gio/gio.h>

#include "phn-dbus.h"
#include "phn-log.h"
#include "phn-common.h"
#include "phnd.h"
#include "phnd-utils.h"
#include "phnd-libphonenumber.h"
#include "phnd-region-data.h"

static inline int _dbus_get_location_handler(const char *number,
		phone_number_region_e region, phone_number_lang_e lang, char **location)
{
	int ret;
	char *lang_str = NULL;
	char *region_str = NULL;

	ret = phn_region_data_get_region_str(region, &region_str);
	if (PHONE_NUMBER_ERROR_NONE != ret) {
		/* LCOV_EXCL_START */
		ERR("phn_region_data_get_region_str() Fail(%d)", ret);
		return ret;
		/* LCOV_EXCL_STOP */
	}

	ret = phn_region_data_get_lang_str(lang, &lang_str);
	if (PHONE_NUMBER_ERROR_NONE != ret) {
		/* LCOV_EXCL_START */
		ERR("phn_region_data_get_lang_str() Fail(%d)", ret);
		free(region_str);
		return ret;
		/* LCOV_EXCL_STOP */
	}

	bool exist = phn_region_data_find_match_info(region, lang);
	if (false == exist) {
		INFO("Language not matched with Region. Set to defualt language.");
		free(lang_str);
		lang_str = strdup(PHN_REGION_DEFAULT_LANG);
	}

	ret = phn_get_location_from_number(number, region_str, lang_str, location);
	if (PHONE_NUMBER_ERROR_NONE != ret) {
		/* LCOV_EXCL_START */
		ERR("phn_get_location_from_number() Fail(%d)", ret);
		/* LCOV_EXCL_STOP */
	}

	free(region_str);
	free(lang_str);

	return ret;
}

int _dbus_get_number_handler(const char *number, phone_number_region_e region,
		char **formatted_number)
{
	int ret = PHONE_NUMBER_ERROR_NONE;
	char *region_str = NULL;

	ret = phn_region_data_get_region_str(region, &region_str);
	if (PHONE_NUMBER_ERROR_NONE != ret) {
		/* LCOV_EXCL_START */
		ERR("phn_region_data_get_region_str() Fail(%d)", ret);
		return ret;
		/* LCOV_EXCL_STOP */
	}

	ret = phn_get_formatted_number(number, region_str, formatted_number);
	if (PHONE_NUMBER_ERROR_NONE != ret) {
		/* LCOV_EXCL_START */
		ERR("phn_get_formatted_number() Fail(%d)", ret);
		/* LCOV_EXCL_STOP */
	}

	free(region_str);
	return ret;
}


int _dbus_get_normalized_number_handler(const char *number,
		char **normalized_number)
{
	int ret = PHONE_NUMBER_ERROR_NONE;

	ret = phn_get_normalized_number(number, normalized_number);
	if (PHONE_NUMBER_ERROR_NONE != ret) {
		/* LCOV_EXCL_START */
		ERR("phn_get_normalized_number() Fail(%d)", ret);
		/* LCOV_EXCL_STOP */
	}

	return ret;
}


static gboolean _dbus_handle_get_location(phnDbus *object,
		GDBusMethodInvocation *invocation,
		gchar *number,
		gint region,
		gint lang,
		guint signal_number)
{
	FN_CALL;
	int ret;
	char *location;

	DBG("number = %s, region = %d, lang = %d", number, region, lang);

	ret = _dbus_get_location_handler(number, region, lang, &location);
	if (PHONE_NUMBER_ERROR_NONE != ret) {
		/* LCOV_EXCL_START */
		ERR("err ret = %d", ret);
		location = strdup("");
		/* LCOV_EXCL_STOP */
	}

	phn_dbus_complete_get_location(object, invocation, location, ret);
	free(location);
	phnd_utils_start_timeout();

	return TRUE;
}

static gboolean _dbus_handle_get_number(phnDbus *object,
		GDBusMethodInvocation *invocation,
		gchar *number,
		gint region,
		guint signal_number)
{
	FN_CALL;
	char *formatted_number;
	int ret;

	DBG("number = %s, region = %d", number, region);

	ret = _dbus_get_number_handler(number, region, &formatted_number);

	if (PHONE_NUMBER_ERROR_NONE != ret) {
		/* LCOV_EXCL_START */
		ERR("err ret = %d", ret);
		formatted_number = strdup("");
		/* LCOV_EXCL_STOP */
	}

	phn_dbus_complete_get_number(object, invocation, formatted_number, ret);
	free(formatted_number);
	phnd_utils_start_timeout();

	return TRUE;
}

static gboolean _dbus_handle_get_normalized_number(phnDbus *object,
		GDBusMethodInvocation *invocation,
		gchar *number,
		guint signal_number)
{
	FN_CALL;
	char *normalized_number;
	int ret;

	DBG("number = %s", number);

	ret = _dbus_get_normalized_number_handler(number, &normalized_number);
	if (PHONE_NUMBER_ERROR_NONE != ret) {
		/* LCOV_EXCL_START */
		ERR("err ret = %d", ret);
		normalized_number = strdup("");
		/* LCOV_EXCL_STOP */
	}

	phn_dbus_complete_get_normalized_number(object, invocation, normalized_number, ret);
	free(normalized_number);
	phnd_utils_start_timeout();

	return TRUE;
}

static void _dbus_bus_acquired_handler(GDBusConnection *conn, const gchar *name,
		gpointer user_data)
{

	gboolean ret;
	phnDbus *dbus_object;
	GError *error = NULL;

	DBG("Acquired the bus %s", name);

	dbus_object = phn_dbus_skeleton_new();

	g_signal_connect(dbus_object, "handle-get-location",
			G_CALLBACK(_dbus_handle_get_location), NULL);
	g_signal_connect(dbus_object, "handle-get-number",
			G_CALLBACK(_dbus_handle_get_number), NULL);
	g_signal_connect(dbus_object, "handle-get-normalized-number",
			G_CALLBACK(_dbus_handle_get_normalized_number), NULL);

	ret = g_dbus_interface_skeleton_export(G_DBUS_INTERFACE_SKELETON(dbus_object), conn,
			PHN_DBUS_OBJPATH, &error);

	if (FALSE == ret) {
		/* LCOV_EXCL_START */
		ERR("g_dbus_interface_skeleton_export() Fail(%s)", error->message);
		g_error_free(error);
		/* LCOV_EXCL_STOP */
	}
}

static void _dbus_name_lost_handler(GDBusConnection *connection, const gchar *name,
		gpointer user_data)
{
	DBG("Lost the name %s", name);
}


static void _dbus_name_acquired_handler(GDBusConnection *connection, const gchar *name,
		gpointer user_data)
{
	DBG("Acquired the name %s", name);
}

unsigned int phnd_dbus_init()
{
	guint id;
	DBG("phnd_dbus_init");

	id = g_bus_own_name(G_BUS_TYPE_SYSTEM,
			PHN_DBUS_INTERFACE,
			G_BUS_NAME_OWNER_FLAGS_REPLACE,
			_dbus_bus_acquired_handler,
			_dbus_name_acquired_handler,
			_dbus_name_lost_handler,
			NULL,
			NULL);

	phnd_utils_start_timeout();

	if (0 == id) {
		/* LCOV_EXCL_START */
		ERR("g_bus_own_name() Fail");
		return 0;
		/* LCOV_EXCL_STOP */
	}

	return id;
}


void phnd_dbus_deinit(unsigned int id)
{
	g_bus_unown_name(id);
}
