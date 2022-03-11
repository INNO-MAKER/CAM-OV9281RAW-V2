
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/videodev2.h>
#include <getopt.h>
#include <errno.h>
#include <syslog.h>
#include <string.h>
#include <stdlib.h>
 
#define u8 unsigned char
#define  LOGD(...)  do {printf(__VA_ARGS__);printf("\n");} while (0)
#define DBG(fmt, args...) LOGD("%s:%d, " fmt, __FUNCTION__, __LINE__, ##args);
#define ASSERT(b) \
do \
{ \
    if (!(b)) \
    { \
        LOGD("error on %s:%d", __FUNCTION__, __LINE__); \
        return 0; \
    } \
} while (0)
 
#define VIDEO_DEVICE "/dev/video0"
#define IMAGE_WIDTH 1280//sensor固定输出1920*1080的图像
#define IMAGE_HEIGHT 800
#define IMAGE_SIZE (IMAGE_WIDTH * IMAGE_HEIGHT) +(IMAGE_WIDTH*IMAGE_HEIGHT)/4
//#define IMAGE_SIZE (IMAGE_WIDTH * IMAGE_HEIGHT * 1.25)
//#define IMAGE_SIZE (IMAGE_WIDTH * IMAGE_HEIGHT * 2)
//#define BUFFER_COUNT 5//申请5个缓冲区
#define BUFFER_COUNT 3//申请5个缓冲区


#define  DEMO_NAME          "mipi raw capture demo"
#define  DEMO_MAINVERSION    (  0)  /**<  Main Version: X.-.-   */
#define  DEMO_VERSION        (  0)  /**<       Version: -.X.-   */
#define  DEMO_SUBVERSION     (  3)  /**<    Subversion: -.-.X   */
 
int cam_fd = -1;
struct v4l2_buffer video_buffer[BUFFER_COUNT];
u8* video_buffer_ptr[BUFFER_COUNT];
u8 buf[IMAGE_SIZE];
 
int cam_open()
{
    cam_fd = open(VIDEO_DEVICE, O_RDWR);//打开摄像头
 
    if (cam_fd >= 0) return 0;
    else return -1;
}
 
int cam_close()
{
    close(cam_fd);//关闭摄像头
 
    return 0;
}
 
int cam_select(int index)
{
    int ret;
 
    int input = index;
    ret = ioctl(cam_fd, VIDIOC_S_INPUT, &input);//设置输入源
    return ret;
}
 
