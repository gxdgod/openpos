
#ifndef __S3C2410_LCD_T6963__
#define __S3C2410_LCD_T6963__

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_VERSION		"0.1"
#define LCDT6963_MINOR 253
#define DEVICE_NAME	"/dev/s3c2410-lcd-t6963"




#define S3C2410_LCD_IOCTL_BASE	'T'

#define LCD_128240			2
#define LCD240_CHECKBUSY	6000
#define LCD240_LCDSUCESS	0x00
#define LCD240_LCDBUSY		0x01
//#define pLCD240_BASE_DATA	vLCD_T6963_DATA_BASE//LCD240_BASE_CS 	1000 2000-1000 23ff
//#define pLCD240_BASE_CMD        vLCD_T6963_CMD_BASE  //	LCD240_COMPORT		1000 2400-1000 27ff

#define LCD240_CMDWR		0x01
#define LCD240_CMDRD		0x02
#define LCD240_CMDCE		0x04
#define LCD240_CMDCD		0x08
#define LCD240_CMDFS		0x10



#define S3C2410_LCD_CMD		_IOW(S3C2410_LCD_IOCTL_BASE, 0, sizeof(int))
#define S3C2410_LCD_DATA	_IOW(S3C2410_LCD_IOCTL_BASE, 1, sizeof(int))
#define S3C2410_LCD_STAUTE	_IOR(S3C2410_LCD_IOCTL_BASE, 2, sizeof(int))
#define S3C2410_LCD_CLS		_IOR(S3C2410_LCD_IOCTL_BASE, 3, sizeof(int))


#define LCD_EN 1
#define LCD_DIS 0

#define LCD_PORT 0x10000000		//���ݵ�·ͼȷ��
#define LCD_RD_PORT LCD_PORT & (1<<1)
#define LCD_WR_PORT LCD_PORT

#ifdef __cplusplus
extern "C" {
#endif

#endif