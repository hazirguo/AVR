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

// �˿�λ����
#define RS PA4     //����/ָ����ƶˣ�0���1����
#define RW PA5	   //��/дѡ����ƶˣ�0д��1��
#define E  PA6	   //ʹ�ܶˣ��½��ض����ߵ�ƽд

// ��������
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