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
#include <glib.h>
#include <system_settings.h>

#include "phnd.h"
#include "phnd-region-data.h"

struct phn_lang_info {
	const char *lang_str;
	phone_number_lang_e lang;
};

struct phn_match_info {
	phone_number_region_e region;
	phone_number_lang_e lang;
};

struct phn_region_info {
	const char *region_str;
	phone_number_region_e region;
};

const struct phn_match_info phn_match_info_table[] = {
	{PHONE_NUMBER_REGION_WESTERN_SAHARA, PHONE_NUMBER_LANG_FRENCH},
	{PHONE_NUMBER_REGION_MOROCCO, PHONE_NUMBER_LANG_FRENCH},
	{PHONE_NUMBER_REGION_MAURITANIA, PHONE_NUMBER_LANG_FRENCH},
	{PHONE_NUMBER_REGION_COTE_D_IVOIRE, PHONE_NUMBER_LANG_FRENCH},
	{PHONE_NUMBER_REGION_TOGO, PHONE_NUMBER_LANG_SPANISH},
	{PHONE_NUMBER_REGION_TOGO, PHONE_NUMBER_LANG_FRENCH},
	{PHONE_NUMBER_REGION_BENIN, PHONE_NUMBER_LANG_FRENCH},
	{PHONE_NUMBER_REGION_MAURITIUS, PHONE_NUMBER_LANG_SPANISH},
	{PHONE_NUMBER_REGION_MAURITIUS, PHONE_NUMBER_LANG_FRENCH},
	{PHONE_NUMBER_REGION_CABO_VERDE, PHONE_NUMBER_LANG_PORTUGUESE},
	{PHONE_NUMBER_REGION_SAO_TOME_AND_PRINCIPE, PHONE_NUMBER_LANG_PORTUGUESE},
	{PHONE_NUMBER_REGION_CONGO, PHONE_NUMBER_LANG_FRENCH},
	{PHONE_NUMBER_REGION_DEMOCRATIC_REPUBLIC_OF_THE_CONGO, PHONE_NUMBER_LANG_FRENCH},
	{PHONE_NUMBER_REGION_ANGOLA, PHONE_NUMBER_LANG_PORTUGUESE},
	{PHONE_NUMBER_REGION_GUINEA_BISSAU, PHONE_NUMBER_LANG_PORTUGUESE},
	{PHONE_NUMBER_REGION_MOZAMBIQUE, PHONE_NUMBER_LANG_PORTUGUESE},
	{PHONE_NUMBER_REGION_COMOROS, PHONE_NUMBER_LANG_FRENCH},
	{PHONE_NUMBER_REGION_SAINT_HELENA, PHONE_NUMBER_LANG_FRENCH},
	{PHONE_NUMBER_REGION_TRISTAN_DA_CUNHA, PHONE_NUMBER_LANG_FRENCH},
	{PHONE_NUMBER_REGION_GREECE, PHONE_NUMBER_LANG_GREEK},
	{PHONE_NUMBER_REGION_NETHERLANDS, PHONE_NUMBER_LANG_DUTCH},
	{PHONE_NUMBER_REGION_BELGIUM, PHONE_NUMBER_LANG_GERMAN},
	{PHONE_NUMBER_REGION_BELGIUM, PHONE_NUMBER_LANG_FRENCH},
	{PHONE_NUMBER_REGION_BELGIUM, PHONE_NUMBER_LANG_DUTCH},
	{PHONE_NUMBER_REGION_FRANCE, PHONE_NUMBER_LANG_FRENCH},
	{PHONE_NUMBER_REGION_SPAIN, PHONE_NUMBER_LANG_SPANISH},
	{PHONE_NUMBER_REGION_PORTUGAL, PHONE_NUMBER_LANG_PORTUGUESE},
	{PHONE_NUMBER_REGION_LUXEMBOURG, PHONE_NUMBER_LANG_GERMAN},
	{PHONE_NUMBER_REGION_LUXEMBOURG, PHONE_NUMBER_LANG_FRENCH},
	{PHONE_NUMBER_REGION_ALAND_ISLANDS, PHONE_NUMBER_LANG_FINNISH},
	{PHONE_NUMBER_REGION_ALAND_ISLANDS, PHONE_NUMBER_LANG_NORTHERN_SAMI},
	{PHONE_NUMBER_REGION_FINLAND, PHONE_NUMBER_LANG_FINNISH},
	{PHONE_NUMBER_REGION_BULGARIA, PHONE_NUMBER_LANG_BULGARIAN},
	{PHONE_NUMBER_REGION_HUNGARY, PHONE_NUMBER_LANG_HUNGARIAN},
	{PHONE_NUMBER_REGION_REPUBLIC_OF_MOLDOVA, PHONE_NUMBER_LANG_ROMANIAN},
	{PHONE_NUMBER_REGION_REPUBLIC_OF_MOLDOVA, PHONE_NUMBER_LANG_RUSSIAN},
	{PHONE_NUMBER_REGION_ARMENIA, PHONE_NUMBER_LANG_AMHARIC},
	{PHONE_NUMBER_REGION_ARMENIA, PHONE_NUMBER_LANG_RUSSIAN},
	{PHONE_NUMBER_REGION_BELARUS, PHONE_NUMBER_LANG_BELARUSIAN},
	{PHONE_NUMBER_REGION_BELARUS, PHONE_NUMBER_LANG_RUSSIAN},
	{PHONE_NUMBER_REGION_SERBIA, PHONE_NUMBER_LANG_SERBIAN},
	{PHONE_NUMBER_REGION_ITALY, PHONE_NUMBER_LANG_ITALIAN},
	{PHONE_NUMBER_REGION_ROMANIA, PHONE_NUMBER_LANG_ROMANIAN},
	{PHONE_NUMBER_REGION_SWITZERLAND, PHONE_NUMBER_LANG_GERMAN},
	{PHONE_NUMBER_REGION_SWITZERLAND, PHONE_NUMBER_LANG_FRENCH},
	{PHONE_NUMBER_REGION_SWITZERLAND, PHONE_NUMBER_LANG_ITALIAN},
	{PHONE_NUMBER_REGION_AUSTRIA, PHONE_NUMBER_LANG_GERMAN},
	{PHONE_NUMBER_REGION_SWEDEN, PHONE_NUMBER_LANG_SWEDISH},
	{PHONE_NUMBER_REGION_POLAND, PHONE_NUMBER_LANG_POLISH},
	{PHONE_NUMBER_REGION_GERMANY, PHONE_NUMBER_LANG_GERMAN},
	{PHONE_NUMBER_REGION_BRAZIL, PHONE_NUMBER_LANG_PORTUGUESE},
	{PHONE_NUMBER_REGION_CHILE, PHONE_NUMBER_LANG_SPANISH},
	{PHONE_NUMBER_REGION_COLOMBIA, PHONE_NUMBER_LANG_SPANISH},
	{PHONE_NUMBER_REGION_VENEZUELA, PHONE_NUMBER_LANG_SPANISH},
	{PHONE_NUMBER_REGION_INDONESIA, PHONE_NUMBER_LANG_INDONESIAN},
	{PHONE_NUMBER_REGION_THAILAND, PHONE_NUMBER_LANG_THAI},
	{PHONE_NUMBER_REGION_KAZAKHSTAN, PHONE_NUMBER_LANG_RUSSIAN},
	{PHONE_NUMBER_REGION_RUSSIAN_FEDERATION, PHONE_NUMBER_LANG_RUSSIAN},
	{PHONE_NUMBER_REGION_JAPAN, PHONE_NUMBER_LANG_JAPANESE},
	{PHONE_NUMBER_REGION_REPUBLIC_OF_KOREA, PHONE_NUMBER_LANG_ARABIC},
	{PHONE_NUMBER_REGION_REPUBLIC_OF_KOREA, PHONE_NUMBER_LANG_CZECH},
	{PHONE_NUMBER_REGION_REPUBLIC_OF_KOREA, PHONE_NUMBER_LANG_FRENCH},
	{PHONE_NUMBER_REGION_REPUBLIC_OF_KOREA, PHONE_NUMBER_LANG_KOREAN},
	{PHONE_NUMBER_REGION_REPUBLIC_OF_KOREA, PHONE_NUMBER_LANG_CHINESE},
	{PHONE_NUMBER_REGION_REPUBLIC_OF_KOREA, PHONE_NUMBER_LANG_CHINESE_TRADITIONAL},
	{PHONE_NUMBER_REGION_VIET_NAM, PHONE_NUMBER_LANG_VIETNAMESE},
	{PHONE_NUMBER_REGION_CHINA, PHONE_NUMBER_LANG_CHINESE},
	{PHONE_NUMBER_REGION_CHINA, PHONE_NUMBER_LANG_CHINESE_TRADITIONAL},
	{PHONE_NUMBER_REGION_TAIWAN_PROVINCE_OF_CHINA, PHONE_NUMBER_LANG_CHINESE},
	{PHONE_NUMBER_REGION_TAIWAN_PROVINCE_OF_CHINA, PHONE_NUMBER_LANG_CHINESE_TRADITIONAL},
	{PHONE_NUMBER_REGION_TURKEY, PHONE_NUMBER_LANG_TURKISH},
	{PHONE_NUMBER_REGION_SAUDI_ARABIA, PHONE_NUMBER_LANG_ARABIC},
	{PHONE_NUMBER_REGION_ISLAMIC_REPUBLIC_OF_IRAN, PHONE_NUMBER_LANG_PERSIAN},
	{PHONE_NUMBER_REGION_SYSTEM, PHONE_NUMBER_LANG_SYSTEM},
};