int cam_init()
{
    int i;
    int ret;
    struct v4l2_format format;
 
    memset(&format, 0, sizeof(format));
    format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;//帧类型，用于视频捕获设备
    //format.fmt.pix.pixelformat = V4L2_PIX_FMT_SBGGR8;
	//format.fmt.pix.pixelformat = V4L2_PIX_FMT_SGRBG10;//10bit raw格式
	//format.fmt.pix.pixelformat = V4L2_PIX_FMT_SRGGB10P;//10bit raw格式
	format.fmt.pix.pixelformat = V4L2_PIX_FMT_Y10;//10bit raw格式
    format.fmt.pix.width = IMAGE_WIDTH;//分辨率
    format.fmt.pix.height = IMAGE_HEIGHT;
    ret = ioctl(cam_fd, VIDIOC_TRY_FMT, &format);//设置当前格式
    if (ret != 0)
    {
        DBG("ioctl(VIDIOC_TRY_FMT) failed %d(%s)", errno, strerror(errno));
        return ret;
    }
 
    format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ret = ioctl(cam_fd, VIDIOC_S_FMT, &format);//设置当前格式
    if (ret != 0)
    {
        DBG("ioctl(VIDIOC_S_FMT) failed %d(%s)", errno, strerror(errno));
        return ret;
    }
 
    struct v4l2_requestbuffers req;
    req.count = BUFFER_COUNT;//缓冲帧个数
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;//缓冲帧数据格式
    req.memory = V4L2_MEMORY_MMAP;//内存映射方式
    ret = ioctl(cam_fd, VIDIOC_REQBUFS, &req);//申请缓冲区
    if (ret != 0)
    {
        DBG("ioctl(VIDIOC_REQBUFS) failed %d(%s)", errno, strerror(errno));
        return ret;
    }
    DBG("req.count: %d", req.count);
    if (req.count < BUFFER_COUNT)
    {
        DBG("request buffer failed");
        return ret;
    }
 
    struct v4l2_buffer buffer;
    memset(&buffer, 0, sizeof(buffer));
    buffer.type = req.type;
    buffer.memory = V4L2_MEMORY_MMAP;
    for (i=0; i<req.count; i++)
    {
        buffer.index = i;
        ret = ioctl (cam_fd, VIDIOC_QUERYBUF, &buffer);//获取缓冲帧地址
        if (ret != 0)
        {
            DBG("ioctl(VIDIOC_QUERYBUF) failed %d(%s)", errno, strerror(errno));
            return ret;
        }
        DBG("buffer.length: %d", buffer.length);
        DBG("buffer.m.offset: %d", buffer.m.offset);
        video_buffer_ptr[i] = (u8*) mmap(NULL, buffer.length, PROT_READ|PROT_WRITE, MAP_SHARED, cam_fd, buffer.m.offset);//内存映射
        if (video_buffer_ptr[i] == MAP_FAILED)
        {
            DBG("mmap() failed %d(%s)", errno, strerror(errno));
            return -1;
        }
 
        buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buffer.memory = V4L2_MEMORY_MMAP;
        buffer.index = i;
        ret = ioctl(cam_fd, VIDIOC_QBUF, &buffer);//把缓冲帧放入队列中
        if (ret != 0)
        {
            DBG("ioctl(VIDIOC_QBUF) failed %d(%s)", errno, strerror(errno));
            return ret;
        }
    }
 
    int buffer_type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ret = ioctl(cam_fd, VIDIOC_STREAMON, &buffer_type);//启动数据流
    if (ret != 0)
    {
        DBG("ioctl(VIDIOC_STREAMON) failed %d(%s)", errno, strerror(errno));
        return ret;
    }
 
    DBG("cam init done.");
 
    return 0;
}
 
int cam_get_image(u8* out_buffer, int out_buffer_size)
{
    int ret;
    struct v4l2_buffer buffer;
 
    memset(&buffer, 0, sizeof(buffer));
    buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buffer.memory = V4L2_MEMORY_MMAP;
    buffer.index = BUFFER_COUNT;
    ret = ioctl(cam_fd, VIDIOC_DQBUF, &buffer);//从队列中取出一帧
    if (ret != 0)
    {
        DBG("ioctl(VIDIOC_DQBUF) failed %d(%s)", errno, strerror(errno));
        return ret;
    }
 
    if (buffer.index < 0 || buffer.index >= BUFFER_COUNT)
    {
        DBG("invalid buffer index: %d", buffer.index);
        return ret;
    }
 
    DBG("dequeue done, index: %d", buffer.index);
    memcpy(out_buffer, video_buffer_ptr[buffer.index], IMAGE_SIZE);//缓冲帧数据拷贝出来
    DBG("copy done.");
 
    ret = ioctl(cam_fd, VIDIOC_QBUF, &buffer);//缓冲帧放入队列
    if (ret != 0)
    {
        DBG("ioctl(VIDIOC_QBUF) failed %d(%s)", errno, strerror(errno));
        return ret;
    }
    DBG("enqueue done.");
 
    return 0;
}

//ouyang add for free memory
void memory_free(void)
{
int i,ret;
for (i=0; i<BUFFER_COUNT; i++)
{
 ret=munmap(video_buffer_ptr[i],IMAGE_SIZE);
 if (ret != 0)
 { DBG("Munmap failed!!."); }
 else
 {  DBG("Munmap Success!!.");}
 //free();
}


}

