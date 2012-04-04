/*
 * Copyright (C) 2011 HTC, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#ifndef _ASM_ARCH_HTC_DEBUG_INFO_H_
#define _ASM_ARCH_HTC_DEBUG_INFO_H_

#include <asm/sizes.h>

extern struct msm_htc_debug_info *htc_debug_info;

struct msm_htc_debug_info {
	char msm_tzlog[SZ_64K];
	char msm_wdt_proc_info[SZ_4K];
	char msm_do_irq_info[SZ_16];
	char msm_spin_lock_irqsave_info[SZ_16];
	struct msm_irq_count_info {
		char msm_irq_count_watchdog_enable[2];
		char msm_irq_count_timestamp[8];
		char msm_irq_count_watchdog_pet_work_cpu[2];
		char msm_irq_count_watchdog_pet_timestamp[8];
		char msm_irq_count[972];
	} msm_irq_count_info;
	char msm_tz_handle_bark_reg_save[SZ_4K];
	char reset_msg[SZ_256];
	char reserved[56052];
	char msm_idle_sleep_delay[4];
	char msm_power_collapse_status[8];
};

static inline void debug_reset_msg(const char *msg)
{
	int len;
	BUG_ON(!htc_debug_info);
	len = strlen(msg);
	if (len >= sizeof(htc_debug_info->reset_msg))
		len = sizeof(htc_debug_info->reset_msg) - 1;
	memcpy(htc_debug_info->reset_msg, msg, len);
}
#endif
