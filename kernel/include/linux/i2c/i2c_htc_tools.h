/* include/linux/i2c/i2c_htc_tools.h
 *
 * Copyright (C) 2007-2008 HTC Corporation.
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

#ifndef I2C_HTC_TOOLS_H
#define I2C_HTC_TOOLS_H

#include <linux/types.h>
#include <linux/platform_device.h>
#include <linux/i2c.h>

#if defined(CONFIG_I2C_QUP)
#include <mach/gpio.h>
#include <mach/board.h>
#endif

struct htc_i2c_platform_data {
	uint8_t allocDone;
	int gpio_clk;
	int gpio_data;
	uint32_t *operatingSetting;
	uint32_t *recoverSetting;
};

#endif /* I2C_HTC_TOOLS_H */