const struct phn_lang_info phn_lang_info_table[] = {
	{"am", PHONE_NUMBER_LANG_AMHARIC},
	{"ar", PHONE_NUMBER_LANG_ARABIC},
	{"be", PHONE_NUMBER_LANG_BELARUSIAN},
	{"bg", PHONE_NUMBER_LANG_BULGARIAN},
	{"zh", PHONE_NUMBER_LANG_CHINESE},
	{"zh_Hant", PHONE_NUMBER_LANG_CHINESE_TRADITIONAL},
	{"cs", PHONE_NUMBER_LANG_CZECH},
	{"nl", PHONE_NUMBER_LANG_DUTCH},
	{"en", PHONE_NUMBER_LANG_ENGLISH},
	{"fi", PHONE_NUMBER_LANG_FINNISH},
	{"fr", PHONE_NUMBER_LANG_FRENCH},
	{"de", PHONE_NUMBER_LANG_GERMAN},
	{"el", PHONE_NUMBER_LANG_GREEK},
	{"hu", PHONE_NUMBER_LANG_HUNGARIAN},
	{"id", PHONE_NUMBER_LANG_INDONESIAN},
	{"it", PHONE_NUMBER_LANG_ITALIAN},
	{"ja", PHONE_NUMBER_LANG_JAPANESE},
	{"ko", PHONE_NUMBER_LANG_KOREAN},
	{"se", PHONE_NUMBER_LANG_NORTHERN_SAMI},
	{"fa", PHONE_NUMBER_LANG_PERSIAN},
	{"pl", PHONE_NUMBER_LANG_POLISH},
	{"pt", PHONE_NUMBER_LANG_PORTUGUESE},
	{"ro", PHONE_NUMBER_LANG_ROMANIAN},
	{"ru", PHONE_NUMBER_LANG_RUSSIAN},
	{"sr", PHONE_NUMBER_LANG_SERBIAN},
	{"se", PHONE_NUMBER_LANG_SPANISH},
	{"sv", PHONE_NUMBER_LANG_SWEDISH},
	{"th", PHONE_NUMBER_LANG_THAI},
	{"tr", PHONE_NUMBER_LANG_TURKISH},
	{"vi", PHONE_NUMBER_LANG_VIETNAMESE},
};

