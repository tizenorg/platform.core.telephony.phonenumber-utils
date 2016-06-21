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
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <glib.h>
#include <tzplatform_config.h>

#include "phnd.h"
#include "phnd-region-data.h"
#include "phnd-location.h"

#define PHN_LOCATION_DIR tzplatform_mkpath(TZ_SYS_DATA, "phonenumber-utils")
#define PHN_LOCATION_FILE_PREFIX "location"

#define PHN_LOCATION_CHINA_MOBILE_SUFFIX_OFFSET 10000
#define PHN_LOCATION_CHINA_MOBILE_PREFIX_LEN 3
#define PHN_LOCATION_CHINA_MOBILE_SUFFIX_LEN 4
#define PHN_LOCATION_CHINA_MOBILE_NUMBER_MIN_LEN 7

struct phn_location_header {
	gunichar2 version[16];
	gunichar2 version_date[16];
	int province_count;
	int telephone_city_count;
	int mobile_city_count;
	int mobile_prefix_index_count;
	int province_name_len[3];
	int province_id_len;
	int telephone_number_len;
	int telephone_city_len[3];
	int mobile_city_len[3];
	int mobile_prefix_len;
};

int phn_location_find_extra_data(char *region_str, char **p_location_file)
{
	DIR *dirp = NULL;
	struct dirent **dir_list;
	char *location_file = NULL;

	dirp = opendir(PHN_LOCATION_DIR);
	if (NULL == dirp) {
		//LCOV_EXCL_START
		ERR("opendir() return NULL");
		return PHONE_NUMBER_ERROR_NO_DATA;
		//LCOV_EXCL_STOP
	}

	int count = scandir(PHN_LOCATION_DIR, &dir_list, 0, alphasort);
	if (count) {
		int idx = 0;
		char location_prefix[PHN_STR_SHORT_LEN] = {0};

		snprintf(location_prefix, sizeof(location_prefix), "%s-%s",
				PHN_LOCATION_FILE_PREFIX, region_str);
		while (idx != count) {
			const char *file_name = dir_list[idx]->d_name;
			if (0 == strncmp(file_name, location_prefix, strlen(location_prefix))) {
				location_file = g_strdup(file_name);
				break;
			}
			idx++;
		}
	}
	closedir(dirp);

	if (location_file) {
		*p_location_file = location_file;
		return PHONE_NUMBER_ERROR_NONE;
	}

	return PHONE_NUMBER_ERROR_NO_DATA;
}

