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
#include <string.h>
#include <glib.h>
#include <phonenumbers/phonenumberutil.h>
#include <phonenumbers/asyoutypeformatter.h>
#include <phonenumbers/geocoding/phonenumber_offline_geocoder.h>
#include "phn_common.h"
#include "phn_phonenumber_wrapper.h"

using namespace i18n::phonenumbers;

int phn_get_location_from_number(const char *number, const char *region, const char *language, char **location)
{
	PhoneNumber phNumber;
	const PhoneNumberUtil& pn_instance = *PhoneNumberUtil::GetInstance();
	const PhoneNumberUtil::ErrorType status = pn_instance.Parse(
		number, region, &phNumber);
	RETVM_IF(status != PhoneNumberUtil::NO_PARSING_ERROR, PHONE_NUMBER_ERROR_NO_DATA, "parse() failed(%d)", status);

	const std::string description =
		PhoneNumberOfflineGeocoder().GetDescriptionForNumber(
			phNumber, icu::Locale(language));
	*location = g_strdup((gchar *)description.c_str());

	return PHONE_NUMBER_ERROR_NONE;
}

int phn_get_formatted_number(const char *number, const char *region, char **formatted_number)
{
	const PhoneNumberUtil& pn_instance = *PhoneNumberUtil::GetInstance();
	AsYouTypeFormatter *formatter = pn_instance.GetAsYouTypeFormatter(region);

	int i=0;
	string result;
	while (number[i] && '\0' != number[i]) {
		formatter->InputDigit(number[i++], &result);
	}
	delete formatter;
	*formatted_number = g_strdup((gchar *)result.c_str());

	return PHONE_NUMBER_ERROR_NONE;
}

