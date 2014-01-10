/*
 * showimg.c
 *
 * Created: 2014-01-10 11:01:11 AM
 *  Author: guohailin
 */ 

#include "sysdef.h"

//����ͼƬ�����ʽ�� ����λΪͼƬ����ţ�����λΪ 0x8f-���
uchar image_command[] = { 0x55, 0x70, 0, 0, 0x4c, 0x59, 0x44, 0x5a };

//�����ı������ʽ
uchar text_command[] = { 0x55, 0x6C, 0, 0, 0, 0, 0x93, 0x93, 0x93, 0x4c, 0x59, 0x44, 0x5a };

//����ͼƬ�����
#define START_IMAGE        01

//����ͼƬ�����
#define WORK_IMAGE         02
	
//����ͼƬ�Ŀ�ʼ��ţ�����ͼƬ���������
#define SCREEN_IMAGE_START 30
//����ͼƬ������
#define SCREEN_IMAGE_NUM   10

//���ֲ�ͬ����ģʽ�����ֵ���ʼλ�ã�X, Y����ÿ��ֵ�����ֽڱ�ʾ
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

//������ʼλ��
uchar text_start_position[][4] = {
	{TEXT_START_X11, TEXT_START_X12,  TEXT_START_Y11, TEXT_START_Y12}, 
	{TEXT_START_X21, TEXT_START_X22,  TEXT_START_Y21, TEXT_START_Y22}, 
	{TEXT_START_X31, TEXT_START_X32,  TEXT_START_Y31, TEXT_START_Y32}
};

//������������ʽ�� 4c 59 44 5a ����
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

//����ͼƬ��ʾ
void show_start_image()
{
	image_command[1] = START_IMAGE;
	image_command[2] = 0x8F - image_command[1];
	send_command(image_command);
}

//����״̬����ʾ, ͼƬ + ����ʱ 
//	  δ�Ӷ�����ʾ
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

//����ͼƬ��ʾ
void show_screen_image()
{
	static uchar show_num = 0;
	image_command[2] = SCREEN_IMAGE_START + show_num;
	image_command[3] = 0x8F - image_command[1];
	send_command(image_command);
	if (++show_num >= SCREEN_IMAGE_NUM)
		show_num = 0;
}