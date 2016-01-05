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
#include <system_info.h>

#include "phone_number.h"
#include "phone_number_errors.h"
#include "phone_number_types.h"

#include "phn-dbus.h"
#include "phnc.h"

#define PHN_FEATURE_TELEPHONY "http://tizen.org/feature/network.telephony"

#define PHN_FEATURE_TELEPHONY_NOT_CHECKED -1
#define PHN_FEATURE_TELEPHONY_NOT_SUPPORTED 0
#define PHN_FEATURE_TELEPHONY_SUPPORTED 1

static phnDbus *phn_client_dbus_object;

static int _phn_client_dbus_start()
{
	FN_CALL;
	GError *error = NULL;

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


static int _phn_is_support_telephony_feature()
{
	int err;
	bool is_support;
	static int _phn_telephony_feature_support = PHN_FEATURE_TELEPHONY_NOT_CHECKED;

	if (PHN_FEATURE_TELEPHONY_NOT_CHECKED != _phn_telephony_feature_support)
		return _phn_telephony_feature_support;

	err  = system_info_get_platform_bool(PHN_FEATURE_TELEPHONY, &is_support);
	if (SYSTEM_INFO_ERROR_NONE != err) {
		DBG("Error system_info_get_platform_bool : %d", err);
		return PHONE_NUMBER_ERROR_SYSTEM;
	}

	if (is_support)
		_phn_telephony_feature_support = PHN_FEATURE_TELEPHONY_SUPPORTED;
	else
		_phn_telephony_feature_support = PHN_FEATURE_TELEPHONY_NOT_SUPPORTED;

	return _phn_telephony_feature_support;
}


API int phone_number_connect(void)
{
	int ret;
	FN_CALL;

#if !GLIB_CHECK_VERSION(2, 35, 0)
	g_type_init();
#endif

	ret = _phn_client_dbus_start();
	if (PHONE_NUMBER_ERROR_NONE != ret)
		ERR("_phn_client_dbus_start() Fail(%d)", ret);

	return ret;
}


API int phone_number_disconnect(void)
{
	RETV_IF(NULL == phn_client_dbus_object, PHONE_NUMBER_ERROR_SYSTEM);

	g_object_unref(phn_client_dbus_object);
	phn_client_dbus_object = NULL;

	DBG("client closed");

	return PHONE_NUMBER_ERROR_NONE;
}


API int phone_number_get_location_from_number(const char *number,
		phone_number_region_e region, phone_number_lang_e lang, char **location)
{
	FN_CALL;
	int ret;
	GError *error = NULL;
	char *out_loc = NULL;

	RETV_IF(NULL == number, PHONE_NUMBER_ERROR_INVALID_PARAMETER);
	RETV_IF('\0' == *number, PHONE_NUMBER_ERROR_INVALID_PARAMETER);
	RETV_IF(NULL == location, PHONE_NUMBER_ERROR_INVALID_PARAMETER);
	RETVM_IF(region < 0 || PHONE_NUMBER_REGION_MAX <= region,
			PHONE_NUMBER_ERROR_INVALID_PARAMETER, "region(%d) Invalid", region);
	RETVM_IF(lang < 0 || PHONE_NUMBER_LANG_MAX <= lang,
			PHONE_NUMBER_ERROR_INVALID_PARAMETER, "lang(%d) Invalid", lang);

	if (NULL == phn_client_dbus_object) {
		phone_number_connect();
		phn_dbus_call_get_location_sync(phn_client_dbus_object, number, region, lang,
				&out_loc, &ret, NULL, &error);
		phone_number_disconnect();
	} else {
		phn_dbus_call_get_location_sync(phn_client_dbus_object, number, region, lang,
				&out_loc, &ret, NULL, &error);
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
	FN_CALL;
	int ret;
	GError *error = NULL;
	char *out_num = NULL;

	RETV_IF(NULL == number, PHONE_NUMBER_ERROR_INVALID_PARAMETER);
	RETV_IF('\0' == *number, PHONE_NUMBER_ERROR_INVALID_PARAMETER);
	RETV_IF(NULL == formatted_number, PHONE_NUMBER_ERROR_INVALID_PARAMETER);
	RETVM_IF(region < 0 || PHONE_NUMBER_REGION_MAX <= region,
			PHONE_NUMBER_ERROR_INVALID_PARAMETER, "region(%d) Invalid", region);


	if (NULL == phn_client_dbus_object) {
		phone_number_connect();
		phn_dbus_call_get_number_sync(phn_client_dbus_object, number, region, &out_num,
				&ret, NULL, &error);
		phone_number_disconnect();
	} else {
		phn_dbus_call_get_number_sync(phn_client_dbus_object, number, region, &out_num,
				&ret, NULL, &error);
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
	FN_CALL;
	int ret;
	GError *error = NULL;
	char *out_num = NULL;

	ret = _phn_is_support_telephony_feature();
	RETV_IF(PHONE_NUMBER_ERROR_SYSTEM == ret, PHONE_NUMBER_ERROR_SYSTEM);
	RETV_IF(PHN_FEATURE_TELEPHONY_NOT_SUPPORTED == ret, PHONE_NUMBER_ERROR_NOT_SUPPORTED);

	RETV_IF(NULL == number, PHONE_NUMBER_ERROR_INVALID_PARAMETER);
	RETV_IF('\0' == *number, PHONE_NUMBER_ERROR_INVALID_PARAMETER);
	RETV_IF(NULL == normalized_number, PHONE_NUMBER_ERROR_INVALID_PARAMETER);

	if (NULL == phn_client_dbus_object) {
		phone_number_connect();
		phn_dbus_call_get_normalized_number_sync(phn_client_dbus_object, number, &out_num,
				&ret, NULL, &error);
		phone_number_disconnect();
	} else {
		phn_dbus_call_get_normalized_number_sync(phn_client_dbus_object, number, &out_num,
				&ret, NULL, &error);
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

