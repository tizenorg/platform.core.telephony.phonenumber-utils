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

#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <glib-object.h>
#include <gio/gio.h>

#include "phone_number.h"
#include "phone_number_errors.h"
#include "phone_number_types.h"

#include "phn-dbus.h"
#include "phnc.h"

static phnDbus *phn_client_dbus_object;


int _phn_client_dbus_start()
{
	GError *error = NULL;
	FN_CALL;

	if (phn_client_dbus_object) {
		DBG("phn_client_dbus_object is already created");
		return PHONE_NUMBER_ERROR_NONE;
	}

	phn_client_dbus_object = phn_dbus_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM,
			G_DBUS_PROXY_FLAGS_NONE,
			PHN_DBUS_INTERFACE,
			PHN_DBUS_OBJPATH,
			NULL,
			&error);
	if (NULL == phn_client_dbus_object) {
		ERR("phn_client_dbus_proxy_new_for_bus_sync() Fail(%s)", error->message);
		g_error_free(error);
		return PHONE_NUMBER_ERROR_SYSTEM;
	}

	return PHONE_NUMBER_ERROR_NONE;
}


API int phone_number_connect(void)
{
	int ret;
	FN_CALL;

#if !GLIB_CHECK_VERSION(2,35,0)
	g_type_init();
#endif

	ret = _phn_client_dbus_start();
	if (PHONE_NUMBER_ERROR_NONE != ret)
		ERR("_phn_client_dbus_start() Fail(%d)", ret);

	return ret;
}


API void phone_number_disconnect(void)
{
	if (NULL == phn_client_dbus_object) {
		WARN("dbus not initialized");
		return;
	}

	g_object_unref(phn_client_dbus_object);
	phn_client_dbus_object = NULL;
	DBG("client closed");
}


API int phone_number_get_location_from_number(const char *number,
		phone_number_region_e region, phone_number_lang_e lang, char **location)
{
	GError *error = NULL;
	char *out_loc = NULL;
	int ret;
	FN_CALL;

	RETVM_IF(NULL == number || '\0' == *number, PHONE_NUMBER_ERROR_INVALID_PARAMETER,
			"Invalid parameter (number is NULL)");
	RETVM_IF(region < 0 || PHONE_NUMBER_REGION_MAX <= region, PHONE_NUMBER_ERROR_INVALID_PARAMETER,
			"Invalid parameter (region:%d)", region);
	RETVM_IF(lang < 0 || PHONE_NUMBER_LANG_MAX <= lang, PHONE_NUMBER_ERROR_INVALID_PARAMETER,
			"Invalid parameter (lang:%d)", lang);
	RETVM_IF(NULL == location, PHONE_NUMBER_ERROR_INVALID_PARAMETER,
			"Invalid parameter (location is NULL)");

	if (NULL == phn_client_dbus_object) {
		phone_number_connect();
		phn_dbus_call_get_location_sync (
			phn_client_dbus_object, number, region, lang, &out_loc, &ret, NULL, &error);
		phone_number_disconnect();
	}
	else {
		phn_dbus_call_get_location_sync (
			phn_client_dbus_object, number, region, lang, &out_loc, &ret, NULL, &error);
	}

	if (NULL != error) {
		ERR("dbus sync error : %s", error->message);
		free(out_loc);
		return PHONE_NUMBER_ERROR_SYSTEM;
	}

	if (PHONE_NUMBER_ERROR_NONE != ret) {
		ERR("Phonenumber utils error : %d", ret);
		free(out_loc);
		return ret;
	}

	*location = out_loc;

	return ret;
}

API int phone_number_get_formatted_number(const char *number,
		phone_number_region_e region, char **formatted_number)
{
	GError *error = NULL;
	char *out_num = NULL;
	int ret;
	FN_CALL;

	if (NULL == phn_client_dbus_object) {
		phone_number_connect();
		phn_dbus_call_get_number_sync (
			phn_client_dbus_object, number, region, &out_num, &ret, NULL, &error);
		phone_number_disconnect();
	}
	else {
		phn_dbus_call_get_number_sync (
			phn_client_dbus_object, number, region, &out_num, &ret, NULL, &error);
	}

	if (NULL != error) {
		ERR("dbus sync error : %s", error->message);
		free(out_num);
		return PHONE_NUMBER_ERROR_SYSTEM;
	}

	if (PHONE_NUMBER_ERROR_NONE != ret) {
		ERR("Phonenumber utils error : %d", ret);
		free(out_num);
		return ret;
	}

	*formatted_number = out_num;
	return ret;
}


API int phone_number_get_normalized_number(const char *number, char **normalized_number)
{
	GError *error = NULL;
	char *out_num = NULL;
	int ret;
	FN_CALL;

	RETVM_IF(NULL == number || '\0' == *number, PHONE_NUMBER_ERROR_INVALID_PARAMETER,
			"Invalid parameter (number is NULL)");
	RETVM_IF(NULL == normalized_number, PHONE_NUMBER_ERROR_INVALID_PARAMETER, "Invalid parameter (normalized_number is NULL)");

	if (NULL == phn_client_dbus_object) {
		phone_number_connect();
		phn_dbus_call_get_normalized_number_sync (
			phn_client_dbus_object, number, &out_num, &ret, NULL, &error);
		phone_number_disconnect();
	}
	else {
		phn_dbus_call_get_normalized_number_sync (
			phn_client_dbus_object, number, &out_num, &ret, NULL, &error);
	}

	if (NULL != error) {
		ERR("dbus sync error : %s", error->message);
		free(out_num);
		return PHONE_NUMBER_ERROR_SYSTEM;
	}

	if (PHONE_NUMBER_ERROR_NONE != ret) {
		ERR("Phonenumber utils error : %d", ret);
		free(out_num);
		return ret;
	}

	*normalized_number = out_num;
	return ret;
}

