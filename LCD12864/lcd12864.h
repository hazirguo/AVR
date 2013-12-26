/*
 * lcd12864.h
 *
 * Created: 2013-12-26 7:38:20 PM
 *  Author: guohailin
 */ 


#ifndef LCD12864_H_
#define LCD12864_H_

typedef unsigned int	uint;
typedef unsigned char	uchar;

// �˿�λ����
#define RS PA4     //����/ָ����ƶˣ�0���1����
#define RW PA5	   //��/дѡ����ƶˣ�0д��1��
#define E  PA6	   //ʹ�ܶˣ��½��ض����ߵ�ƽд

#define PSB PA3	   //����/���ڷ�ʽѡ��0���ڡ�1����

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



#endif /* LCD12864_H_ */