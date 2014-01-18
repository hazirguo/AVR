/*
 * showimg.c
 *
 * Created: 2014-01-10 11:01:11 AM
 *  Author: guohailin
 */ 

#include "sysdef.h"

//����ͼƬ��ŵ�λ��
//	��һάΪ������ͼƬ
//	�ڶ�άΪ����������ʾ��ͼƬ
uchar work_image_pos[] = { 7,13, 19, 25 };

//����ͼƬ�л�����
uchar work_image_cnt = 0;

//����ͼƬ�ܵ�������һ��ģʽ�£�
#define WORK_IMAGE_NUM  5

//�������������ʽ
uchar clear_screeen_command[] = { 0x55, 0x52, 0x4c, 0x59, 0x44, 0x5a };

//����ͼƬ�����ʽ�� ����λΪͼƬ����ţ�����λΪ 0x8f-���
uchar image_command[] = { 0x55, 0x70, 0, 0, 0x4c, 0x59, 0x44, 0x5a };

//�ı��������ģʽ��
uchar text_mode_command[WORK_MODE_LAST][15] = 
{
	// 0     1   2  3  4  5   6     7     8     9     10    11    12    13    14       
	{0x55, 0x6C, 0, 0, 0, 0, 0xb4, 0xf3, 0xcf, 0xef, 0x00, 0x4c, 0x59, 0x44, 0x5a},		//����
	{0x55, 0x6C, 0, 0, 0, 0, 0xd7, 0xaa, 0xd5, 0xdb, 0x00, 0x4c, 0x59, 0x44, 0x5a},		//ת��
	{0x55, 0x6C, 0, 0, 0, 0, 0xbd, 0xb5, 0xce, 0xc2, 0x00, 0x4c, 0x59, 0x44, 0x5a}		//�¿�
};	

//�����ı������ʽ�� ʱ����ʾ��ʽΪ��    00:00																
uchar text_time_command[] = { 
								0x55, 0x6d,	//[0]:��ʼλ��[1]:�ı�����
								0, 0, 0, 0,		//[2-5]:λ��
							   // 6    7  |  8     9   | 10    11  | 12    13  | 14    15
								0xa3, 0xb0, 0xa3, 0xb0, 0xa3, 0x3A, 0xa3, 0xb0, 0xa3, 0xb0,		//0 0 ��0 0
								0x00,		//[16]��У��
								0x4c, 0x59, 0x44, 0x5a		//[17-20]������λ
							};


#define CUT_IMAGE_XS	0x00, 0xFA
#define CUT_IMAGE_XE	0x02, 0x0D
#define CUT_IMAGE_YS	0x01, 0x3B
#define CUT_IMAGE_YE	0x01, 0xE0
#define CUT_SHOW_X		0x00, 0xFA
#define CUT_SHOW_Y		0x01, 0x3B

//����ͼƬ������
uchar cut_image_command[] = {
								0x55, 0x71, //[0]:��ʼλ��[1]:����ͼƬ����
								0,			//[2]:����ͼƬ�����
								CUT_IMAGE_XS, CUT_IMAGE_YS,		//[3-6]: ���еĿ�ʼλ�ã�Xs, Ys)
								CUT_IMAGE_XE, CUT_IMAGE_YE,		//[7-10]: ���еĽ���λ�ã�Xe, Ye)
								CUT_SHOW_X, CUT_SHOW_Y,			//[11-14]: ճ����λ�ã�X, Y)
								0,			//[15]:У��λ
								0x4c, 0x59, 0x44, 0x5a			//[16-19]:����λ
							};


//����ͼƬ�����
#define START_IMAGE        01
	
//����ͼƬ�Ŀ�ʼ��ţ�����ͼƬ���������
#define SCREEN_IMAGE_START 30
//����ͼƬ������
#define SCREEN_IMAGE_NUM   10

//���ֲ�ͬ����ģʽ�����ֵ���ʼλ�ã�X, Y����ÿ��ֵ�����ֽڱ�ʾ
#define TEXT_START_X11     0x00       
#define TEXT_START_X12     0x98
#define TEXT_START_Y11     0x01
#define TEXT_START_Y12	   0x04

#define TEXT_START_X21    0x01
#define TEXT_START_X22    0x50
#define TEXT_START_Y21    0x01
#define TEXT_START_Y22	  0x04

#define TEXT_START_X31    0x02
#define TEXT_START_X32    0x1A
#define TEXT_START_Y31    0x01
#define TEXT_START_Y32	  0x04

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
	send_command(clear_screeen_command);
	
	Delay_ms(100);
	
	image_command[2] = START_IMAGE;
	image_command[3] = 0xFF - image_command[1] - image_command[2];
	send_command(image_command);
}

