/*
 * sysdef.h
 *
 * Created: 2014-01-10 8:37:55 AM
 *  Author: guohailin
 */ 


#ifndef MACRO_H_
#define MACRO_H_

#define FIVE_SECONDS	5*60
#define FIVE_MININUTES	5*60*60

typedef unsigned char uchar;
typedef unsigned int uint;


//ϵͳ��ǰ״̬
enum SYSTEM_STATE {
	SYS_START,			//����
	SYS_WORK,			//����״̬
	SYS_INTERIM,		//����->�����Ĺ���״̬
	SYS_SCREEN,			//����״̬
	SYS_STOP			//�ػ�
};

//ϵͳ�Ĺ���ģʽ
enum WORK_MODE {
	WORK_MODE_DAXIANG,		//��������
	WORK_MODE_ZHUANGZAI,	//ת�ص�����
	WORK_MODE_WENKONG,		//Ƥ��������������
	WORK_MODE_LAST
};

//ϵͳ��ǰ״̬
extern enum SYSTEM_STATE cur_state;

//ϵͳ��������ģʽ
extern enum WORK_MODE  work_mode;

//����ʱ��������λΪ s
extern uchar time_count;

//���õĹ���ʱ��
extern uchar time_set;

//��ȡ����ģʽ
//	������	��
//	����ֵ��	����ģʽ
enum WORK_MODE read_work_mode();

//��ȡʱ������
//	������	����ģʽ
//	����ֵ��	���õ�ʱ��
uchar read_time_set(enum WORK_MODE mode);

#endif /* MACRO_H_ */