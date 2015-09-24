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
#include <TapiUtility.h>
#include <ITapiNetwork.h>
#include <system_info.h>
#include <phonenumbers/phonenumberutil.h>
#include <phonenumbers/asyoutypeformatter.h>
#include <phonenumbers/geocoding/phonenumber_offline_geocoder.h>
#include "phn_common.h"
#include "phn_phonenumber_wrapper.h"

#define MCC_LEN 3
#define PHN_FEATURE_TELEPHONY "http://tizen.org/feature/network.telephony"

using namespace i18n::phonenumbers;

typedef struct {
	int mcc;
	int cc;
} mcc_cc_map_s;

static int _cc = 0;
static TapiHandle **_tapi_handle = NULL;
static int _modem_num = 0;
static int _phn_have_telephony_feature = -1;

static int _phn_get_cc(bool reload, int *out_cc);

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

static void _phn_cc_changed_cb(TapiHandle *handle, const char *noti_id, void *data, void *user_data)
{
	int ret;
	ret = _phn_get_cc(true, NULL);
	if (PHONE_NUMBER_ERROR_NONE != ret) {
		ERR("_phn_get_cc() Fail(%d)", ret);
		return;
	}
}

static int _phn_get_tapi_handle()
{
	int i;
	int ret;
	char **cp_list = NULL;
	_modem_num = 0;

	cp_list = tel_get_cp_name_list();
	if (NULL == cp_list) {
		ERR("tel_get_cp_name_list() Fail(NULL)");
		return PHONE_NUMBER_ERROR_SYSTEM;
	}

	while (cp_list[_modem_num])
		_modem_num++;
	DBG("_modem_num=[%d]", _modem_num);

	_tapi_handle = (TapiHandle **)calloc(_modem_num, sizeof(TapiHandle *));
	if (NULL == _tapi_handle) {
		ERR("calloc() Fail");
		g_strfreev(cp_list);
		return PHONE_NUMBER_ERROR_OUT_OF_MEMORY;
	}

	for (i = 0; i < _modem_num; i++) {
		_tapi_handle[i] = tel_init(cp_list[i]);
		if (NULL == _tapi_handle[i]) {
			ERR("tel_init() for _tapi_handle[%d] Fail", i);
			g_strfreev(cp_list);
			return PHONE_NUMBER_ERROR_SYSTEM;
		}

		ret = tel_register_noti_event(_tapi_handle[i], TAPI_PROP_NETWORK_PLMN,
				_phn_cc_changed_cb, NULL);
		if (TAPI_API_SUCCESS != ret) {
			if (TAPI_API_ACCESS_DENIED == ret) {
				ERR("tel_register_noti_event() Fail(%d)", ret);
				g_strfreev(cp_list);
				return PHONE_NUMBER_ERROR_PERMISSION_DENIED;
			} else {
				ERR("tel_register_noti_event() for _tapi_handle[%d] Fail(%d)", i, ret);
				g_strfreev(cp_list);
				return PHONE_NUMBER_ERROR_SYSTEM;
			}
		}
	}

	g_strfreev(cp_list);
	return PHONE_NUMBER_ERROR_NONE;
}

