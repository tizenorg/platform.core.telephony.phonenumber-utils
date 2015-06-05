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

#ifndef __PHN_REGION_DATA_H__
#define __PHN_REGION_DATA_H__

#include "phone_number_types.h"

#define PHN_REGION_DEFAULT_LANG "en"

const char* phn_region_data_get_region_str(phone_number_region_e region);
const char* phn_region_data_get_lang_str(phone_number_lang_e lang);
bool phn_region_data_find_match_info(phone_number_region_e region, phone_number_lang_e lang);

#endif
