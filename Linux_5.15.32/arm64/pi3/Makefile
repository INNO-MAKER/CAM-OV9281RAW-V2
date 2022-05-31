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

################################################################################
SENSOR_DRIVER       = $(SENSOR_NAME)
SENSOR_DRIVER_DIR   = $(SENSOR_NAME)
PLATFORM_DRIVER     = $(PLATFORM_NAME)-unicam
KERNEL_MODULE_DIR   = /lib/modules/$(shell uname -r)
KERNEL_BUILD_DIR    = $(KERNEL_MODULE_DIR)/build
KERNEL_I2C_DIR      = $(KERNEL_MODULE_DIR)/kernel/drivers/media/i2c
KERNEL_PLATFORM_DIR = $(KERNEL_MODULE_DIR)/kernel/drivers/media/platform/$(PLATFORM_NAME)
BOOT_OVERLAYS_DIR   = /boot/overlays

.PHONY: all

install: 
	sudo install -p -m 644 $(SENSOR_DRIVER_DIR)/$(SENSOR_DRIVER).ko   $(KERNEL_I2C_DIR)/
	sudo install -p -m 644 $(SENSOR_DRIVER).dtbo $(BOOT_OVERLAYS_DIR)/
	sudo /sbin/depmod -a $(shell uname -r)
	sudo /sbin/modprobe $(SENSOR_DRIVER)
	@echo "--------------------------------------"
	@echo
	@echo "  ADD 'dtparam=i2c_vc=on' and 'dtoverlay=$(SENSOR_DRIVER)' to your /boot/config.txt"
	@echo "  ADD 'disable_touchscreen=1' to your /boot/config.txt if a touchscreen is attached"
	@echo "  ADD 'cma=128M' to your /boot/cmdline.txt"
	@echo
	@echo "--------------------------------------"

rmmod:
	sudo /sbin/modprobe -r $(PLATFORM_DRIVER)
	sudo /sbin/modprobe -r $(SENSOR_DRIVER)

uninstall:
	sudo rm -f $(KERNEL_I2C_DIR)/$(SENSOR_DRIVER).ko
	sudo rm -f $(BOOT_OVERLAYS_DIR)/$(SENSOR_DRIVER).dtbo
	sudo /sbin/depmod -a $(shell uname -r)
	@echo "--------------------------------------"
	@echo
	@echo "  REMOVE 'dtparam=i2c_vc=on' and 'dtoverlay=$(SENSOR_DRIVER)' from your /boot/config.txt"
	@echo "  REMOVE 'disable_touchscreen=1' from your /boot/config.txt if a touchscreen is attached"
	@echo "  REMOVE 'cma=128M' from your /boot/cmdline.txt"
	@echo
	@echo "--------------------------------------"


devicetree-install: devicetree
	sudo install -p -m 644  $(SENSOR_DRIVER).dtbo  $(BOOT_OVERLAYS_DIR)

setmode0:
	sudo /sbin/modprobe -r $(PLATFORM_DRIVER)
	sudo /sbin/modprobe -r $(SENSOR_DRIVER)
	sudo /sbin/modprobe $(PLATFORM_DRIVER) debug=3
	sudo /sbin/modprobe $(SENSOR_DRIVER) sensor_mode=0
	sudo dmesg -c

setmode1:
	sudo /sbin/modprobe -r $(PLATFORM_DRIVER)
	sudo /sbin/modprobe -r $(SENSOR_DRIVER)
	sudo /sbin/modprobe $(PLATFORM_DRIVER) debug=3
	sudo /sbin/modprobe $(SENSOR_DRIVER) sensor_mode=1
	sudo dmesg -c

setmode2:
	sudo /sbin/modprobe -r $(PLATFORM_DRIVER)
	sudo /sbin/modprobe -r $(SENSOR_DRIVER)
	sudo /sbin/modprobe $(PLATFORM_DRIVER) debug=3
	sudo /sbin/modprobe $(SENSOR_DRIVER) sensor_mode=2
	sudo dmesg -c
	
