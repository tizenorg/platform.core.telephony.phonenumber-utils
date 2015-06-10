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

#include "phn_common.h"
#include "phn_region_data.h"
#include "phn_location.h"

#define PHN_PATH_MAX_LEN 1024
#define PHN_COPY_SIZE_MAX 4096

#define PHN_LOCATION_DIR "/opt/usr/data/phonenumber-utils"
#define PHN_LOCATION_DOWNLOAD_DIR "/opt/usr/data/phonenumber-utils/downloads"
#define PHN_LOCATION_FILE_PREFIX "location"
#define PHN_LOCATION_LANG_INDEX_MAX 3
#define PHN_LOCATION_FILE_MARK 0xfefe0000

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

int phn_location_find_extra_data(const char *region_str, char **p_location_file)
{
	DIR *dirp = NULL;
	struct dirent **dir_list;
	char *location_file = NULL;

	dirp = opendir(PHN_LOCATION_DIR);
	if (NULL == dirp) {
		ERR("opendir() return NULL");
		return PHONE_NUMBER_ERROR_NO_DATA;
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
				location_file = strdup(file_name);
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
		phone_number_region_e region, phone_number_lang_e lang, char **p_location)
{
	int ret = 0;
	int city_str_len = 0;
	int province_str_len = 0;
	size_t read_size;
	gchar *city_temp = NULL;
	gchar *province_temp = NULL;
	const char *real_number = number;
	const gunichar2 *city_str = NULL;
	const gunichar2 *province_str = NULL;
	char file_path[PHN_STR_SHORT_LEN] = {0};

	/* support region - CN, support lang - zh,en,ko */
	RETVM_IF(region != PHONE_NUMBER_REGION_CHINA, PHONE_NUMBER_ERROR_NOT_SUPPORTED,
			"Not supported region(%d)", region);

	int lang_index = 0;
	switch (lang) {
	case PHONE_NUMBER_LANG_CHINESE:
		lang_index = 0;
		break;
	case PHONE_NUMBER_LANG_ENGLISH:
		lang_index = 1;
		break;
	case PHONE_NUMBER_LANG_KOREAN:
		lang_index = 2;
		break;
	default:
		ERR("Not supported lang(%d)", lang);
		return PHONE_NUMBER_ERROR_NOT_SUPPORTED;
	}

	while (real_number && real_number[0] == '0')
		real_number++;
	RETVM_IF(NULL == real_number, PHONE_NUMBER_ERROR_INVALID_PARAMETER, "number=%s",
			number);

	snprintf(file_path, sizeof(file_path), "%s/%s", PHN_LOCATION_DIR, file);

	int fd = open(file_path, O_RDONLY);
	RETVM_IF(fd < 0, PHONE_NUMBER_ERROR_NOT_SUPPORTED, "open() Fail(%d)", errno);

	ret = lseek(fd, sizeof(int), SEEK_CUR); /* start_mark */
	if (ret <= 0) {
		ERR("lseek() Fail(%d)", errno);
		close(fd);
		return PHONE_NUMBER_ERROR_NOT_SUPPORTED;
	}

	struct phn_location_header header;
	ret = read(fd, &header, sizeof(struct phn_location_header));
	if (ret <= 0) {
		ERR("read() Fail(%d)", errno);
		close(fd);
		return PHONE_NUMBER_ERROR_NOT_SUPPORTED;
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

	struct phn_province_info province_info[header.province_count];
	ret = read(fd, &province_info,
			sizeof(struct phn_province_info)*header.province_count);
	if (ret <= 0) {
		ERR("read() Fail(%d)", errno);
		close(fd);
		return PHONE_NUMBER_ERROR_NOT_SUPPORTED;
	}

	struct phn_telephone_city_info telephone_city_info[header.telephone_city_count];
	ret = read(fd, &telephone_city_info,
			sizeof(struct phn_telephone_city_info)*header.telephone_city_count);
	if (ret <= 0) {
		ERR("read() Fail(%d)", errno);
		close(fd);
		return PHONE_NUMBER_ERROR_NOT_SUPPORTED;
	}

	int i;
	for (i = 0; i < header.telephone_city_count; i++) {
		gint8 provice_idx;
		gint16 prefix = telephone_city_info[i].prefix;
		char prefix_str[PHN_STR_SHORT_LEN] = {0};

		snprintf(prefix_str, sizeof(prefix_str), "%u", prefix);
		if (0 == strncmp(real_number, prefix_str, strlen(prefix_str))) {
			switch (lang_index) {
			case 0:
				city_str = telephone_city_info[i].city1;
				city_str_len = header.telephone_city_len[0]/2;
				break;
			case 1:
				city_str = telephone_city_info[i].city2;
				city_str_len = header.telephone_city_len[1]/2;
				break;
			case 2:
				city_str = telephone_city_info[i].city3;
				city_str_len = header.telephone_city_len[2]/2;
				break;
			default:
				ERR("Invalid lang_index(%d)", lang_index);
				break;
			}

			provice_idx = telephone_city_info[i].province_index;
			if (0 < provice_idx && provice_idx <= header.province_count) {
				switch (lang_index) {
				case 0:
					province_str = province_info[provice_idx-1].name1;
					province_str_len = header.province_name_len[0]/2;
					break;
				case 1:
					province_str = province_info[provice_idx-1].name2;
					province_str_len = header.province_name_len[1]/2;
					break;
				case 2:
					province_str = province_info[provice_idx-1].name3;
					province_str_len = header.province_name_len[2]/2;
					break;
				default:
					ERR("Invalid lang_index(%d)", lang_index);
					break;
				}
			}

			province_temp = g_utf16_to_utf8(province_str, province_str_len, NULL, NULL,
					NULL);
			city_temp = g_utf16_to_utf8(city_str, city_str_len, NULL, NULL, NULL);

			if (city_temp && province_temp) {
				int size = strlen(city_temp) + strlen(province_temp);
				char *location = NULL;
				location = calloc(size+3, sizeof(char));
				if (location)
					snprintf(location, size+3, "%s, %s", city_temp, province_temp);
				*p_location = location;
			} else if (city_temp) {
				int size = strlen(city_temp);
				char *location = NULL;
				location = calloc(size+1, sizeof(char));
				if (location)
					snprintf(location, size+1, "%s", city_temp);
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

	if (region != PHONE_NUMBER_REGION_CHINA) {
		ERR("Invalid region(%d)", region);
		close(fd);
		return PHONE_NUMBER_ERROR_NO_DATA;
	} else if (strlen(number) < PHN_LOCATION_CHINA_MOBILE_NUMBER_MIN_LEN) {
		ERR("Invalid number(%s)", number);
		close(fd);
		return PHONE_NUMBER_ERROR_INVALID_PARAMETER;
	}

	struct phn_mobile_city_info mobile_city_info[header.mobile_city_count];
	read_size = sizeof(struct phn_mobile_city_info)*header.mobile_city_count;
	ret = read(fd, &mobile_city_info, read_size);
	if (ret <= 0) {
		ERR("read() Fail(%d)", errno);
		close(fd);
		return PHONE_NUMBER_ERROR_NOT_SUPPORTED;
	}

	gint16 mobile_prefix_info[header.mobile_prefix_index_count];
	read_size = sizeof(gint16)*header.mobile_prefix_index_count;
	ret = read(fd, &mobile_prefix_info, read_size);
	if (ret <= 0) {
		ERR("read() Fail(%d)", errno);
		close(fd);
		return PHONE_NUMBER_ERROR_NOT_SUPPORTED;
	}

	char num_prefix_str[PHN_LOCATION_CHINA_MOBILE_PREFIX_LEN+1] = {0};
	char num_suffix_str[PHN_LOCATION_CHINA_MOBILE_SUFFIX_LEN+1] = {0};
	snprintf(num_prefix_str, sizeof(num_prefix_str), "%s", number);
	snprintf(num_suffix_str, sizeof(num_suffix_str), "%s", number+3);
	int num_prefix = atoi(num_prefix_str);
	int num_suffix = atoi(num_suffix_str);

	for (i = 0; i < header.mobile_prefix_index_count; i++) {
		if (num_prefix == mobile_prefix_info[i]) {
			gint16 mobile_prefix = 0;
			ret = lseek(fd, (PHN_LOCATION_CHINA_MOBILE_SUFFIX_OFFSET*sizeof(gint16)*i)
					+ (num_suffix*sizeof(gint16)), SEEK_CUR);
			ret = read(fd, &mobile_prefix, sizeof(gint16));
			WARN_IF(ret < 0, "read() Fail(%d)", errno);
			if (0 == mobile_prefix) {
				ERR("Invalid value:mobile_prefix is 0");
				continue;
			}

			switch (lang_index) {
			case 0:
				city_str = mobile_city_info[mobile_prefix-1].city1;
				city_str_len = header.mobile_city_len[0]/2;
				break;
			case 1:
				city_str = mobile_city_info[mobile_prefix-1].city2;
				city_str_len = header.mobile_city_len[1]/2;
				break;
			case 2:
				city_str = mobile_city_info[mobile_prefix-1].city3;
				city_str_len = header.mobile_city_len[2]/2;
				break;
			default:
				ERR("Invalid lang_index(%d)", lang_index);
				break;
			}

			int province_index = mobile_city_info[mobile_prefix-1].province_index;

			if (0 < province_index && province_index <= header.province_count) {
				switch (lang_index) {
				case 0:
					province_str = province_info[province_index-1].name1;
					province_str_len = header.province_name_len[0]/2;
					break;
				case 1:
					province_str = province_info[province_index-1].name2;
					province_str_len = header.province_name_len[1]/2;
					break;
				case 2:
					province_str = province_info[province_index-1].name3;
					province_str_len = header.province_name_len[2]/2;
					break;
				default:
					ERR("Invalid lang_index(%d)", lang_index);
					break;
				}
			}

			province_temp = g_utf16_to_utf8(province_str, province_str_len, NULL, NULL,
					NULL);
			city_temp = g_utf16_to_utf8(city_str, city_str_len, NULL, NULL, NULL);

			if (city_temp && province_temp) {
				int size = strlen(city_temp) + strlen(province_temp);
				char *location = NULL;
				location = calloc(size+3, sizeof(char));
				if (location)
					snprintf(location, size+3, "%s, %s", city_temp, province_temp);
				*p_location = location;
			} else if (city_temp) {
				int size = strlen(city_temp);
				char *location = NULL;
				location = calloc(size+1, sizeof(char));
				if (location)
					snprintf(location, size+1, "%s", city_temp);
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

