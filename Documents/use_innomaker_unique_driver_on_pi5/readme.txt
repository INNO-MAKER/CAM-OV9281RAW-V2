Use_innomaker_unique_driver_on_pi5

Step1 ,  Follow CAM-MIPI9281V2-Compile Driver Source Code.PDF compiler and install driver;


Step2,   run below scripts accordinglly

# For csi0
./ov9281-pi5-csi0-initial.sh

# For csi1
./ov9281-pi5-csi1-initial.sh




Step3, copy dtb files to overlays folder

# For csi0
copy inno_mipi_ov9281_csi0.dtbo to folder  /boot/overlays  
add dtoverlay=inno_mipi_ov9281_csi0 to last line.

# For csi1
copy inno_mipi_ov9281.dtbo to folder  /boot/overlays  
add dtoverlay=inno_mipi_ov9281 to last line.

If use CSI0,CSI1 Both,  need run both command.



Step4,  Set working mode

# For PI5 Only
sudo make -f Makefile_pi5 setmodeX  #X means working mode 0,1,2,3,etc...