setmode3:
	sudo /sbin/modprobe -r $(PLATFORM_DRIVER)
	sudo /sbin/modprobe -r $(SENSOR_DRIVER)
	sudo /sbin/modprobe $(PLATFORM_DRIVER) debug=3
	sudo /sbin/modprobe $(SENSOR_DRIVER) sensor_mode=3
	sudo dmesg -c
	
setmode4:
	sudo /sbin/modprobe -r $(PLATFORM_DRIVER)
	sudo /sbin/modprobe -r $(SENSOR_DRIVER)
	sudo /sbin/modprobe $(PLATFORM_DRIVER) debug=3
	sudo /sbin/modprobe $(SENSOR_DRIVER) sensor_mode=4
	sudo dmesg -c	
	
setmode5:
	sudo /sbin/modprobe -r $(PLATFORM_DRIVER)
	sudo /sbin/modprobe -r $(SENSOR_DRIVER)
	sudo /sbin/modprobe $(PLATFORM_DRIVER) debug=3
	sudo /sbin/modprobe $(SENSOR_DRIVER) sensor_mode=5
	sudo dmesg -c	

setmode6:
	sudo /sbin/modprobe -r $(PLATFORM_DRIVER)
	sudo /sbin/modprobe -r $(SENSOR_DRIVER)
	sudo /sbin/modprobe $(PLATFORM_DRIVER) debug=3
	sudo /sbin/modprobe $(SENSOR_DRIVER) sensor_mode=6
	sudo dmesg -c

setmode7:
	sudo /sbin/modprobe -r $(PLATFORM_DRIVER)
	sudo /sbin/modprobe -r $(SENSOR_DRIVER)
	sudo /sbin/modprobe $(PLATFORM_DRIVER) debug=3
	sudo /sbin/modprobe $(SENSOR_DRIVER) sensor_mode=7
	sudo dmesg -c

setmode8:
	sudo /sbin/modprobe -r $(PLATFORM_DRIVER)
	sudo /sbin/modprobe -r $(SENSOR_DRIVER)
	sudo /sbin/modprobe $(PLATFORM_DRIVER) debug=3
	sudo /sbin/modprobe $(SENSOR_DRIVER) sensor_mode=8
	sudo dmesg -c

setmode9:
	sudo /sbin/modprobe -r $(PLATFORM_DRIVER)
	sudo /sbin/modprobe -r $(SENSOR_DRIVER)
	sudo /sbin/modprobe $(PLATFORM_DRIVER) debug=3
	sudo /sbin/modprobe $(SENSOR_DRIVER) sensor_mode=9
	sudo dmesg -c
	
setmode10:
	sudo /sbin/modprobe -r $(PLATFORM_DRIVER)
	sudo /sbin/modprobe -r $(SENSOR_DRIVER)
	sudo /sbin/modprobe $(PLATFORM_DRIVER) debug=3
	sudo /sbin/modprobe $(SENSOR_DRIVER) sensor_mode=10
	sudo dmesg -c

setmode11:
	sudo /sbin/modprobe -r $(PLATFORM_DRIVER)
	sudo /sbin/modprobe -r $(SENSOR_DRIVER)
	sudo /sbin/modprobe $(PLATFORM_DRIVER) debug=3
	sudo /sbin/modprobe $(SENSOR_DRIVER) sensor_mode=11
	sudo dmesg -c
						
setmode12:
	sudo /sbin/modprobe -r $(PLATFORM_DRIVER)
	sudo /sbin/modprobe -r $(SENSOR_DRIVER)
	sudo /sbin/modprobe $(PLATFORM_DRIVER) debug=3
	sudo /sbin/modprobe $(SENSOR_DRIVER) sensor_mode=12
	sudo dmesg -c
setmode13:
	sudo /sbin/modprobe -r $(PLATFORM_DRIVER)
	sudo /sbin/modprobe -r $(SENSOR_DRIVER)
	sudo /sbin/modprobe $(PLATFORM_DRIVER) debug=3
	sudo /sbin/modprobe $(SENSOR_DRIVER) sensor_mode=13
	sudo dmesg -c



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