int  change_options_by_commandline(int argc, char *argv[], int *shutter, float *gain, int *hflip, int *vflip, int *capcnt)
{
	int  opt;

	while((opt =  getopt(argc, argv, "g:s:h:v:c:")) != -1)
	{
		switch(opt)
		{
			default:
				printf("_______________________________________________________________________________\n");
				printf("                                                                               \n");
				printf("  %s v.%d.%d.%d.\n", DEMO_NAME, DEMO_MAINVERSION, DEMO_VERSION, DEMO_SUBVERSION);
				printf("  -----------------------------------------------------------------------------\n");
				printf("                                                                               \n");
				printf("  Usage: %s [-s sh] [-g gain] [-h f] [-v f] [-c cnt]\n", argv[0]);
				printf("                                                                               \n");
				printf("  -s,  Shutter Time.                                                           \n");
				printf("  -g,  Gain Value.                                                           \n");
				printf("  -h,  horizen flip.                                                    \n");
				printf("  -v,  vertical flip       \n");
				printf("  -c,  capture count \n");
					printf("_______________________________________________________________________________\n");
				printf("                                                                               \n");
				return(+1);
			case 's':  *shutter    = atol(optarg);  printf("Setting Shutter Value to %d.\n",*shutter);  break;
			case 'g':  *gain       = atof(optarg);  printf("Setting Gain Value to %f.\n",   *gain   );  break;
			case 'h':  *hflip      = atol(optarg);  printf("Horizen flip the captured image.\n");       break;
			case 'v':  *vflip      = atol(optarg);  printf("Vertical flip the captured image.\n" );     break;
			case 'c':  *capcnt     = atol(optarg);  printf("Capture %d frame.\n",*capcnt );             break;
			
		}
	}

	if(argc<2)
	{
		printf("  Hint: Incorrect command line option (see:  %s -? )\n", argv[0]);
	}

	return(0);
}


