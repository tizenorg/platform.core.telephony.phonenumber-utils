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

#include "phone_number.h"
#include "phn-log.h"

int main(int argc, char **argv)
{

	DBG("start test main !!!");

	char *location = NULL;
	char *formatted_number = NULL;
	char *normalized_number = NULL;
	char test_number[11] = {0};
	int region = PHONE_NUMBER_REGION_REPUBLIC_OF_KOREA;
	int lang = PHONE_NUMBER_LANG_KOREAN;
	int count = 0;
	int ret;

	ret = phone_number_client_open();

	if (PHONE_NUMBER_ERROR_NONE != ret) {
		ERR("client open error!!");
		return 0;
	}

	while(1) {

		if (count > 5) {
			count = 0;
			break;
		}

		/* test number start from 0212340000 to 0212340005*/
		snprintf(test_number, sizeof(test_number), "021234%04d", count);
		DBG("========== start test with number[%s], region[%d], lang[%d] =====", test_number, region, lang);

		DBG("=========== Call get location =======================");
		ret = phone_number_get_location_from_number(test_number, region, lang, &location);
		if (PHONE_NUMBER_ERROR_NONE != ret) {
			ERR("phone_number_get_location_from_number() Fail(%d)", ret);
			phone_number_client_close();
			return -1;
		}
		/* location - 'seoul' */
		DBG("location = %s", location);
		free(location);
		location = NULL;


		DBG("=========== Call get formatted number =======================");
		ret = phone_number_get_formatted_number(test_number, region, &formatted_number);
		if (PHONE_NUMBER_ERROR_NONE != ret) {
			ERR("phone_number_get_formatted_number() Fail(%d)", ret);
			phone_number_client_close();
			return -1;
		}

		/* formatted_number - '02-1234-0001' */
		DBG("formatted_number = %s", formatted_number);
		free(formatted_number);
		formatted_number = NULL;

		DBG("=========== Call get normalized number =======================");
		ret = phone_number_get_normalized_number(test_number, &normalized_number);
		if (PHONE_NUMBER_ERROR_NONE != ret) {
			ERR("phone_number_get_normalized_number() Fail(%d)", ret);
			phone_number_client_close();
			return -1;
		}

		/* normalized_number - '+82212340001' */
		DBG("normalized_number = %s", normalized_number);
		free(normalized_number);
		normalized_number = NULL;

		count++;
	}
	phone_number_client_close();

	DBG("end test main !!!");
	return 0;
}



