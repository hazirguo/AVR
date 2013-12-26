/*
 * lcd1602.h
 *
 * Created: 2013-12-26 4:50:57 PM
 *  Author: guohailin
 */ 


#ifndef LCD1602_H_
#define LCD1602_H_

typedef unsigned int	uint;
typedef unsigned char	uchar;

// 端口位定义
#define RS PA4     //数据/指令控制端：0命令、1数据
#define RW PA5	   //读/写选择控制端：0写、1读
#define E  PA6	   //使能端：下降沿读、高电平写

// 函数声明
void LCDPort_Init(void);
void LCD_Init(void);
void Write_Com(uchar LCD_Com);
void Write_Data_Byte(uchar LCD_Data);
void Write_Placce_xy(uchar x, uchar y);
void Write_Data_String(uchar x, uchar y, uchar *data);
void Write_Data_Char(uchar x, uchar y, uchar data);
void Check_Busy(void);
void Delay_us(uint us);
void Delay_ms(uint ms);

#endif /* LCD1602_H_ */