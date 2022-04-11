###############################################################################################################
#Description: This codes was designed for help the user Automatic install the OV9281 driver
#             on all Raspbian system. There are two parts in this script. 
#             Part 1 is add the to modify the '/boot/config.txt'and '/boot/cmdline.txt', 
#             Part 2 is automatic the right driver version and install.
#             This script was done in a hurry , if you find some bug, kindly let me know. thanks in advance.
#
#USAGE:      run './autoinstall_driver.sh' on the terminal of Raspbian
#
#Author:      calvin (calvin@inno-maker.com)
#
#data:        2022.03.11
#
###############################################################################################################



#Part 1

echo "------INNO-MAKER cammipi_ov9281 driver install script  v1.0------"

echo "Enable i2c_vc in /boot/config.txt ..."

awk 'BEGIN{ count=0;  }
{
	if ( $1 == "dtparam=i2c_vc=on" )

		count++;

}
END{
	print "count = "count;
	if ( count <= 0 ) {
		print  "--- Add i2c_vc=on to /boot/config.txt";
		system("sudo sh -c \"sed -i.bak \"/^#dtparam=i2c_vc=on/d\" /boot/config.txt\" ");
		system("sudo sh -c \"echo \"dtparam=i2c_vc=on\" >> /boot/config.txt\" ");
	}

}' /boot/config.txt




awk 'BEGIN{ count2=0;  }
{
	if ( $1 == "dtoverlay=vc_mipi_ov9281" )

		count2++;

}
END{
	print "count2 = "count2;
	if ( count <= 0 ) {
		print  "--- Add dtoverlay=vc_mipi_ov9281 to /boot/config.txt";
		system("sudo sh -c \"sed -i.bak \"/^#dtoverlay=vc_mipi_ov9281/d\" /boot/config.txt\" ");
		system("sudo sh -c \"echo \"dtoverlay=vc_mipi_ov9281\" >> /boot/config.txt\" ");
	}

}' /boot/config.txt





str=$(sudo cat /boot/cmdline.txt)
echo "$str"
if [[ "$str" =~ .*cma=.*M ]]; then
	echo "--- alread have cma=128M string"
else
	echo "--- Add cma=128M to /boot/cmdline.txt"
	sudo sed 's/[ \t]*$//g' /boot/cmdline.txt -i.bak
	sudo sed '/console=/s/$/ cma=128M/' /boot/cmdline.txt  -i.bak
fi



#Part 2


strVersion=$(awk -F " " '{print $3}' /proc/version)
strModel=$(cat /proc/device-tree/model)

echo $strVersion
echo $strModel

version=0
model=0

case $strVersion in
    *5.10.92*)
                version=Linux_5.10.92
        ;;   
    *5.15.13*)
                version=Linux_5.15.13
        ;;   

	*)
                echo "@ Linux Version Not Match! Please contact support@inno-maker.com"
        ;;
esac

case $strModel in
	*Pi' '4*)
		model=pi4
	;;
	*Pi' '3*)
		model=pi3
	;;
    *Pi' 'Compute' 'Module' '4*)
		model=pi4
	;;
	
	*Pi' '0*)
		echo "@ This driver do not suppor pi0, Please contact support@inno-maker.com"
	;;
	*)
                echo "@ HW Version Not Match! Please contact support@inno-maker.com"
    ;;
esac


if [[ $version == 0 || $model == 0 ]]; then
	echo "@ Install Failed, Please contact support@inno-maker.com"
	exit
fi


sudo chmod -R 777 ./tools/*

cd ./$version/$model
echo "PWD: "$(pwd)
sudo chmod -R 777 ./*


echo "-----make install----START:"
sudo make install
echo "-----make install----END."


echo "INNO-MAKER: reboot now?(y/n):"
read KB_INPUT
case $KB_INPUT in
	'y'|'Y')
		echo "reboot..."
		sudo reboot
	;;
	*)
		echo "cancel."
	;;
esac


