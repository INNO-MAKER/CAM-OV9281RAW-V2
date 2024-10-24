DEV="$(v4l2-ctl  --list-devices |grep -A 9 1f00128000 |grep media |tr -d '[:space:]')"
media-ctl  -d $DEV -r
media-ctl -d $DEV -l ''\''csi2'\'':4->'\''rp1-cfe-csi2_ch0'\'':0[1]'
media-ctl -v -d $DEV -V ''\''csi2'\'':0[fmt:Y8_1X8/1280x800 field:none]'
media-ctl -v -d $DEV  -V ''\''csi2'\'':4[fmt:Y8_1X8/1280x800 field:none]'
v4l2-ctl -v width=1280,height=800,pixelformat=GREY