const struct phn_region_info phn_region_info_table[] = {
	{"AC", PHONE_NUMBER_REGION_ASCENSION_ISLAND},
	{"AD", PHONE_NUMBER_REGION_ANDORRA},
	{"AE", PHONE_NUMBER_REGION_UNITED_ARAB_EMIRATES},
	{"AF", PHONE_NUMBER_REGION_AFGHANISTAN},
	{"AG", PHONE_NUMBER_REGION_ATIGUA_AND_BARBUDA},
	{"AI", PHONE_NUMBER_REGION_ANGUILLA},
	{"AL", PHONE_NUMBER_REGION_ALBANIA},
	{"AM", PHONE_NUMBER_REGION_ARMENIA},
	{"AO", PHONE_NUMBER_REGION_ANGOLA},
	{"AR", PHONE_NUMBER_REGION_ARGENTINA},
	{"AS", PHONE_NUMBER_REGION_AMERICAN_SAMOA},
	{"AT", PHONE_NUMBER_REGION_AUSTRIA},
	{"AU", PHONE_NUMBER_REGION_AUSTRALIA},
	{"AW", PHONE_NUMBER_REGION_ARUBA},
	{"AX", PHONE_NUMBER_REGION_ALAND_ISLANDS},
	{"AZ", PHONE_NUMBER_REGION_AZERBAIJAN},
	{"BA", PHONE_NUMBER_REGION_BOSNIA_AND_HERZEGOVINA},
	{"BB", PHONE_NUMBER_REGION_BARBADOS},
	{"BD", PHONE_NUMBER_REGION_BANGLADESH},
	{"BE", PHONE_NUMBER_REGION_BELGIUM},
	{"BF", PHONE_NUMBER_REGION_BURKINA_FASO},
	{"BG", PHONE_NUMBER_REGION_BULGARIA},
	{"BH", PHONE_NUMBER_REGION_BAHRAIN},
	{"BI", PHONE_NUMBER_REGION_BURUNDI},
	{"BJ", PHONE_NUMBER_REGION_BENIN},
	{"BL", PHONE_NUMBER_REGION_SAINT_BARTHELEMY},
	{"BM", PHONE_NUMBER_REGION_BERMUDA},
	{"BN", PHONE_NUMBER_REGION_BRUNEI_DARUSSALAM},
	{"BO", PHONE_NUMBER_REGION_BOLIVIA},
	{"BQ", PHONE_NUMBER_REGION_BONAIRE_SINT_EUSTATIUS_AND_SABA},
	{"BR", PHONE_NUMBER_REGION_BRAZIL},
	{"BS", PHONE_NUMBER_REGION_BAHAMAS},
	{"BT", PHONE_NUMBER_REGION_BHUTAN},
	{"BW", PHONE_NUMBER_REGION_BOTSWANA},
	{"BY", PHONE_NUMBER_REGION_BELARUS},
	{"BZ", PHONE_NUMBER_REGION_BELIZE},
	{"CA", PHONE_NUMBER_REGION_CANADA},
	{"CC", PHONE_NUMBER_REGION_COCOS_ISLANDS},
	{"CD", PHONE_NUMBER_REGION_DEMOCRATIC_REPUBLIC_OF_THE_CONGO},
	{"CF", PHONE_NUMBER_REGION_CENTRAL_AFRICAN_REPUBLIC},
	{"CG", PHONE_NUMBER_REGION_CONGO},
	{"CH", PHONE_NUMBER_REGION_SWITZERLAND},
	{"CI", PHONE_NUMBER_REGION_COTE_D_IVOIRE},
	{"CK", PHONE_NUMBER_REGION_COOK_ISLANDS},
	{"CL", PHONE_NUMBER_REGION_CHILE},
	{"CM", PHONE_NUMBER_REGION_CAMEROON},
	{"CN", PHONE_NUMBER_REGION_CHINA},
	{"CO", PHONE_NUMBER_REGION_COLOMBIA},
	{"CR", PHONE_NUMBER_REGION_COSTA_RICA},
	{"CU", PHONE_NUMBER_REGION_CUBA},
	{"CV", PHONE_NUMBER_REGION_CABO_VERDE},
	{"CW", PHONE_NUMBER_REGION_CURACAO},
	{"CX", PHONE_NUMBER_REGION_CHRISTMAS_ISLANDS},
	{"CY", PHONE_NUMBER_REGION_CYPRUS},
	{"CZ", PHONE_NUMBER_REGION_CZECH_REPUBLIC},
	{"DE", PHONE_NUMBER_REGION_GERMANY},
	{"DJ", PHONE_NUMBER_REGION_DJIBOUTI},
	{"DK", PHONE_NUMBER_REGION_DENMARK},
	{"DM", PHONE_NUMBER_REGION_DOMINICA},
	{"DO", PHONE_NUMBER_REGION_DOMINICAN_REPUBLIC },
	{"DZ", PHONE_NUMBER_REGION_ALGERIA},
	{"EC", PHONE_NUMBER_REGION_ECUADOR},
	{"EE", PHONE_NUMBER_REGION_ESTONIA},
	{"EG", PHONE_NUMBER_REGION_EGYPT},
	{"EH", PHONE_NUMBER_REGION_WESTERN_SAHARA},
	{"ER", PHONE_NUMBER_REGION_ERITREA},
	{"ES", PHONE_NUMBER_REGION_SPAIN},
	{"ET", PHONE_NUMBER_REGION_ETHIOPIA},
	{"FI", PHONE_NUMBER_REGION_FINLAND},
	{"FJ", PHONE_NUMBER_REGION_FIJI},
	{"FK", PHONE_NUMBER_REGION_FALKLAND_ISLANDS},
	{"FM", PHONE_NUMBER_REGION_MICRONESIA},
	{"FO", PHONE_NUMBER_REGION_FAROE_ISLANDS},
	{"FR", PHONE_NUMBER_REGION_FRANCE},
	{"GA", PHONE_NUMBER_REGION_GABON},
	{"GB", PHONE_NUMBER_REGION_UNITED_KINGDOM},
	{"GD", PHONE_NUMBER_REGION_GRENADA},
	{"GE", PHONE_NUMBER_REGION_GEORGIA},
	{"GF", PHONE_NUMBER_REGION_FRENCH_GUIANA},
	{"GG", PHONE_NUMBER_REGION_GUERNSEY},
	{"GH", PHONE_NUMBER_REGION_GHANA},
	{"GI", PHONE_NUMBER_REGION_GIBRALTAR},
	{"GL", PHONE_NUMBER_REGION_GREENLAND},
	{"GM", PHONE_NUMBER_REGION_GAMBIA},
	{"GN", PHONE_NUMBER_REGION_GUINEA},
	{"GP", PHONE_NUMBER_REGION_GUADELOUPE},
	{"GQ", PHONE_NUMBER_REGION_EQUATORIAL_GUINEA},
	{"GR", PHONE_NUMBER_REGION_GREECE},
	{"GT", PHONE_NUMBER_REGION_GUATEMALA},
	{"GU", PHONE_NUMBER_REGION_GUAM},
	{"GW", PHONE_NUMBER_REGION_GUINEA_BISSAU},
	{"GY", PHONE_NUMBER_REGION_GUYANA},
	{"HK", PHONE_NUMBER_REGION_HONG_KONG},
	{"HN", PHONE_NUMBER_REGION_HONDURAS},
	{"HR", PHONE_NUMBER_REGION_CROATIA},
	{"HT", PHONE_NUMBER_REGION_HAITI},
	{"HU", PHONE_NUMBER_REGION_HUNGARY},
	{"ID", PHONE_NUMBER_REGION_INDONESIA},
	{"IE", PHONE_NUMBER_REGION_IRELAND},
	{"IL", PHONE_NUMBER_REGION_ISRAEL},
	{"IM", PHONE_NUMBER_REGION_ISLE_OF_MAN},
	{"IN", PHONE_NUMBER_REGION_INDIA},
	{"IO", PHONE_NUMBER_REGION_DIEGO_GARCIA},
	{"IQ", PHONE_NUMBER_REGION_IRAQ},
	{"IR", PHONE_NUMBER_REGION_ISLAMIC_REPUBLIC_OF_IRAN},
	{"IS", PHONE_NUMBER_REGION_ICELAND},
	{"IT", PHONE_NUMBER_REGION_ITALY},
	{"JE", PHONE_NUMBER_REGION_JERSEY},
	{"JM", PHONE_NUMBER_REGION_JAMAICA},
	{"JO", PHONE_NUMBER_REGION_JORDAN},
	{"JP", PHONE_NUMBER_REGION_JAPAN},
	{"KE", PHONE_NUMBER_REGION_KENYA},
	{"KG", PHONE_NUMBER_REGION_KYRGYZSTAN},
	{"KH", PHONE_NUMBER_REGION_CAMBODIA},
	{"KI", PHONE_NUMBER_REGION_KIRIBATI},
	{"KM", PHONE_NUMBER_REGION_COMOROS},
	{"KN", PHONE_NUMBER_REGION_SAINT_KITTS_AND_NEVIS},
	{"KP", PHONE_NUMBER_REGION_NORTH_KOREA},
	{"KR", PHONE_NUMBER_REGION_REPUBLIC_OF_KOREA},
	{"KW", PHONE_NUMBER_REGION_KUWAIT},
	{"KY", PHONE_NUMBER_REGION_CAYMAN_ISLAND},
	{"KZ", PHONE_NUMBER_REGION_KAZAKHSTAN},
	{"LA", PHONE_NUMBER_REGION_LAOS},
	{"LB", PHONE_NUMBER_REGION_LEBANON},
	{"LC", PHONE_NUMBER_REGION_SAINT_LUCIA},
	{"LI", PHONE_NUMBER_REGION_LIECHTENSTEIN},
	{"LK", PHONE_NUMBER_REGION_SRI_LANKA},
	{"LR", PHONE_NUMBER_REGION_LIBERIA},
	{"LS", PHONE_NUMBER_REGION_LESOTHO},
	{"LT", PHONE_NUMBER_REGION_LITHUANIA },
	{"LU", PHONE_NUMBER_REGION_LUXEMBOURG},
	{"LV", PHONE_NUMBER_REGION_LATVIA},
	{"LY", PHONE_NUMBER_REGION_LIBYA},
	{"MA", PHONE_NUMBER_REGION_MOROCCO},
	{"MC", PHONE_NUMBER_REGION_MONACO},
	{"MD", PHONE_NUMBER_REGION_REPUBLIC_OF_MOLDOVA},
	{"ME", PHONE_NUMBER_REGION_MONTENEGRO},
	{"MF", PHONE_NUMBER_REGION_SAINT_MARTIN},
	{"MG", PHONE_NUMBER_REGION_MADAGASCAR},
	{"MH", PHONE_NUMBER_REGION_MARSHALL_ISLANDS},
	{"MK", PHONE_NUMBER_REGION_THE_FORMER_YUGOSLAV_REPUBLIC_OF_MACEDONIA},
	{"ML", PHONE_NUMBER_REGION_MALI},
	{"MM", PHONE_NUMBER_REGION_MYANMAR},
	{"MN", PHONE_NUMBER_REGION_MONGOLIA},
	{"MO", PHONE_NUMBER_REGION_MACAO},
	{"MP", PHONE_NUMBER_REGION_NORTHERN_MARIANA_ISLANDS},
	{"MQ", PHONE_NUMBER_REGION_MARTINIQUE},
	{"MR", PHONE_NUMBER_REGION_MAURITANIA},
	{"MS", PHONE_NUMBER_REGION_MONTSERRAT},
	{"MT", PHONE_NUMBER_REGION_MALTA},
	{"MU", PHONE_NUMBER_REGION_MAURITIUS },
	{"MV", PHONE_NUMBER_REGION_MALDIVES},
	{"MW", PHONE_NUMBER_REGION_MALAWI},
	{"MX", PHONE_NUMBER_REGION_MEXICO},
	{"MY", PHONE_NUMBER_REGION_MALAYSIA},
	{"MZ", PHONE_NUMBER_REGION_MOZAMBIQUE},
	{"NA", PHONE_NUMBER_REGION_NAMIBIA},
	{"NC", PHONE_NUMBER_REGION_NEW_CALEDONIA},
	{"NE", PHONE_NUMBER_REGION_NIGER},
	{"NF", PHONE_NUMBER_REGION_NORFOLK_ISLAND},
	{"NG", PHONE_NUMBER_REGION_NIGERIA},
	{"NI", PHONE_NUMBER_REGION_NICARAGUA},
	{"NL", PHONE_NUMBER_REGION_NETHERLANDS},
	{"NO", PHONE_NUMBER_REGION_NORWAY},
	{"NP", PHONE_NUMBER_REGION_NEPAL},
	{"NR", PHONE_NUMBER_REGION_NAURU},
	{"NU", PHONE_NUMBER_REGION_NIUE},
	{"NZ", PHONE_NUMBER_REGION_NEW_ZEALAND},
	{"OM", PHONE_NUMBER_REGION_OMAN},
	{"PA", PHONE_NUMBER_REGION_PANAMA},
	{"PE", PHONE_NUMBER_REGION_PERU},
	{"PF", PHONE_NUMBER_REGION_FRENCH_POLYNESIA},
	{"PG", PHONE_NUMBER_REGION_PAPUA_NEW_GUINEA},
	{"PH", PHONE_NUMBER_REGION_PHILIPPINES},
	{"PK", PHONE_NUMBER_REGION_PAKISTAN},
	{"PL", PHONE_NUMBER_REGION_POLAND},
	{"PM", PHONE_NUMBER_REGION_SAINT_PIERRE_AND_MIQUELON},
	{"PR", PHONE_NUMBER_REGION_PUERTO_RICO},
	{"PS", PHONE_NUMBER_REGION_PALESTINIAN_AUTHORITY},
	{"PT", PHONE_NUMBER_REGION_PORTUGAL},
	{"PW", PHONE_NUMBER_REGION_PALAU},
	{"PY", PHONE_NUMBER_REGION_PARAGUAY},
	{"QA", PHONE_NUMBER_REGION_QATAR},
	{"RE", PHONE_NUMBER_REGION_REUNION},
	{"RO", PHONE_NUMBER_REGION_ROMANIA},
	{"RS", PHONE_NUMBER_REGION_SERBIA},
	{"RU", PHONE_NUMBER_REGION_RUSSIAN_FEDERATION},
	{"RW", PHONE_NUMBER_REGION_RWANDA},
	{"SA", PHONE_NUMBER_REGION_SAUDI_ARABIA},
	{"SB", PHONE_NUMBER_REGION_SOLOMON_ISLANDS},
	{"SC", PHONE_NUMBER_REGION_SEYCHELLES},
	{"SD", PHONE_NUMBER_REGION_SUDAN},
	{"SE", PHONE_NUMBER_REGION_SWEDEN},
	{"SG", PHONE_NUMBER_REGION_SINGAPORE},
	{"SH", PHONE_NUMBER_REGION_SAINT_HELENA},
	{"SI", PHONE_NUMBER_REGION_SLOVENIA},
	{"SJ", PHONE_NUMBER_REGION_SVALBARD_AND_JAN_MAYEN},
	{"SK", PHONE_NUMBER_REGION_SLOVAKIA},
	{"SL", PHONE_NUMBER_REGION_SIERRA_LEONE},
	{"SM", PHONE_NUMBER_REGION_SAN_MARINO},
	{"SN", PHONE_NUMBER_REGION_SENEGAL},
	{"SO", PHONE_NUMBER_REGION_SOMALIA},
	{"SR", PHONE_NUMBER_REGION_SURINAME},
	{"SS", PHONE_NUMBER_REGION_SOUTH_SUDAN},
	{"ST", PHONE_NUMBER_REGION_SAO_TOME_AND_PRINCIPE},
	{"SV", PHONE_NUMBER_REGION_EL_SALVADOR},
	{"SX", PHONE_NUMBER_REGION_SINT_MAARTEN},
	{"SY", PHONE_NUMBER_REGION_SYRIAN_ARAB_REPUBLIC},
	{"SZ", PHONE_NUMBER_REGION_SWAZILAND},
	{"TA", PHONE_NUMBER_REGION_TRISTAN_DA_CUNHA},
	{"TC", PHONE_NUMBER_REGION_TURKS_AND_CAICOS_ISLANDS},
	{"TD", PHONE_NUMBER_REGION_CHAD},
	{"TG", PHONE_NUMBER_REGION_TOGO},
	{"TH", PHONE_NUMBER_REGION_THAILAND},
	{"TJ", PHONE_NUMBER_REGION_TAJIKISTAN},
	{"TK", PHONE_NUMBER_REGION_TOKELAU},
	{"TL", PHONE_NUMBER_REGION_TIMOR_LESTE},
	{"TM", PHONE_NUMBER_REGION_TURKMENISTAN},
	{"TN", PHONE_NUMBER_REGION_TUNISIA},
	{"TO", PHONE_NUMBER_REGION_TONGA},
	{"TR", PHONE_NUMBER_REGION_TURKEY},
	{"TT", PHONE_NUMBER_REGION_TRINIDAD_AND_TOBAGO},
	{"TV", PHONE_NUMBER_REGION_TUVALU},
	{"TW", PHONE_NUMBER_REGION_TAIWAN_PROVINCE_OF_CHINA},
	{"TZ", PHONE_NUMBER_REGION_TANZANIA},
	{"UA", PHONE_NUMBER_REGION_UKRAINE},
	{"UG", PHONE_NUMBER_REGION_UGANDA},
	{"US", PHONE_NUMBER_REGION_UNITED_STATES_OF_AMERICA},
	{"UY", PHONE_NUMBER_REGION_URUGUAY},
	{"UZ", PHONE_NUMBER_REGION_UZBEKISTAN},
	{"VA", PHONE_NUMBER_REGION_VATICAN_CITY},
	{"VC", PHONE_NUMBER_REGION_SAINT_VINCENT_AND_THE_GRENADINES},
	{"VE", PHONE_NUMBER_REGION_VENEZUELA},
	{"VG", PHONE_NUMBER_REGION_VIRGIN_ISLAND_BRITISH},
	{"VI", PHONE_NUMBER_REGION_VIRGIN_ISLAND_US},
	{"VN", PHONE_NUMBER_REGION_VIET_NAM},
	{"VU", PHONE_NUMBER_REGION_VANUATU},
	{"WF", PHONE_NUMBER_REGION_WALLIS_AND_FUTUNA},
	{"WS", PHONE_NUMBER_REGION_SAMOA},
	{"YE", PHONE_NUMBER_REGION_YEMEN},
	{"YT", PHONE_NUMBER_REGION_MAYOTTE},
	{"ZA", PHONE_NUMBER_REGION_SOUTH_AFRICA},
	{"ZM", PHONE_NUMBER_REGION_ZAMBIA},
	{"ZW", PHONE_NUMBER_REGION_ZIMBABWE},
};

