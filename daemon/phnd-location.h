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
#ifndef __PHN_LOCATION_H__
#define __PHN_LOCATION_H__

#include "phone_number_types.h"

#define PHN_LOCATION_SUPPORT_ZH_CN "zh_CN"
#define PHN_LOCATION_SUPPORT_EN_CN "en_CN"
#define PHN_LOCATION_SUPPORT_KO_CN "ko_CN"

int phn_location_find_extra_data(char *region_str, char **p_location_file);
int phn_location_get_location_from_extra_data(const char *file, const char *number,
		char *region_str, char *lang_str, char **p_location);

#endif /* __PHN_LOCATION_H__ */
