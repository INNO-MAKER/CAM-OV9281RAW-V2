## CAM-OV9281RAW V2  Quick Start:

This document is to guide you on how to quickly test this camera on the Raspberry Pi 5. If you don't have experience with Raspberry Pi or its cameras, or if you need more information, please refer to the user manual.

The CAM-OV9281RAW can be used with the Raspberry Pi system's built-in drivers and the libcamera/rpicam tools. It can be used directly once the DTOVERLAY configuration is completed

### 1. Dtoverlay

1.Open  the config.txt on terminal via build-in nano tools.

```
sudo nano /boot/firmware/config.txt
```

Legacy version system：

```
sudo nano /boot/config.txt
```

2.Append the following lines to the end of the files.

camera 1 (default):   

```
dtoverlay=ov9281,cam1
```

camera 0 (default):   
```
dtoverlay=ov9281,cam0
```
3.Pressing CTRL+ x key to exit and pressing  y’ key to save your changes. Finally, reboot.

```
sudo reboot
```

### 2. Camera Information

```
rpicam-hello --list-cameras
```

### 3. Camera Preview

For camera 0: 

```
rpicam-hello -t 0 --camera 0
```

For camera 1:

```
rpicam-hello -t 0 --camera 1
```

### 4. Camera Mode Setting

The CAM-MIPIOV9281 V2 supports the following operating modes under the built-in drivers of the Raspberry Pi.

RAW8 :  640x400--309.79 fps

​               1280x720--171.79 fps

​               1280x800--143.66 fps

RAW10: 640x400--247.83 fps

​               1280x720--137.43 fps

​               1280x800--114.93 fps

Below, I will provide two examples to demonstrate how to set the operating modes.

(1) Set the working mode of camera 1 to 640x480, RAW8, and 300 frame rate using the following command.

```
rpicam-still --viewfinder-mode 640:400:8 --framerate 300 -t 0 --camera 1
```

(2) Set the working mode of camera 1 to 1280x800, RAW10, and 114 frame rate using the following command

```
rpicam-still --viewfinder-mode 1280:800:10 --framerate 114 -t 0 --camera 1
```

### 5.Camera Trigger Mode

The CAM-OV9281RAW camera module can use the trigger functionality of libcamera/rpicam, but some setup is required.

(1) Copy this folder named OV9281_libcamera to the directory in the Raspberry Pi Os, enter the folder directory and change the folder's permissions.

```
cd ov9281_libcamera

sudo chmod -R a+rwx *
```

(2) To check the I2C bus addresses, use the following command. Since two OV9821 cameras are connected, you should see addresses 10 and 11. Typically, address 11 corresponds to camera 1, and address 10 corresponds to camera 0. Generally, these are fixed addresses, but occasionally, after a Raspberry Pi system upgrade, this numbering may change

```
dmesg | grep ov9281
```

(3) If the cameras are not all started within 1 second, the rpicam applications can time out. To prevent this, you must edit a configuration file on any Raspberry Pi(s) with sink cameras. You can follow the official documentation to perform the operation and edit the copy. Alternatively, you can directly use the file provided in the ov9281_libcamera folder.

```
sources ./libcamera-timeout.sh
```

(4) Enable preview windows, Please note that the command to start the preview must be run in the same terminal window as the script that sets the timeout. Do not open a new terminal window to run it.

```
rpicam-hello -t 0 --camera 0
```

  Or

```
rpicam-hello -t 0 --camera 1
```



(5) Start/Stop Trigger Mode.

The usage of the script to start and stop the trigger mode is as follows: 

./ov9281_trigger  [i2c bus] [on/off] 

Open a new terminal window, and based on the previously obtained I2C bus numbers, you can use the following commands to start/stop the trigger for CAMERA0 and CAMERA1. After the trigger is successfully started, you will see the preview window remain still

camera 1 trigger on:

./ov9281_trigger 11 1

camera 1 trigger off:

./ov9281_trigger 11 0

camera 0 trigger on:

./ov9281_trigger 10 1

camera 0 trigger off:

./ov9281_trigger 10 0 

(6) Generate a Trigger Signal

At this point, a trigger signal needs to be provided to the CAM-OV9281RAW module. We have provided a test script that generates a rising edge every 2 seconds on pin 23 of the 40-pin header on the Raspberry Pi to provide the trigger signal.

```
./trig_sig_pin_23.sh
```



### 5.About Innomaker Unique Driver

Some of our long-time customers may know that INNOMAKER has its own set of OV9281 drivers, which support higher frame rates and more operating modes. However, during the actual rollout, we found that many customers still encountered difficulties compiling and using these drivers correctly on the Raspberry Pi system, which caused significant issues for both parties. Therefore, we recommend that customers prioritize using the built-in Raspberry Pi drivers and rpicam. Previous documentation and drivers are  placed in the Legacy folder, and we will continue to maintain it. If you need any assistance, please feel free to contact our technical support team.







​                                                                                                                                                                                            Author : Calvin （calvin@inno-maker.com）

​                                                                                                                                                                                           Support: support@inno-maker.com