int phn_region_data_get_region_str(phone_number_region_e region, char **region_str)
{
	int ret, i;

	if (PHONE_NUMBER_REGION_SYSTEM == region) {
		char *str = NULL;
		ret = system_settings_get_value_string(SYSTEM_SETTINGS_KEY_LOCALE_COUNTRY, &str);
		if (str)
			*region_str = g_strdup(strchr(str, '_') + 1);
		free(str);
		if (SYSTEM_SETTINGS_ERROR_NONE != ret) {
			//LCOV_EXCL_START
			ERR("system_settings_get_value_string() Fail(%d)", ret);
			return PHONE_NUMBER_ERROR_NOT_SUPPORTED;
			//LCOV_EXCL_STOP
		}
		return PHONE_NUMBER_ERROR_NONE;
	}

	for (i = 0; i < sizeof(phn_region_info_table)/sizeof(struct phn_region_info); i++) {
		if (phn_region_info_table[i].region == region) {
			*region_str = g_strdup(phn_region_info_table[i].region_str);
			return PHONE_NUMBER_ERROR_NONE;
		}
	}
	return PHONE_NUMBER_ERROR_NOT_SUPPORTED;
}

int phn_region_data_get_lang_str(phone_number_lang_e lang, char **lang_str)
{
	int ret, i;

	if (PHONE_NUMBER_LANG_SYSTEM == lang) {
		char *str = NULL;
		char *last;
		ret = system_settings_get_value_string(SYSTEM_SETTINGS_KEY_LOCALE_LANGUAGE, &str);
		if (str)
			*lang_str = g_strdup(strtok_r(str, "_", &last));
		free(str);
		if (SYSTEM_SETTINGS_ERROR_NONE != ret) {
			//LCOV_EXCL_START
			ERR("system_settings_get_value_string() Fail(%d)", ret);
			return PHONE_NUMBER_ERROR_NOT_SUPPORTED;
			//LCOV_EXCL_STOP
		}
		return PHONE_NUMBER_ERROR_NONE;
	}

	for (i = 0; i < sizeof(phn_lang_info_table)/sizeof(struct phn_lang_info); i++) {
		if (phn_lang_info_table[i].lang == lang) {
			*lang_str = g_strdup(phn_lang_info_table[i].lang_str);
			return PHONE_NUMBER_ERROR_NONE;
		}
	}
	return PHONE_NUMBER_ERROR_NOT_SUPPORTED;
}

bool phn_region_data_find_match_info(phone_number_region_e region,
		phone_number_lang_e lang)
{
	int i;
	for (i = 0; i < sizeof(phn_match_info_table)/sizeof(struct phn_match_info); i++) {
		if (phn_match_info_table[i].region == region
				&& phn_match_info_table[i].lang == lang) {
			return true;
		}
	}
	return false;
}
