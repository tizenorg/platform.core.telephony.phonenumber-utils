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
#ifndef __TIZEN_TELEPHONY_PHONE_NUMBER_UTILS_H__
#define __TIZEN_TELEPHONY_PHONE_NUMBER_UTILS_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <phone_number_types.h>
#include <phone_number_errors.h>

/**
 * @file phone_number.h
 */

/**
 * @addtogroup CAPI_TELEPHONY_PHONE_NUMBER_UTILS_MODULE
 *
 * @{
 */

/**
 * @brief This function gets the location string from number, region and language.
 *
 * @since_tizen 2.4
 *
 * @remarks You must release @a location using free().
 *
 * @param[in]   number    The number
 * @param[in]   region    The region of number
 * @param[in]   language  The language of location
 * @param[out]  location  The location string to be returned
 *
 * @return     @c 0 on success,
 *             otherwise a negative error value
 *
 * @retval  #PHONE_NUMBER_ERROR_NONE                        Successful
 * @retval  #PHONE_NUMBER_ERROR_OUT_OF_MEMORY               Out of memory
 * @retval  #PHONE_NUMBER_ERROR_INVALID_PARAMETER           Invalid parameter
 * @retval  #PHONE_NUMBER_ERROR_FILE_NO_SPACE_ON_DEVICE     FS Full
 * @retval  #PHONE_NUMBER_ERROR_PERMISSION_DENIED           Permission denied
 * @retval  #PHONE_NUMBER_ERROR_NOT_SUPPORTED               Not supported
 * @retval  #PHONE_NUMBER_ERROR_NO_DATA                     Requested data does not exist
 *
 */
int phone_number_get_location_from_number(const char *number,
		phone_number_region_e region, phone_number_lang_e language, char **location);

/**
 * @brief This function gets the formatted number.
 *
 * @since_tizen 2.4
 *
 * @remarks You must release @a formatted_number using free().
 *
 * @param[in]   number             The number
 * @param[in]   region             The region of number
 * @param[out]  formatted_number   The formatted number string to be returned
 *
 * @return     @c 0 on success,
 *             otherwise a negative error value
 *
 * @retval  #PHONE_NUMBER_ERROR_NONE                        Successful
 * @retval  #PHONE_NUMBER_ERROR_OUT_OF_MEMORY               Out of memory
 * @retval  #PHONE_NUMBER_ERROR_INVALID_PARAMETER           Invalid parameter
 * @retval  #PHONE_NUMBER_ERROR_FILE_NO_SPACE_ON_DEVICE     FS Full
 * @retval  #PHONE_NUMBER_ERROR_PERMISSION_DENIED           Permission denied
 * @retval  #PHONE_NUMBER_ERROR_NOT_SUPPORTED               Not supported
 * @retval  #PHONE_NUMBER_ERROR_NO_DATA                     Requested data does not exist
 *
 */
int phone_number_get_formatted_number(const char *number, phone_number_region_e region,
		char **formatted_number);

/**
 * @brief This function gets the normalized number.
 *        Normalized number starts with plus('+') and country code, and excludes the separators such as dash or space.
 *
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @remarks You must release @a normalized_number using free().
 *
 * @param[in]   number             The number
 * @param[out]  normalized_number  The normalized number string to be returned
 *
 * @return     @c 0 on success,
 *             otherwise a negative error value
 *
 * @retval  #PHONE_NUMBER_ERROR_NONE                        Successful
 * @retval  #PHONE_NUMBER_ERROR_OUT_OF_MEMORY               Out of memory
 * @retval  #PHONE_NUMBER_ERROR_INVALID_PARAMETER           Invalid parameter
 * @retval  #PHONE_NUMBER_ERROR_FILE_NO_SPACE_ON_DEVICE     FS Full
 * @retval  #PHONE_NUMBER_ERROR_NO_DATA                     Requested data does not exist
 * @retval  #PHONE_NUMBER_ERROR_SYSTEM                      Error from another modules
 *
 */
int phone_number_get_normalized_number(const char *number, char **normalized_number);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_TELEPHONY_PHONE_NUMBER_UTILS_H__ */
