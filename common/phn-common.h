/*
 * Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
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
 */
#ifndef __PHONENUMBER_UTILS_COMMON_H__
#define __PHONENUMBER_UTILS_COMMON_H__

#include "phone_number_errors.h"

#ifndef PHN_DBUS_INTERFACE
#define PHN_DBUS_INTERFACE "org.tizen.PhonenumberUtils.dbus"
#endif

#define PHN_DBUS_OBJPATH "/org/tizen/phonenumberutils/dbus"
#define PHN_DBUS_SIGNAL_LENGTH 20
#define PHN_STR_SHORT_LEN 1024
#define STRING_EQUAL 0

#define PHN_DBUS_SIGNAL_REQUEST_HANDLER "REQ"
#define PHN_DBUS_SIGNAL_FOUND_RESOURCE "RES"
#define PHN_DBUS_SIGNAL_GET "GET"
#define PHN_DBUS_SIGNAL_PUT "PUT"
#define PHN_DBUS_SIGNAL_POST "POST"
#define PHN_DBUS_SIGNAL_DELETE "DELETE"
#define PHN_DBUS_SIGNAL_OBSERVE "OBSERVE"
#define PHN_DBUS_SIGNAL_DEVICE "DEVICE"
#define PHN_DBUS_SIGNAL_PLATFORM "PLATFORM"
#define PHN_DBUS_SIGNAL_PRESENCE "PRESENCE"

#endif /* __PHONENUMBER_UTILS_COMMON_H__ */

