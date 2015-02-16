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

#ifndef __PHN_COMMON_H__
#define __PHN_COMMON_H__

#include "phone_number_errors.h"

#define PHN_STR_SHORT_LEN 1024

#define LOG_TAG "PHONE_NUMBER_UTILS"
#include <dlog.h>

#ifndef API
#define API __attribute__ ((visibility("default")))
#endif

#define DLOG(prio, fmt, arg...) \
  do { SLOG(prio, LOG_TAG, fmt, ##arg); } while(0);
#define INFO(fmt, arg...) SLOGI(fmt, ##arg)
#define WARN(fmt, arg...) SLOGW("%s:%d " fmt, __FUNCTION__, __LINE__, ##arg)
#define ERR(fmt, arg...) SLOGE("%s:%d " fmt, __FUNCTION__, __LINE__, ##arg)
#define DBG(fmt, arg...) SLOGD("%s:" fmt, __FUNCTION__, ##arg)
#define WARN_IF(expr, fmt, arg...) do { \
	if (expr) { \
		WARN(fmt, ##arg); \
	} \
} while (0)

#define RET_IF(expr) do { \
	if (expr) { \
		ERR("(%s)", #expr); \
		return; \
	} \
} while (0)
#define RETV_IF(expr, val) do { \
	if (expr) { \
		ERR("(%s)", #expr); \
		return (val); \
	} \
} while (0)
#define RETM_IF(expr, fmt, arg...) do { \
	if (expr) { \
		ERR(fmt, ##arg); \
		return; \
	} \
} while (0)
#define RETVM_IF(expr, val, fmt, arg...) do { \
	if (expr) { \
		ERR(fmt, ##arg); \
		return (val); \
	} \
} while (0)

#endif // __PHN_COMMON_H__