//����״̬����ʾ, ͼƬ + ����ʱ 
//	  �Ӷ�����ʾ
void show_work_image(enum WORK_MODE mode, uchar count)
{
	//������ʾͼƬ
	//	���빤��״̬����ʾһ�ŵ�Ƭ
	//	����������ͼƬ������ͼƬ����ʾ
	//	������������ʾ�ض���ͼƬ
	if (count == time_set) {
		image_command[2] = work_image_pos[mode];
		image_command[3] = 0x8F - image_command[2];
		work_image_cnt = 0;
		send_command(image_command);
	} else {
		
		if (count == 0) {
			cut_image_command[2] = work_image_pos[mode] - 1;
		} else {		
			work_image_cnt++;
			cut_image_command[2] = work_image_pos[mode] + work_image_cnt%WORK_IMAGE_NUM;
		}		
		cut_image_command[15] = (0xff - (cut_image_command[1] + cut_image_command[2] + cut_image_command[3] + cut_image_command[4] + cut_image_command[5] +
										 cut_image_command[6] + cut_image_command[7] + cut_image_command[8] + cut_image_command[9] + cut_image_command[10] + 
										 cut_image_command[11] + cut_image_command[12] + cut_image_command[13] + cut_image_command[14])%256 );
		send_command(cut_image_command);
	}
	
	Delay_ms(100);
	
	//��ʾͼƬ�ϵĹ���ģʽ����
	text_mode_command[mode][2] = text_start_position[0][0];
	text_mode_command[mode][3] = text_start_position[0][1];
	text_mode_command[mode][4] = text_start_position[0][2];
	text_mode_command[mode][5] = text_start_position[0][3];
	text_mode_command[mode][10] = (0xff - (text_mode_command[mode][1] + text_mode_command[mode][2] + text_mode_command[mode][3] + text_mode_command[mode][4] + text_mode_command[mode][5] + 
											text_mode_command[mode][6] + text_mode_command[mode][7] + text_mode_command[mode][8] + text_mode_command[mode][9])%256 );
	send_command(text_mode_command[mode]);
	
	Delay_ms(20);
	
	//��ʾͼƬ�ϵĵ���ʱ����
	text_time_command[2] = text_start_position[2][0];
	text_time_command[3] = text_start_position[2][1];
	text_time_command[4] = text_start_position[2][2];
	text_time_command[5] = text_start_position[2][3];
	text_time_command[7] = count/60/10 + 0xB0;
	text_time_command[9] = count/60%10 + 0xB0;
	text_time_command[11] = 0xB0;						//":"
	text_time_command[13] = count%60/10 + 0xB0;
	text_time_command[15] = count%60%10 + 0xB0;
	text_time_command[16] = (0xff - (text_time_command[1] + text_time_command[2] + text_time_command[3] + text_time_command[4] + text_time_command[5] + text_time_command[6] +
									text_time_command[7] + text_time_command[8] + text_time_command[9] + text_time_command[10] + text_time_command[11] +
									text_time_command[12] + text_time_command[13] + text_time_command[14] + text_time_command[15])%256 );
	send_command(text_time_command);
	
	Delay_ms(50);
	
	//��ʾͼƬ�ϵ�����ʱ������
	
	text_time_command[2] = text_start_position[1][0];
	text_time_command[3] = text_start_position[1][1];
	text_time_command[4] = text_start_position[1][2];
	text_time_command[5] = text_start_position[1][3];
	count = time_set - count;
	text_time_command[7] = count/60/10 + 0xB0;
	text_time_command[9] = count/60%10 + 0xB0;
	text_time_command[11] = 0xB0;					//":"
	text_time_command[13] = count%60/10 + 0xB0;
	text_time_command[15] = count%60%10 + 0xB0;
	text_time_command[16] = (0xff - (text_time_command[1] + text_time_command[2] + text_time_command[3] + text_time_command[4] + text_time_command[5] + text_time_command[6] +
									text_time_command[7] + text_time_command[8] + text_time_command[9] + text_time_command[10] + text_time_command[11] +
									text_time_command[12] + text_time_command[13] + text_time_command[14] + text_time_command[15])%256 );
	send_command(text_time_command);
	
}

//����ͼƬ��ʾ
void show_screen_image()
{
	static uchar show_num = 0;
	image_command[2] = SCREEN_IMAGE_START + show_num;
	image_command[3] = 0x8F - image_command[2];
	send_command(image_command);
	if (++show_num >= SCREEN_IMAGE_NUM)
		show_num = 0;
}