int phn_location_get_location_from_extra_data(const char *file, const char *number,
		char *region_str, char *lang_str, char **p_location)
{
	int ret = 0;
	int city_str_len = 0;
	int province_str_len = 0;
	gchar *city_temp = NULL;
	gchar *province_temp = NULL;
	const char *real_number = number;
	const gunichar2 *city_str = NULL;
	const gunichar2 *province_str = NULL;
	char file_path[PHN_STR_SHORT_LEN] = {0};

	/* support region - CN, support lang - zh,en,ko */

	RETV_IF(NULL == number, PHONE_NUMBER_ERROR_INVALID_PARAMETER);
	RETV_IF(NULL == lang_str, PHONE_NUMBER_ERROR_INVALID_PARAMETER);
	RETV_IF(NULL == region_str, PHONE_NUMBER_ERROR_INVALID_PARAMETER);

	while (real_number && real_number[0] == '0')
		real_number++;

	char lang_region[PHN_STR_SHORT_LEN] = {0};
	snprintf(lang_region, sizeof(lang_region), "%s_%s", lang_str, region_str);

	int lang_index = 0;
	if (STRING_EQUAL == strcmp(lang_region, PHN_LOCATION_SUPPORT_ZH_CN)) {
		lang_index = 0;
	} else if (STRING_EQUAL == strcmp(lang_region, PHN_LOCATION_SUPPORT_EN_CN)) {
		lang_index = 1;
	} else if (STRING_EQUAL == strcmp(lang_region, PHN_LOCATION_SUPPORT_KO_CN)) {
		lang_index = 2;
	} else {
		//LCOV_EXCL_START
		ERR("Not supported(%s)", lang_region);
		return PHONE_NUMBER_ERROR_NOT_SUPPORTED;
		//LCOV_EXCL_STOP
	}

	snprintf(file_path, sizeof(file_path), "%s/%s", PHN_LOCATION_DIR, file);

	int fd = open(file_path, O_RDONLY);
	RETVM_IF(fd < 0, PHONE_NUMBER_ERROR_NOT_SUPPORTED, "open() Fail(%d)", errno);

	ret = lseek(fd, sizeof(int), SEEK_CUR); /* start_mark */
	if (ret <= 0) {
		//LCOV_EXCL_START
		ERR("lseek() Fail(%d)", errno);
		close(fd);
		return PHONE_NUMBER_ERROR_NOT_SUPPORTED;
		//LCOV_EXCL_STOP
	}

	struct phn_location_header header;
	ret = read(fd, &header, sizeof(struct phn_location_header));
	if (ret <= 0) {
		//LCOV_EXCL_START
		ERR("read() Fail(%d)", errno);
		close(fd);
		return PHONE_NUMBER_ERROR_NOT_SUPPORTED;
		//LCOV_EXCL_STOP
	}

	struct phn_province_info {
		gunichar2 name1[header.province_name_len[0]/2];
		gunichar2 name2[header.province_name_len[1]/2];
		gunichar2 name3[header.province_name_len[2]/2];
	};

#pragma pack(1)
	struct phn_telephone_city_info {
		gint8 province_index;
		gunichar2 city1[header.telephone_city_len[0]/2];
		gunichar2 city2[header.telephone_city_len[1]/2];
		gunichar2 city3[header.telephone_city_len[2]/2];
		gint16 prefix;
	};

#pragma pack(1)
	struct phn_mobile_city_info {
		gint8 province_index;
		gunichar2 city1[header.mobile_city_len[0]/2];
		gunichar2 city2[header.mobile_city_len[1]/2];
		gunichar2 city3[header.mobile_city_len[2]/2];
	};

	if (header.province_count <= 0) {
		//LCOV_EXCL_START
		ERR("Invalid provice_count(%d)", header.province_count);
		close(fd);
		return PHONE_NUMBER_ERROR_NOT_SUPPORTED;
		//LCOV_EXCL_STOP
	}
	struct phn_province_info province_info[header.province_count];
	ret = read(fd, &province_info, sizeof(province_info));
	if (ret <= 0) {
		//LCOV_EXCL_START
		ERR("read() Fail(%d)", errno);
		close(fd);
		return PHONE_NUMBER_ERROR_NOT_SUPPORTED;
		//LCOV_EXCL_STOP
	}

	if (header.telephone_city_count <= 0) {
		//LCOV_EXCL_START
		ERR("Invalid telephone_city_count(%d)", header.telephone_city_count);
		close(fd);
		return PHONE_NUMBER_ERROR_NOT_SUPPORTED;
		//LCOV_EXCL_STOP
	}
	struct phn_telephone_city_info telephone_city_info[header.telephone_city_count];
	ret = read(fd, &telephone_city_info, sizeof(telephone_city_info));
	if (ret <= 0) {
		//LCOV_EXCL_START
		ERR("read() Fail(%d)", errno);
		close(fd);
		return PHONE_NUMBER_ERROR_NOT_SUPPORTED;
		//LCOV_EXCL_STOP
	}

	int i;
	int telephone_city_count =
		sizeof(telephone_city_info) / sizeof(struct phn_telephone_city_info);
	for (i = 0; i < telephone_city_count; i++) {
		gint8 provice_idx;
		gint16 prefix = telephone_city_info[i].prefix;
		char prefix_str[PHN_STR_SHORT_LEN] = {0};

		snprintf(prefix_str, sizeof(prefix_str), "%u", prefix);
		if (0 == strncmp(real_number, prefix_str, strlen(prefix_str))) {
			switch (lang_index) {
			case 0:
				city_str = telephone_city_info[i].city1;
				city_str_len = header.telephone_city_len[0] / 2;
				break;
			case 1:
				city_str = telephone_city_info[i].city2;
				city_str_len = header.telephone_city_len[1] / 2;
				break;
			case 2:
				city_str = telephone_city_info[i].city3;
				city_str_len = header.telephone_city_len[2] / 2;
				break;
			default:
				//LCOV_EXCL_START
				ERR("Invalid lang_index(%d)", lang_index);
				break;
				//LCOV_EXCL_STOP
			}

			provice_idx = telephone_city_info[i].province_index;
			if (0 < provice_idx && provice_idx <= header.province_count) {
				switch (lang_index) {
				case 0:
					province_str = province_info[provice_idx-1].name1;
					province_str_len = header.province_name_len[0] / 2;
					break;
				case 1:
					province_str = province_info[provice_idx-1].name2;
					province_str_len = header.province_name_len[1] / 2;
					break;
				case 2:
					province_str = province_info[provice_idx-1].name3;
					province_str_len = header.province_name_len[2] / 2;
					break;
				default:
					//LCOV_EXCL_START
					ERR("Invalid lang_index(%d)", lang_index);
					break;
					//LCOV_EXCL_STOP
				}
			}

			province_temp = g_utf16_to_utf8(province_str, province_str_len, NULL, NULL,
					NULL);
			city_temp = g_utf16_to_utf8(city_str, city_str_len, NULL, NULL, NULL);

			if (city_temp && province_temp) {
				int size = strlen(city_temp) + strlen(province_temp);
				char *location = NULL;
				location = calloc(size + 3, sizeof(char));
				if (location)
					snprintf(location, size + 3, "%s, %s", city_temp, province_temp);
				*p_location = location;
			} else if (city_temp) {
				int size = strlen(city_temp);
				char *location = NULL;
				location = calloc(size + 1, sizeof(char));
				if (location)
					snprintf(location, size + 1, "%s", city_temp);
				*p_location = location;
			}

			close(fd);
			free(city_temp);
			free(province_temp);
			if (NULL == p_location)
				return PHONE_NUMBER_ERROR_NOT_SUPPORTED;
			return PHONE_NUMBER_ERROR_NONE;
		}
	}

	if (strlen(number) < PHN_LOCATION_CHINA_MOBILE_NUMBER_MIN_LEN) {
		//LCOV_EXCL_START
		ERR("Invalid number(%s)", number);
		close(fd);
		return PHONE_NUMBER_ERROR_INVALID_PARAMETER;
		//LCOV_EXCL_STOP
	}

	if (header.mobile_city_count <= 0) {
		//LCOV_EXCL_START
		ERR("Invalid mobile_city_count(%d)", header.mobile_city_count);
		close(fd);
		return PHONE_NUMBER_ERROR_NOT_SUPPORTED;
		//LCOV_EXCL_STOP
	}

	struct phn_mobile_city_info mobile_city_info[header.mobile_city_count];
	ret = read(fd, &mobile_city_info, sizeof(mobile_city_info));
	if (ret <= 0) {
		//LCOV_EXCL_START
		ERR("read() Fail(%d)", errno);
		close(fd);
		return PHONE_NUMBER_ERROR_NOT_SUPPORTED;
		//LCOV_EXCL_STOP
	}

	if (header.mobile_prefix_index_count <= 0) {
		//LCOV_EXCL_START
		ERR("Invalid mobile_prefix_index_count(%d)", header.mobile_prefix_index_count);
		close(fd);
		return PHONE_NUMBER_ERROR_NOT_SUPPORTED;
		//LCOV_EXCL_STOP
	}

	gint16 mobile_prefix_info[header.mobile_prefix_index_count];
	ret = read(fd, &mobile_prefix_info, sizeof(mobile_prefix_info));
	if (ret <= 0) {
		//LCOV_EXCL_START
		ERR("read() Fail(%d)", errno);
		close(fd);
		return PHONE_NUMBER_ERROR_NOT_SUPPORTED;
		//LCOV_EXCL_STOP
	}

	char num_prefix_str[PHN_LOCATION_CHINA_MOBILE_PREFIX_LEN + 1] = {0};
	char num_suffix_str[PHN_LOCATION_CHINA_MOBILE_SUFFIX_LEN + 1] = {0};
	snprintf(num_prefix_str, sizeof(num_prefix_str), "%s", number);
	snprintf(num_suffix_str, sizeof(num_suffix_str), "%s", number + 3);
	int num_prefix = atoi(num_prefix_str);
	int num_suffix = atoi(num_suffix_str);

	int mobile_prefix_index_count = sizeof(mobile_prefix_info) / sizeof(gint16);
	for (i = 0; i < mobile_prefix_index_count; i++) {
		if (num_prefix == mobile_prefix_info[i]) {
			gint16 mobile_prefix = 0;
			ret = lseek(fd, (PHN_LOCATION_CHINA_MOBILE_SUFFIX_OFFSET * sizeof(gint16) * i)
					+ (num_suffix * sizeof(gint16)), SEEK_CUR);
			ret = read(fd, &mobile_prefix, sizeof(gint16));
			WARN_IF(ret < 0, "read() Fail(%d)", errno);
			if (0 == mobile_prefix) {
				//LCOV_EXCL_START
				ERR("Invalid value:mobile_prefix is 0");
				continue;
				//LCOV_EXCL_STOP
			}

			switch (lang_index) {
			case 0:
				city_str = mobile_city_info[mobile_prefix - 1].city1;
				city_str_len = header.mobile_city_len[0] / 2;
				break;
			case 1:
				city_str = mobile_city_info[mobile_prefix - 1].city2;
				city_str_len = header.mobile_city_len[1] / 2;
				break;
			case 2:
				city_str = mobile_city_info[mobile_prefix - 1].city3;
				city_str_len = header.mobile_city_len[2] / 2;
				break;
			default:
				//LCOV_EXCL_START
				ERR("Invalid lang_index(%d)", lang_index);
				break;
				//LCOV_EXCL_STOP
			}

			int province_index = mobile_city_info[mobile_prefix - 1].province_index;

			if (0 < province_index && province_index <= header.province_count) {
				switch (lang_index) {
				case 0:
					province_str = province_info[province_index - 1].name1;
					province_str_len = header.province_name_len[0] / 2;
					break;
				case 1:
					province_str = province_info[province_index - 1].name2;
					province_str_len = header.province_name_len[1] / 2;
					break;
				case 2:
					province_str = province_info[province_index - 1].name3;
					province_str_len = header.province_name_len[2] / 2;
					break;
				default:
					//LCOV_EXCL_START
					ERR("Invalid lang_index(%d)", lang_index);
					break;
					//LCOV_EXCL_STOP
				}
			}

			province_temp = g_utf16_to_utf8(province_str, province_str_len, NULL, NULL,
					NULL);
			city_temp = g_utf16_to_utf8(city_str, city_str_len, NULL, NULL, NULL);

			if (city_temp && province_temp) {
				int size = strlen(city_temp) + strlen(province_temp);
				char *location = NULL;
				location = calloc(size + 3, sizeof(char));
				if (location)
					snprintf(location, size + 3, "%s, %s", city_temp, province_temp);
				*p_location = location;
			} else if (city_temp) {
				int size = strlen(city_temp);
				char *location = NULL;
				location = calloc(size + 1, sizeof(char));
				if (location)
					snprintf(location, size + 1, "%s", city_temp);
				*p_location = location;
			}
			close(fd);
			free(city_temp);
			free(province_temp);
			if (NULL == p_location)
				return PHONE_NUMBER_ERROR_NOT_SUPPORTED;
			return PHONE_NUMBER_ERROR_NONE;
		}
	}

	close(fd);
	return PHONE_NUMBER_ERROR_NONE;
}

