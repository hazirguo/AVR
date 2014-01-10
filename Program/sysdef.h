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


//系统当前状态
enum SYSTEM_STATE {
	SYS_START,			//开机
	SYS_WORK,			//工作状态
	SYS_INTERIM,		//工作->屏保的过渡状态
	SYS_SCREEN,			//屏保状态
	SYS_STOP			//关机
};

//系统的工作模式
enum WORK_MODE {
	WORK_MODE_DAXIANG,		//大巷喷雾
	WORK_MODE_ZHUANGZAI,	//转载点喷雾
	WORK_MODE_WENKONG,		//皮带防尘防火喷雾
	WORK_MODE_LAST
};

//系统当前状态
extern enum SYSTEM_STATE cur_state;

//系统所处工作模式
extern enum WORK_MODE  work_mode;

//倒计时计数，单位为 s
extern uchar time_count;

//设置的工作时间
extern uchar time_set;

//读取工作模式
//	参数：	无
//	返回值：	工作模式
enum WORK_MODE read_work_mode();

//读取时间设置
//	参数：	工作模式
//	返回值：	设置的时间
uchar read_time_set(enum WORK_MODE mode);

#endif /* MACRO_H_ */