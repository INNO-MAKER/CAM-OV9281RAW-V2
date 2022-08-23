################################################################################
# Makefile
#
# Mipi Module Sensor Driver Inno-maker 2019
#
# Author: Jack Yang
#
################################################################################

# must correspond to its file name
#
SENSOR_NAME   = vc_mipi_ov9281

# must correspond to its directory name
# at /lib/modules/*/kernel/drivers/media/platform
#
PLATFORM_NAME = bcm2835
CMOS_NAME      = ov9281-i2c

################################################################################
SENSOR_DRIVER       = $(SENSOR_NAME)
SENSOR_DRIVER_DIR   = $(SENSOR_NAME)
PLATFORM_DRIVER     = $(PLATFORM_NAME)-unicam
CMOS_DRIVER         = $(CMOS_NAME)
KERNEL_MODULE_DIR   = /lib/modules/$(shell uname -r)
KERNEL_BUILD_DIR    = $(KERNEL_MODULE_DIR)/build
KERNEL_I2C_DIR      = $(KERNEL_MODULE_DIR)/kernel/drivers/media/i2c
KERNEL_CMOS_DIR      = $(KERNEL_MODULE_DIR)/kernel/drivers/input/touchscreen
KERNEL_PLATFORM_DIR = $(KERNEL_MODULE_DIR)/kernel/drivers/media/platform/$(PLATFORM_NAME)
BOOT_OVERLAYS_DIR   = /boot/overlays


obj-m := $(SENSOR_DRIVER_DIR)/$(SENSOR_DRIVER).o
obj-m += $(CMOS_DRIVER).o

.PHONY: all






install:  devicetree-install
	sudo install -p -m 644 $(CMOS_DRIVER).ko   $(KERNEL_CMOS_DIR)/
	sudo install -p -m 644 $(SENSOR_DRIVER_DIR)/$(SENSOR_DRIVER).ko   $(KERNEL_I2C_DIR)/
	sudo install -p -m 644 $(SENSOR_DRIVER).dtbo $(BOOT_OVERLAYS_DIR)/
	sudo /sbin/depmod -a $(shell uname -r)
	sudo /sbin/modprobe $(CMOS_DRIVER) 
	sudo /sbin/modprobe $(SENSOR_DRIVER)
	@echo "--------------------------------------"
	@echo
	@echo "  ADD 'dtparam=i2c_vc=on' and 'dtoverlay=$(SENSOR_DRIVER)' to your /boot/config.txt"
	@echo "  ADD 'disable_touchscreen=1' to your /boot/config.txt if a touchscreen is attached"
	@echo "  ADD 'cma=128M' to your /boot/cmdline.txt"
	@echo
	@echo "--------------------------------------"

rmmod:
	#sudo /sbin/modprobe -r $(PLATFORM_DRIVER)
	sudo /sbin/modprobe -r $(SENSOR_DRIVER)
	sudo /sbin/modprobe -r $(CMOS_DRIVER)

uninstall:
	sudo rm -f $(KERNEL_I2C_DIR)/$(SENSOR_DRIVER).ko
	sudo rm -f $(KERNEL_CMOS_DIR)/$(CMOS_DRIVER).ko
	sudo rm -f $(BOOT_OVERLAYS_DIR)/$(SENSOR_DRIVER).dtbo
	sudo /sbin/depmod -a $(shell uname -r)
	@echo "--------------------------------------"
	@echo
	@echo "  REMOVE 'dtparam=i2c_vc=on' and 'dtoverlay=$(SENSOR_DRIVER)' from your /boot/config.txt"
	@echo "  REMOVE 'disable_touchscreen=1' from your /boot/config.txt if a touchscreen is attached"
	@echo "  REMOVE 'cma=128M' from your /boot/cmdline.txt"
	@echo
	@echo "--------------------------------------"


devicetree-install: 
	sudo install -p -m 644  $(SENSOR_DRIVER).dtbo  $(BOOT_OVERLAYS_DIR)

setmode0:
	sudo /sbin/modprobe -r $(PLATFORM_DRIVER)
	sudo /sbin/modprobe -r $(SENSOR_DRIVER)
	sudo /sbin/modprobe $(PLATFORM_DRIVER) debug=3
	sudo /sbin/modprobe $(SENSOR_DRIVER) sensor_mode=0

setmode1:
	sudo /sbin/modprobe -r $(PLATFORM_DRIVER)
	sudo /sbin/modprobe -r $(SENSOR_DRIVER)
	sudo /sbin/modprobe $(PLATFORM_DRIVER) debug=3
	sudo /sbin/modprobe $(SENSOR_DRIVER) sensor_mode=1