int  sensor_set_parameters(int optGain, int optShutter,int opthflip,int optvflip)
{
	int    ee, rc, target;
	unsigned int    ctlID, val;
	char   a10cTarget[11];
	struct v4l2_control  ctl;


	for(target= 0; target< 4; target++)
	//for(target= 0; target< 2; target++)
	{
		switch(target)
		{
			case 0:  sprintf(a10cTarget,"Gain"    );  ctlID = V4L2_CID_GAIN;      val = optGain;     break;
			case 1:  sprintf(a10cTarget,"Exposure");  ctlID = V4L2_CID_EXPOSURE;  val = optShutter;
  break;    case 2:  sprintf(a10cTarget,"Hflip");     ctlID = V4L2_CID_HFLIP;     val = opthflip;
  break;    case 3:  sprintf(a10cTarget,"Vflip");     ctlID = V4L2_CID_VFLIP;     val = optvflip;
  break;
		}

		// Only needed for debugging: Get old value.
		//if((target!=2)&&(target!=3))
		{
			memset(&ctl, 0, sizeof(ctl));
			ctl.id = ctlID;

			rc =  ioctl(cam_fd , VIDIOC_G_CTRL, &ctl);
			if(rc<0)
			{
				if(EINVAL!=errno){ee=-1; goto fail;} //general error.
				else             {ee=-2; goto fail;} //unsupported.
			}

			//syslog(LOG_DEBUG, "%s():  Old %s Value: %d.\n", __FUNCTION__, a10cTarget, ctl.value);
			printf( "%s():  Old %s Value: %d.\n", __FUNCTION__, a10cTarget, ctl.value);
		}

		// Set new value.
		{
			memset(&ctl, 0, sizeof(ctl));
			ctl.id    = ctlID;
		//if((target==2)&&(target==3))
		//	{
		//    if (val>0)	
		 //   ctl.value=1;
		//	else
		 //   ctl.value=0;
		//	}
		//else	
		    ctl.value = val;

			rc = ioctl(cam_fd , VIDIOC_S_CTRL, &ctl);
			if(rc<0)
			{
				if((EINVAL!=errno)&&(ERANGE!=errno)){ee=-3; goto fail;} //general error.
				else                                {ee=-4; goto fail;} //Value out of Range Error.
			}

			//syslog(LOG_DEBUG, "%s():  Requested New %s Value: %d.\n", __FUNCTION__, a10cTarget, val);
			printf( "%s():  Requested New %s Value: %d.\n", __FUNCTION__, a10cTarget, val);
		}

		// Only needed for debugging: Get new value.
		//if((target!=2)&&(target!=3))
		{
			memset(&ctl, 0, sizeof(ctl));
			ctl.id = ctlID;

			rc =  ioctl(cam_fd , VIDIOC_G_CTRL, &ctl);
			if(rc<0)
			{
				if(EINVAL!=errno){ee=-5; goto fail;} //general error.
				else             {ee=-6; goto fail;} //unsupported.
			}

			//syslog(LOG_DEBUG, "%s():  New %s Value: %d.\n", __FUNCTION__, a10cTarget, ctl.value);
			printf( "%s():  New %s Value: %d.\n", __FUNCTION__, a10cTarget, ctl.value);
		}
	}


	ee = 0;
fail:
	switch(ee)
	{
		case 0:
			break;
		case -1:
		case -3:
		case -5:
			//syslog(LOG_ERR, "%s():  ioctl(%s) throws Error (%d(%s))!\n", __FUNCTION__, (-3==ee)?("VIDIOC_S_CTRL"):("VIDIOC_G_CTRL"), errno, strerror(errno));
			printf( "%s():  ioctl(%s) throws Error (%d(%s))!\n", __FUNCTION__, (-3==ee)?("VIDIOC_S_CTRL"):("VIDIOC_G_CTRL"), errno, strerror(errno));
			break;
		case -2:
		case -6:
			//syslog(LOG_ERR, "%s():  V4L2_CID_.. is unsupported!\n", __FUNCTION__);
			printf( "%s():  V4L2_CID_.. is unsupported!\n", __FUNCTION__);
			break;
		case -4:
			//syslog(LOG_ERR, "%s():  %s Value is out of range (or V4L2_CID_.. is invalid)!\n", __FUNCTION__, a10cTarget);
			printf( "%s():  %s Value is out of range (or V4L2_CID_.. is invalid)!\n", __FUNCTION__, a10cTarget);
			break;
	}

	return(ee);
}


 
//int main()
int  main(int argc, char *argv[])
{
    int i,j;
    int ret;
    int    optShutter=5000;
	int    opthflip=0;
	int    optvflip=0;
	int    optcapcnt=2;
    float  optGain=136;

    ret =  change_options_by_commandline(argc, argv, &optShutter, &optGain, &opthflip, &optvflip, &optcapcnt);
    ASSERT(ret==0);

    ret = cam_open();
    ASSERT(ret==0);

    ret =  sensor_set_parameters( optGain, optShutter,opthflip, optvflip);
    ASSERT(ret==0);

    ret = cam_select(0);
    ASSERT(ret==0);
 
    ret = cam_init();
    ASSERT(ret==0);
 
    while (1) {
    int count = 0;
    for(j=0;j<optcapcnt;j++)
    {
        ret = cam_get_image(buf, IMAGE_SIZE);
        ASSERT(ret==0);
 
        char tmp[64] = {"---\n"};
        for (i=0; i<16; i++)
            sprintf(&tmp[strlen(tmp)], "%02x ", buf[i]);
        LOGD("%s", tmp);
 
        char filename[32];
        //sprintf(filename, "/sdcard/%05d.raw", count++);
        sprintf(filename, "./%05d.raw", count++);
        //sprintf(filename, "./%05d.raw", count);
        int fd = open(filename,O_WRONLY|O_CREAT,00700);//保存图像数据
        if (fd >= 0)
        {
            write(fd, buf, IMAGE_SIZE);
            close(fd);
        }
        else
        {
            LOGD("open() failed: %d(%s)", errno, strerror(errno));
        }
    }
	  } //while end
    //ouyang add here
    memory_free();
    ret = cam_close();
    ASSERT(ret==0);
 
    return 0;
}
