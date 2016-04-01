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
#ifndef __PHONENUMBER_COMMON_LOG_H__
#define __PHONENUMBER_COMMON_LOG_H__

#ifdef PHN_CONSOLE_COLOR_LOG
#define PHN_LOG_RED "\033[0;31m"
#define PHN_LOG_GREEN "\033[0;32m"
#define PHN_LOG_BROWN "\033[0;33m"
#define PHN_LOG_BLUE "\033[0;34m"
#define PHN_LOG_END "\033[0;m"
#else
#define PHN_LOG_RED
#define PHN_LOG_GREEN
#define PHN_LOG_BROWN
#define PHN_LOG_BLUE
#define PHN_LOG_END
#endif

#undef _DBG
#undef _INFO
#undef _WARN
#undef _ERR

#undef DBG
#undef INFO
#undef WARN
#undef ERR

#define LOG_TAG "PHN_UTILS"
#include <dlog.h>

#ifdef PHN_DAEMON_LOG

#define _DBG(fmt, arg...) SLOGD(PHN_LOG_GREEN "<Daemon> " PHN_LOG_END fmt, ##arg)
#define _INFO(fmt, arg...) SLOGI(PHN_LOG_GREEN "<Daemon> " PHN_LOG_END fmt, ##arg)
#define _WARN(fmt, arg...) SLOGW(PHN_LOG_GREEN "<Daemon> " PHN_LOG_END fmt, ##arg)
#define _ERR(fmt, arg...) SLOGE(PHN_LOG_GREEN "<Daemon> " PHN_LOG_END fmt, ##arg)

#else /* PHN_DAEMON_LOG */

#define _DBG(fmt, arg...) SLOGD(fmt, ##arg)
#define _INFO(fmt, arg...) SLOGI(fmt, ##arg)
#define _WARN(fmt, arg...) SLOGW(fmt, ##arg)
#define _ERR(fmt, arg...) SLOGE(fmt, ##arg)

#endif /* PHN_DAEMON_LOG */


#define PHN_DEBUG_ON

#ifdef PHN_DEBUG_ON

#define FN_CALL _INFO(">>>>>>>> called")
#define FN_END _INFO("<<<<<<<< ended")
#define DBG(fmt, arg...) _DBG(fmt, ##arg)
#define WARN(fmt, arg...) _WARN(PHN_LOG_BROWN fmt PHN_LOG_END, ##arg)
#define ERR(fmt, arg...) _ERR(PHN_LOG_RED fmt PHN_LOG_END, ##arg)
#define INFO(fmt, arg...) _INFO(PHN_LOG_BLUE fmt PHN_LOG_END, ##arg)
#define SECURE_DBG(fmt, arg...) SECURE_SLOGD(fmt, ##arg)
#define SECURE_ERR(fmt, arg...) SECURE_SLOGE(fmt, ##arg)

#else /* PHN_DEBUG_ON */

#define FN_CALL
#define FN_END
#define DBG(fmt, arg...)
#define WARN(fmt, arg...)
#define ERR(fmt, arg...) _ERR(fmt, ##arg)
#define INFO(fmt, arg...)
#define SECURE_DBG(fmt, arg...)
#define SECURE_ERR(fmt, arg...) SECURE_SLOGE(fmt, ##arg)

#endif /* PHN_DEBUG_ON */


#define RET_IF(expr) \
	do { \
		if (expr) { \
			ERR("(%s)", #expr); \
			return; \
		} \
	} while (0)

#define RETV_IF(expr, val) \
	do {\
		if (expr) { \
			ERR("(%s)", #expr); \
			return (val); \
		} \
	} while (0)

#define RETM_IF(expr, fmt, arg...) \
	do {\
		if (expr) { \
			ERR(fmt, ##arg); \
			return; \
		} \
	} while (0)

#define RETVM_IF(expr, val, fmt, arg...) \
	do {\
		if (expr) { \
			ERR(fmt, ##arg); \
			return (val); \
		} \
	} while (0)

#define ERR_IF(expr) \
	do { \
		if (expr) { \
			ERR("(%s)", #expr); \
		} \
	} while (0)

#define WARN_IF(expr, fmt, arg...) \
	do { \
		if (expr) { \
			WARN(fmt, ##arg); \
		} \
	} while (0)

#endif /* __PHONENUMBER_COMMON_LOG_H__ */
