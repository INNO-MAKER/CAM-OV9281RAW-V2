Use_innomaker_unique_driver_on_pi5

Step1 ,  Follow CAM-MIPI9281V2-Compile Driver Source Code.PDF compiler and install driver;

Step2,   run both below 3 scripts # CSI 1

# For csi1
ov9281-pi5-initial.sh
ov9281-pi5-raw.sh


# For csi0
ov9281-pi5-csi0-initial.sh
ov9281-pi5-raw.sh

Step3, copy dtb files

# CSI0
copy inno_mipi_ov9281_csi0.dtbo to folder  /boot/overlays  
add dtoverlay=inno_mipi_ov9281_csi0 to last line.

# CSI1
copy inno_mipi_ov9281.dtbo to folder  /boot/overlays  
add dtoverlay=inno_mipi_ov9281 to last line.

#If use CSI0,CSI1 Both,  need run both command.

Step4,  Set working mode

# For PI4
Follow CAM-MIPIOV9281 V2 User Manual V1.2.pdf Chapter 4.11 to set working mode and preview

# For PI5
sudo make -f Makefile_pi5 setmodeX  #X means working mode 0,1,2,3,etc...