#
# Makefile for miscellaneous I2C chip drivers.
#
# Do not add new drivers to this directory! It is DEPRECATED.
#
# Device drivers are better grouped according to the functionality they
# implement rather than to the bus they are connected to. In particular:
# * Hardware monitoring chip drivers go to drivers/hwmon
# * RTC chip drivers go to drivers/rtc
# * I/O expander drivers go to drivers/gpio
#

obj-$(CONFIG_SENSORS_ONLY_BMA250)      += bma250_without_Ecompass.o
obj-$(CONFIG_TPS65200)          += tps65200.o
obj-$(CONFIG_INPUT_CAPELLA_CM3628)       += cm3628.o
obj-$(CONFIG_INPUT_CAPELLA_CM3629)       += cm3629.o
