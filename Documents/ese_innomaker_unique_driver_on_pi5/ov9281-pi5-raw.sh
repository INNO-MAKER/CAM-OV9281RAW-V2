
media0:

media-ctl  -d /dev/media0 -r


media-ctl -d /dev/media0 -l ''\''csi2'\'':4->'\''rp1-cfe-csi2_ch0'\'':0[1]'

media-ctl -v -d /dev/media0 -V ''\''csi2'\'':0[fmt:Y8_1X8/1280x800 field:none]'
media-ctl -v -d /dev/media0 -V ''\''csi2'\'':4[fmt:Y8_1X8/1280x800 field:none]'

v4l2-ctl -v width=1280,height=800,pixelformat=GREY


v4l2-ctl --stream-mmap --stream-count=-1  -d /dev/video0 --stream-to=/dev/null

//test with vcmipi tools
./vcmipidemo-pi4-arch64  -s 30000 -g 0x88 -f >/dev/null
v4l2-ctl -d /dev/v4l-subdev2 --set-ctrl 'exposure=800000,gain=200'



media1:
media-ctl  -d /dev/media1 -r


media-ctl -d /dev/media1 -l ''\''csi2'\'':4->'\''rp1-cfe-csi2_ch0'\'':0[1]'

media-ctl -v -d /dev/media1 -V ''\''csi2'\'':0[fmt:Y8_1X8/1280x800 field:none]'
media-ctl -v -d /dev/media1 -V ''\''csi2'\'':4[fmt:Y8_1X8/1280x800 field:none]'

v4l2-ctl -v width=1280,height=800,pixelformat=GREY