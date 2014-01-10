/*
 * showimg.c
 *
 * Created: 2014-01-10 11:01:11 AM
 *  Author: guohailin
 */ 

#include "sysdef.h"

//发送图片命令格式， 第三位为图片的序号，第四位为 0x8f-序号
uchar image_command[] = { 0x55, 0x70, 0, 0, 0x4c, 0x59, 0x44, 0x5a };

//发送文本命令格式
uchar text_command[] = { 0x55, 0x6C, 0, 0, 0, 0, 0x93, 0x93, 0x93, 0x4c, 0x59, 0x44, 0x5a };

//开机图片的序号
#define START_IMAGE        01

//工作图片的序号
#define WORK_IMAGE         02
	
//屏保图片的开始序号（屏保图片存放连续）
#define SCREEN_IMAGE_START 30
//屏保图片的张数
#define SCREEN_IMAGE_NUM   10

//三种不同工作模式下文字的起始位置（X, Y），每个值两个字节表示
#define TEXT_START_X11     0x00       
#define TEXT_START_X12     0x00
#define TEXT_START_Y11     0x00
#define TEXT_START_Y12	   0x00

#define TEXT_START_X21     0x00
#define TEXT_START_X22     0x00
#define TEXT_START_Y21     0x00
#define TEXT_START_Y22	   0x00

#define TEXT_START_X31     0x00
#define TEXT_START_X32     0x00
#define TEXT_START_Y31     0x00
#define TEXT_START_Y32	   0x00

//文字起始位置
uchar text_start_position[][4] = {
	{TEXT_START_X11, TEXT_START_X12,  TEXT_START_Y11, TEXT_START_Y12}, 
	{TEXT_START_X21, TEXT_START_X22,  TEXT_START_Y21, TEXT_START_Y22}, 
	{TEXT_START_X31, TEXT_START_X32,  TEXT_START_Y31, TEXT_START_Y32}
};

//发送命令，命令格式以 4c 59 44 5a 结束
void send_command(uchar *cmd)
{
	uchar a1, a2, a3, a4;
	while ( (a1!=0x4c) || (a2!=0x59) || (a3!=0x44) || (a4!=0x5a) )
	{
		a1 = a2; a2 = a3; a3 = a4; a4 = *cmd;
		cmd++;
		ustra_transmit(a4);
	}
}

//开机图片显示
void show_start_image()
{
	image_command[1] = START_IMAGE;
	image_command[2] = 0x8F - image_command[1];
	send_command(image_command);
}

//工作状态下显示, 图片 + 倒计时 
//	  未加动画显示
void show_work_image(enum WORK_MODE mode, uchar count)
{
	image_command[2] = WORK_IMAGE;
	image_command[3] = 0x8F - image_command[1];
	send_command(image_command);
	
	text_command[4] = text_start_position[mode][0];
	text_command[5] = text_start_position[mode][1];
	text_command[6] = text_start_position[mode][2];
	text_command[7] = text_start_position[mode][3];
	
	text_command[8] = count/100 + 0x30;
	text_command[9] = count%100/10 + 0x30;
	text_command[10] = count%10 + 0x30;
	send_command(text_command);
}

//屏保图片显示
void show_screen_image()
{
	static uchar show_num = 0;
	image_command[2] = SCREEN_IMAGE_START + show_num;
	image_command[3] = 0x8F - image_command[1];
	send_command(image_command);
	if (++show_num >= SCREEN_IMAGE_NUM)
		show_num = 0;
}