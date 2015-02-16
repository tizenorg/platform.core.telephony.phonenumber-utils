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

#include "phone_number.h"
#include "phn_common.h"
#include "phn_phonenumber_wrapper.h"
#include "phn_location.h"
#include "phn_region_data.h"

API int phone_number_replace_extra_location_data(phone_number_region_e region, const char *file_name)
{
	RETVM_IF(PHONE_NUMBER_REGION_CHINA != region, PHONE_NUMBER_ERROR_NOT_SUPPORTED,
			"Not supported region(%d)", region);
	RETVM_IF(NULL == file_name, PHONE_NUMBER_ERROR_INVALID_PARAMETER, "Invalid parameter (file_name is NULL)");
	RETVM_IF(strchr(file_name, '/'), PHONE_NUMBER_ERROR_INVALID_PARAMETER, "Invalid parameter");

	int ret;
	ret = phn_location_validate_extra_data(region, file_name);
	RETVM_IF(PHONE_NUMBER_ERROR_NONE != ret, ret, "phn_location_validate_extra_data() Fail(%d)", ret);

	ret = phn_location_replace_extra_data(region, file_name);
	RETVM_IF(PHONE_NUMBER_ERROR_NONE != ret, ret, "phn_location_replace_extra_data() Fail(%d)", ret);

	return PHONE_NUMBER_ERROR_NONE;
}

API int phone_number_get_location_from_number(const char *number,
		phone_number_region_e region, phone_number_lang_e lang, char **location)
{
	int ret;
	const char *region_str = NULL;
	const char *lang_str = NULL;

	RETVM_IF(NULL == number || '\0' == *number, PHONE_NUMBER_ERROR_INVALID_PARAMETER,
			"Invalid parameter (number is NULL)");
	RETVM_IF(region < 0 || PHONE_NUMBER_REGION_MAX <= region, PHONE_NUMBER_ERROR_INVALID_PARAMETER,
			"Invalid parameter (region:%d)", region);
	RETVM_IF(lang < 0 || PHONE_NUMBER_LANG_MAX <= lang, PHONE_NUMBER_ERROR_INVALID_PARAMETER,
			"Invalid parameter (lang:%d)", lang);
	RETVM_IF(NULL == location, PHONE_NUMBER_ERROR_INVALID_PARAMETER,
			"Invalid parameter (location is NULL)");

	region_str = phn_region_data_get_region_str(region);
	RETVM_IF(NULL == region_str, PHONE_NUMBER_ERROR_NOT_SUPPORTED, "phn_region_info not found(%d)", region);

	lang_str = phn_region_data_get_lang_str(lang);
	RETVM_IF(NULL == lang_str, PHONE_NUMBER_ERROR_NOT_SUPPORTED, "phn_lang_info not found(%d)", lang);

	char *location_file = NULL;
	ret = phn_location_find_extra_data(region_str, &location_file);
	if (PHONE_NUMBER_ERROR_NONE == ret && location_file) {
		ret = phn_location_get_location_from_extra_data(location_file, number, region, lang, location);
		free(location_file);
		if (PHONE_NUMBER_ERROR_NONE == ret && *location)
			return PHONE_NUMBER_ERROR_NONE;
	}

	bool exist = phn_region_data_find_match_info(region, lang);
	if (!exist)
		lang_str = PHN_REGION_DEFAULT_LANG;

	ret = phn_get_location_from_number(number, region_str, lang_str, location);
	RETVM_IF(PHONE_NUMBER_ERROR_NONE != ret, ret, "phn_get_location_from_number() Fail(%d)", ret);

	return PHONE_NUMBER_ERROR_NONE;
}

API int phone_number_get_formatted_number(const char *number, phone_number_region_e region, char **formatted_number)
{
	int ret;
	const char *region_str = NULL;

	RETVM_IF(NULL == number || '\0' == *number, PHONE_NUMBER_ERROR_INVALID_PARAMETER,
			"Invalid parameter (number is NULL)");
	RETVM_IF((int)region < 0 || PHONE_NUMBER_REGION_MAX <= region, PHONE_NUMBER_ERROR_INVALID_PARAMETER,
			"Invalid parameter (region:%d)", region);
	RETVM_IF(NULL == formatted_number, PHONE_NUMBER_ERROR_INVALID_PARAMETER, "Invalid parameter (formatted_number is NULL)");

	region_str = phn_region_data_get_region_str(region);
	RETVM_IF(NULL == region_str, PHONE_NUMBER_ERROR_NOT_SUPPORTED, "phn_region_info not found(%d)", region);

	ret = phn_get_formatted_number(number, region_str, formatted_number);
	RETVM_IF(PHONE_NUMBER_ERROR_NONE != ret, ret, "phn_get_formatted_number() Fail(%d)", ret);

	return PHONE_NUMBER_ERROR_NONE;
}