static int _phn_get_cc(bool reload, int *out_cc)
{
	int i = 0;
	int state;
	int ret;
	int mcc = 0;
	char *temp = NULL;
	static bool cc_loaded = false;

	mcc_cc_map_s _mcc_cc_list[] = {
		{0, 1},
		{202, 30},
		{204, 31},
		{206, 32},
		{208, 33},
		{212, 377},
		{213, 376},
		{214, 34},
		{216, 36},
		{218, 387},
		{219, 385},
		{220, 381},
		{222, 39},
		{225, 39},
		{226, 40},
		{228, 41},
		{230, 420},
		{231, 421},
		{232, 43},
		{234, 44},
		{235, 44},
		{238, 45},
		{240, 46},
		{242, 47},
		{244, 358},
		{246, 370},
		{247, 371},
		{248, 372},
		{250, 7},
		{255, 380},
		{257, 375},
		{259, 373},
		{260, 48},
		{262, 49},
		{266, 350},
		{268, 351},
		{270, 352},
		{272, 353},
		{274, 354},
		{276, 355},
		{278, 356},
		{280, 357},
		{282, 995},
		{283, 374},
		{284, 359},
		{286, 90},
		{288, 298},
		{290, 299},
		{292, 378},
		{293, 386},
		{294, 389},
		{295, 423},
		{297, 382},
		{302, 1},
		{308, 508},
		{310, 1},
		{311, 1},
		{312, 1},
		{313, 1},
		{314, 1},
		{315, 1},
		{316, 1},
		{330, 1},
		{332, 1},
		{334, 52},
		{338, 1},
		{340, 590},
		{340, 596},
		{342, 1},
		{344, 1},
		{346, 1},
		{348, 1},
		{350, 1},
		{352, 1},
		{354, 1},
		{356, 1},
		{358, 1},
		{360, 1},
		{362, 599},
		{363, 297},
		{364, 1},
		{365, 1},
		{366, 1},
		{368, 53},
		{370, 1},
		{372, 509},
		{374, 1},
		{376, 1},
		{400, 994},
		{401, 7},
		{402, 975},
		{404, 91},
		{405, 91},
		{406, 91},
		{410, 92},
		{412, 93},
		{413, 94},
		{414, 95},
		{415, 961},
		{416, 962},
		{417, 963},
		{418, 964},
		{419, 965},
		{420, 966},
		{421, 967},
		{422, 968},
		{424, 971},
		{425, 972},
		{426, 973},
		{427, 974},
		{428, 976},
		{429, 977},
		{430, 971},
		{431, 971},
		{432, 98},
		{434, 998},
		{436, 992},
		{437, 996},
		{438, 993},
		{440, 81},
		{441, 81},
		{450, 82},
		{452, 84},
		{454, 852},
		{455, 853},
		{456, 855},
		{457, 856},
		{460, 86},
		{461, 86},
		{466, 886},
		{467, 850},
		{470, 880},
		{472, 960},
		{502, 60},
		{505, 61},
		{510, 62},
		{514, 670},
		{515, 63},
		{520, 66},
		{525, 65},
		{528, 673},
		{530, 64},
		{536, 674},
		{537, 675},
		{539, 676},
		{540, 677},
		{541, 678},
		{542, 679},
		{543, 681},
		{544, 1},
		{545, 686},
		{546, 687},
		{547, 689},
		{548, 682},
		{549, 685},
		{550, 691},
		{551, 692},
		{552, 680},
		{602, 20},
		{603, 213},
		{604, 212},
		{605, 216},
		{606, 218},
		{607, 220},
		{608, 221},
		{609, 222},
		{610, 223},
		{611, 224},
		{612, 225},
		{613, 226},
		{614, 227},
		{615, 228},
		{616, 229},
		{617, 230},
		{618, 231},
		{619, 232},
		{620, 233},
		{621, 234},
		{622, 235},
		{623, 236},
		{624, 237},
		{625, 238},
		{626, 239},
		{627, 240},
		{628, 241},
		{629, 242},
		{630, 243},
		{631, 244},
		{632, 245},
		{633, 248},
		{634, 249},
		{635, 250},
		{636, 251},
		{637, 252},
		{638, 253},
		{639, 254},
		{640, 255},
		{641, 256},
		{642, 257},
		{643, 258},
		{645, 260},
		{646, 261},
		{647, 262},
		{648, 263},
		{649, 264},
		{650, 265},
		{651, 266},
		{652, 267},
		{653, 268},
		{654, 269},
		{655, 27},
		{657, 291},
		{702, 501},
		{704, 502},
		{706, 503},
		{708, 504},
		{710, 505},
		{712, 506},
		{714, 507},
		{716, 51},
		{722, 54},
		{724, 55},
		{730, 56},
		{732, 57},
		{734, 58},
		{736, 591},
		{738, 592},
		{740, 593},
		{742, 594},
		{744, 595},
		{746, 597},
		{748, 598},
		{750, 500},
	};

	if (cc_loaded && false == reload) {
		if (NULL == out_cc) {
			ERR("Invalid parameter (out_cc is NULL)");
			return PHONE_NUMBER_ERROR_INVALID_PARAMETER;
		}

		*out_cc = _cc;
		DBG("cc=[%d]", *out_cc);
		return PHONE_NUMBER_ERROR_NONE;
	}

	_cc = 0;
	cc_loaded = true;

	if (NULL == _tapi_handle) {
		ret = _phn_get_tapi_handle();
		if (PHONE_NUMBER_ERROR_NONE != ret) {
			ERR("_phn_get_tapi_handle() Fail(%d)", ret);
			return ret;
		}
	}

	do {
		WARN("Using multi SIM");
		free(temp);
		temp = NULL;

		ret = tel_get_property_int(_tapi_handle[i], TAPI_PROP_NETWORK_SERVICE_TYPE, &state);
		if (TAPI_API_SUCCESS != ret) {
			if (TAPI_API_ACCESS_DENIED == ret) {
				ERR("tel_get_property_int() Fail(%d)", ret);
				return PHONE_NUMBER_ERROR_PERMISSION_DENIED;
			} else {
				ERR("tel_get_property_int() Fail(%d)", ret);
				return PHONE_NUMBER_ERROR_SYSTEM;
			}
		}

		if (TAPI_NETWORK_SERVICE_TYPE_UNKNOWN == state
				|| TAPI_NETWORK_SERVICE_TYPE_NO_SERVICE == state
				|| TAPI_NETWORK_SERVICE_TYPE_EMERGENCY == state
				|| TAPI_NETWORK_SERVICE_TYPE_SEARCH == state) {
			WARN("network service is not working : state(%d)", state);
		}

		ret = tel_get_property_string(_tapi_handle[i], TAPI_PROP_NETWORK_PLMN, &temp);
		if (TAPI_API_SUCCESS != ret) {
			if (TAPI_API_ACCESS_DENIED == ret) {
				ERR("tel_get_property_string() Fail(%d)", ret);
				return PHONE_NUMBER_ERROR_PERMISSION_DENIED;
			} else {
				ERR("tel_get_property_string() Fail(%d)", ret);
				return PHONE_NUMBER_ERROR_SYSTEM;
			}
		}
		DBG("temp=[%s] from _tapi_handle[%d]", temp, i);

		i++;
		if (_modem_num <= i) break;

	} while (NULL == temp || '\0' == temp[0]);

	if (NULL == temp || '\0' == temp[0]) {
		ERR("get NETWORK_PLMN Fail");
		free(temp);
		return PHONE_NUMBER_ERROR_SYSTEM;
	}

	if (temp && MCC_LEN < strlen(temp))
		temp[MCC_LEN] = '\0';
	mcc = atoi(temp);
	DBG("mcc=[%d]", mcc);

	for (i = 0; i < (int)(sizeof(_mcc_cc_list)/sizeof(mcc_cc_map_s)); i++) {
		if (_mcc_cc_list[i].mcc == mcc) {
			_cc = _mcc_cc_list[i].cc;
			break;
		}
	}

	if (0 == _cc) {
		ERR("No data for current mcc(%s)", mcc);
		free(temp);
		return PHONE_NUMBER_ERROR_NO_DATA;
	}

	if (out_cc) {
		*out_cc = _cc;
		DBG("cc=[%d]", *out_cc);
	}

	free(temp);
	return PHONE_NUMBER_ERROR_NONE;
}

