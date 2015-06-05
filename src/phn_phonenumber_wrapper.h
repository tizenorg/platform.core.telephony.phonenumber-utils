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

#ifndef __PHN_PHONENUMBER_WRAPPER_H__
#define __PHN_PHONENUMBER_WRAPPER_H__

#ifdef __cplusplus
extern "C" {
#endif

int phn_get_formatted_number(const char *number, const char *region, char **formatted_number);
int phn_get_location_from_number(const char *number, const char *region, const char *language, char **location);

#ifdef __cplusplus
}
#endif

#endif // __PHN_PHONENUMBER_WRAPPER_H__