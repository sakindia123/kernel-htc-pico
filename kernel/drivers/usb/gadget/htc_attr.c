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

#include <mach/board.h>
#include <linux/gpio.h>

static int use_mfg_serialno;
static char mfg_df_serialno[16];

extern int usb_get_connect_type(void);
extern int usb_get_accessory_detect(void);
extern int android_switch_function(unsigned func);
extern int android_show_function(char *buf);
extern void android_set_serialno(char *serialno);
extern void android_force_reset(void);

void init_mfg_serialno(void)
{
	char *serialno = "000000000000";

	use_mfg_serialno = (board_mfg_mode() == 1) ? 1 : 0;
	strncpy(mfg_df_serialno, serialno, strlen(serialno));
}

static ssize_t show_usb_cable_connect(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	unsigned length;

	length = sprintf(buf, "%d",
		    (usb_get_connect_type() == CONNECT_TYPE_USB) ? 1 : 0);
	return length;
}

static ssize_t show_usb_function_switch(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	return android_show_function(buf);
}

static ssize_t store_usb_function_switch(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	unsigned u;
	ssize_t  ret;

	u = simple_strtoul(buf, NULL, 10);
	ret = android_switch_function(u);

	if (ret == 0)
		return count;
	else
		return 0;
}

static ssize_t show_USB_ID_status(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct android_usb_platform_data *pdata = dev->platform_data;
	int value = 1;
	unsigned length;

	if (pdata->usb_id_pin_gpio != 0) {
		value = gpio_get_value(pdata->usb_id_pin_gpio);
		printk(KERN_INFO"[USB] id pin status %d\n", value);
	}

	length = sprintf(buf, "%d", value);
	return length;
}

static ssize_t show_usb_serial_number(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	unsigned length;
	struct android_usb_platform_data *pdata = dev->platform_data;

	length = sprintf(buf, "%s", pdata->serial_number);
	return length;
}

static ssize_t store_usb_serial_number(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	struct android_usb_platform_data *pdata = dev->platform_data;
	char *serialno = "000000000000";

	if (buf[0] == '0' || buf[0] == '1') {
		memset(mfg_df_serialno, 0x0, sizeof(mfg_df_serialno));
		if (buf[0] == '0') {
			strncpy(mfg_df_serialno, serialno, strlen(serialno));
			use_mfg_serialno = 1;
			android_set_serialno(mfg_df_serialno);
		} else {
			strncpy(mfg_df_serialno, pdata->serial_number,
					strlen(pdata->serial_number));
			use_mfg_serialno = 0;
			android_set_serialno(pdata->serial_number);
		}
		/* reset_device */
		android_force_reset();
	}

	return count;
}

static ssize_t show_dummy_usb_serial_number(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	unsigned length;
	struct android_usb_platform_data *pdata = dev->platform_data;

	if (use_mfg_serialno)
		length = sprintf(buf, "%s", mfg_df_serialno); /* dummy */
	else
		length = sprintf(buf, "%s", pdata->serial_number); /* Real */
	return length;
}

static ssize_t store_dummy_usb_serial_number(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	int data_buff_size = (sizeof(mfg_df_serialno) > strlen(buf))?
		strlen(buf):sizeof(mfg_df_serialno);
	int loop_i;

	/* avoid overflow, mfg_df_serialno[16] always is 0x0 */
	if (data_buff_size == 16)
		data_buff_size--;

	for (loop_i = 0; loop_i < data_buff_size; loop_i++)     {
		if (buf[loop_i] >= 0x30 && buf[loop_i] <= 0x39) /* 0-9 */
			continue;
		else if (buf[loop_i] >= 0x41 && buf[loop_i] <= 0x5A) /* A-Z */
			continue;
		if (buf[loop_i] == 0x0A) /* Line Feed */
			continue;
		else {
			printk(KERN_INFO "%s(): get invaild char (0x%2.2X)\n",
					__func__, buf[loop_i]);
			return -EINVAL;
		}
	}

	use_mfg_serialno = 1;
	memset(mfg_df_serialno, 0x0, sizeof(mfg_df_serialno));
	strncpy(mfg_df_serialno, buf, data_buff_size);
	android_set_serialno(mfg_df_serialno);
	/*device_reset */
	android_force_reset();

	return count;
}

static ssize_t
show_usb_car_kit_enable(struct device *dev, struct device_attribute *attr,
		char *buf)
{
	int value = (usb_get_accessory_detect() == 0) ? 0 : 1;
	unsigned length;

	printk(KERN_INFO "USB_car_kit_enable %d\n", usb_get_accessory_detect());
	length = sprintf(buf, "%d", value);
	return length;
}

static DEVICE_ATTR(usb_cable_connect, 0444, show_usb_cable_connect, NULL);
static DEVICE_ATTR(usb_function_switch, 0664,
		show_usb_function_switch, store_usb_function_switch);
static DEVICE_ATTR(USB_ID_status, 0444, show_USB_ID_status, NULL);
static DEVICE_ATTR(usb_serial_number, 0644,
		show_usb_serial_number, store_usb_serial_number);
static DEVICE_ATTR(dummy_usb_serial_number, 0644,
		show_dummy_usb_serial_number, store_dummy_usb_serial_number);
static DEVICE_ATTR(usb_car_kit_enable, 0444, show_usb_car_kit_enable, NULL);

static struct attribute *android_usb_attributes[] = {
	&dev_attr_usb_cable_connect.attr,
	&dev_attr_usb_function_switch.attr,
	&dev_attr_USB_ID_status.attr, /* for MFG */
	&dev_attr_usb_serial_number.attr, /* for MFG */
	&dev_attr_dummy_usb_serial_number.attr, /* for MFG */
	&dev_attr_usb_car_kit_enable.attr,
	NULL
};

static const struct attribute_group android_usb_attr_group = {
	.attrs = android_usb_attributes,
};