static int _phn_server_load_feature_list(void)
{
	int ret;
	bool telephony_feature = false;

	ret = system_info_get_platform_bool(PHN_FEATURE_TELEPHONY, &telephony_feature);
	if (SYSTEM_INFO_ERROR_NONE != ret) {
		ERR("system_info_get_platform_bool() Fail(%d)", ret);
		return PHONE_NUMBER_ERROR_SYSTEM;
	}

	if (false == telephony_feature)
		_phn_have_telephony_feature = 0;
	else
		_phn_have_telephony_feature = 1;

	return PHONE_NUMBER_ERROR_NONE;
}

int phn_get_normalized_number(const char *number, char **out_e164)
{
	int ret;
	int cc = 0;
	PhoneNumber pn;
	string number_e164;
	string region_code;
	const PhoneNumberUtil& pnu = *PhoneNumberUtil::GetInstance();

	if (-1 == _phn_have_telephony_feature) {
		ret = _phn_server_load_feature_list();
		if (PHONE_NUMBER_ERROR_NONE != ret) {
			ERR("_phn_server_load_feature_list() Fail(%d)", ret);
			return PHONE_NUMBER_ERROR_SYSTEM;
		}
	}

	if (0 == _phn_have_telephony_feature) {
		ERR("Telephony feature disabled");
		return PHONE_NUMBER_ERROR_NOT_SUPPORTED;
	}

	ret = _phn_get_cc(false, &cc);
	if (PHONE_NUMBER_ERROR_NONE != ret) {
		ERR("_phn_get_cc() Fail(%d)", ret);
		return ret;
	}
	pnu.GetRegionCodeForCountryCode(cc, &region_code);

	DBG("cc=[%d], number=[%s], region_code=[%s]", cc, number, region_code.c_str());

	const PhoneNumberUtil::ErrorType status = pnu.Parse(number, region_code, &pn);
	if (PhoneNumberUtil::NO_PARSING_ERROR != status) {
		ERR("pnu.Parse() Fail(%d), cc(%d), number(%s), region_code(%s)",
				status, cc, number, region_code.c_str());
		return PHONE_NUMBER_ERROR_SYSTEM;
	}

	pnu.Format(pn, PhoneNumberUtil::E164, &number_e164);

	*out_e164 = g_strdup((gchar *)number_e164.c_str());
	DBG("number_e164=[%s]", *out_e164);

	return PHONE_NUMBER_ERROR_NONE;

}