setmode2:
	sudo /sbin/modprobe -r $(PLATFORM_DRIVER)
	sudo /sbin/modprobe -r $(SENSOR_DRIVER)
	sudo /sbin/modprobe $(PLATFORM_DRIVER) debug=3
	sudo /sbin/modprobe $(SENSOR_DRIVER) sensor_mode=2

setmode3:
	sudo /sbin/modprobe -r $(PLATFORM_DRIVER)
	sudo /sbin/modprobe -r $(SENSOR_DRIVER)
	sudo /sbin/modprobe $(PLATFORM_DRIVER) debug=3
	sudo /sbin/modprobe $(SENSOR_DRIVER) sensor_mode=3

setmode4:
	sudo /sbin/modprobe -r $(PLATFORM_DRIVER)
	sudo /sbin/modprobe -r $(SENSOR_DRIVER)
	sudo /sbin/modprobe $(PLATFORM_DRIVER) debug=3
	sudo /sbin/modprobe $(SENSOR_DRIVER) sensor_mode=4
setmode5:
	sudo /sbin/modprobe -r $(PLATFORM_DRIVER)
	sudo /sbin/modprobe -r $(SENSOR_DRIVER)
	sudo /sbin/modprobe $(PLATFORM_DRIVER) debug=3
	sudo /sbin/modprobe $(SENSOR_DRIVER) sensor_mode=5
setmode6:
	sudo /sbin/modprobe -r $(PLATFORM_DRIVER)
	sudo /sbin/modprobe -r $(SENSOR_DRIVER)
	sudo /sbin/modprobe $(PLATFORM_DRIVER) debug=3
	sudo /sbin/modprobe $(SENSOR_DRIVER) sensor_mode=6
setmode7:
	sudo /sbin/modprobe -r $(PLATFORM_DRIVER)
	sudo /sbin/modprobe -r $(SENSOR_DRIVER)
	sudo /sbin/modprobe $(PLATFORM_DRIVER) debug=3
	sudo /sbin/modprobe $(SENSOR_DRIVER) sensor_mode=7
setmode8:
	sudo /sbin/modprobe -r $(PLATFORM_DRIVER)
	sudo /sbin/modprobe -r $(SENSOR_DRIVER)
	sudo /sbin/modprobe $(PLATFORM_DRIVER) debug=3
	sudo /sbin/modprobe $(SENSOR_DRIVER) sensor_mode=8
setmode9:
	sudo /sbin/modprobe -r $(PLATFORM_DRIVER)
	sudo /sbin/modprobe -r $(SENSOR_DRIVER)
	sudo /sbin/modprobe $(PLATFORM_DRIVER) debug=3
	sudo /sbin/modprobe $(SENSOR_DRIVER) sensor_mode=9
setmode10:
	sudo /sbin/modprobe -r $(PLATFORM_DRIVER)
	sudo /sbin/modprobe -r $(SENSOR_DRIVER)
	sudo /sbin/modprobe $(PLATFORM_DRIVER) debug=3
	sudo /sbin/modprobe $(SENSOR_DRIVER) sensor_mode=10
setmode11:
	sudo /sbin/modprobe -r $(PLATFORM_DRIVER)
	sudo /sbin/modprobe -r $(SENSOR_DRIVER)
	sudo /sbin/modprobe $(PLATFORM_DRIVER) debug=3
	sudo /sbin/modprobe $(SENSOR_DRIVER) sensor_mode=11
setmode12:
	sudo /sbin/modprobe -r $(PLATFORM_DRIVER)
	sudo /sbin/modprobe -r $(SENSOR_DRIVER)
	sudo /sbin/modprobe $(PLATFORM_DRIVER) debug=3
	sudo /sbin/modprobe $(SENSOR_DRIVER) sensor_mode=12
setmode13:
	sudo /sbin/modprobe -r $(PLATFORM_DRIVER)
	sudo /sbin/modprobe -r $(SENSOR_DRIVER)
	sudo /sbin/modprobe $(PLATFORM_DRIVER) debug=3
	sudo /sbin/modprobe $(SENSOR_DRIVER) sensor_mode=13



$(KERNEL_BUILD_DIR):
	@echo "--------------------------------------"
	@echo
	@echo "  Error:  Cannot continue due to the absence of the directory"
	@echo
	@echo "    $(KERNEL_BUILD_DIR)"
	@echo
	@echo "  You can try to fix it by installing the kernel header files package."
	@echo "  The following command may do so:"
	@echo
	@echo
	@echo "--------------------------------------"
	@false


