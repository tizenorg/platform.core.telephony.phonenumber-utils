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


API int phone_number_get_location_from_number(const char *number,
		phone_number_region_e region, phone_number_lang_e lang, char **location)
{
	int ret;
	char *region_str = NULL;
	char *lang_str = NULL;

	RETVM_IF(NULL == number || '\0' == *number, PHONE_NUMBER_ERROR_INVALID_PARAMETER,
			"Invalid parameter (number is NULL)");
	RETVM_IF(region < 0 || PHONE_NUMBER_REGION_MAX <= region, PHONE_NUMBER_ERROR_INVALID_PARAMETER,
			"Invalid parameter (region:%d)", region);
	RETVM_IF(lang < 0 || PHONE_NUMBER_LANG_MAX <= lang, PHONE_NUMBER_ERROR_INVALID_PARAMETER,
			"Invalid parameter (lang:%d)", lang);
	RETVM_IF(NULL == location, PHONE_NUMBER_ERROR_INVALID_PARAMETER,
			"Invalid parameter (location is NULL)");

	ret = phn_region_data_get_region_str(region, &region_str);
	if (PHONE_NUMBER_ERROR_NONE != ret) {
		ERR("phn_region_data_get_region_str() Fail(%d)", ret);
		return ret;
	}

	ret = phn_region_data_get_lang_str(lang, &lang_str);
	if (PHONE_NUMBER_ERROR_NONE != ret) {
		ERR("phn_region_data_get_lang_str() Fail(%d)", ret);
		free(region_str);
		return ret;
	}

	char *location_file = NULL;
	ret = phn_location_find_extra_data(region_str, &location_file);
	if (PHONE_NUMBER_ERROR_NONE == ret && location_file) {
		ret = phn_location_get_location_from_extra_data(location_file, number,
				region_str, lang_str, location);
		free(location_file);
		if (PHONE_NUMBER_ERROR_NONE == ret && *location) {
			free(region_str);
			free(lang_str);
			return PHONE_NUMBER_ERROR_NONE;
		}
	}

	bool exist = phn_region_data_find_match_info(region, lang);
	if (false == exist) {
		INFO("Language not matched with Region. Set to defualt language.");
		free(lang_str);
		lang_str = strdup(PHN_REGION_DEFAULT_LANG);
	}

	ret = phn_get_location_from_number(number, region_str, lang_str, location);
	if (PHONE_NUMBER_ERROR_NONE != ret) {
		ERR("phn_get_location_from_number() Fail(%d)", ret);
		free(region_str);
		free(lang_str);
		return ret;
	}

	free(region_str);
	free(lang_str);
	return PHONE_NUMBER_ERROR_NONE;
}

API int phone_number_get_formatted_number(const char *number, phone_number_region_e region, char **formatted_number)
{
	int ret;
	char *region_str = NULL;

	RETVM_IF(NULL == number || '\0' == *number, PHONE_NUMBER_ERROR_INVALID_PARAMETER,
			"Invalid parameter (number is NULL)");
	RETVM_IF((int)region < 0 || PHONE_NUMBER_REGION_MAX <= region, PHONE_NUMBER_ERROR_INVALID_PARAMETER,
			"Invalid parameter (region:%d)", region);
	RETVM_IF(NULL == formatted_number, PHONE_NUMBER_ERROR_INVALID_PARAMETER, "Invalid parameter (formatted_number is NULL)");

	ret = phn_region_data_get_region_str(region, &region_str);
	if (PHONE_NUMBER_ERROR_NONE != ret) {
		ERR("phn_region_data_get_region_str() Fail(%d)", ret);
		return ret;
	}

	ret = phn_get_formatted_number(number, region_str, formatted_number);
	if (PHONE_NUMBER_ERROR_NONE != ret) {
		ERR("phn_get_formatted_number() Fail(%d)", ret);
		free(region_str);
		return ret;
	}

	free(region_str);
	return PHONE_NUMBER_ERROR_NONE;
}

