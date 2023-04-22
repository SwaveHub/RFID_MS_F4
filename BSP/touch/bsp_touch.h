#ifndef __BSP_TOUCH_H
#define	__BSP_TOUCH_H


#include "stm32f4xx_hal.h"

/******************************* Touch �����������ź�ָʾ���Ŷ���(��ʹ���ж�) ***************************/
#define             Touch_PENIRQ_GPIO_PORT                       GPIOB
#define             Touch_PENIRQ_GPIO_PIN                        GPIO_PIN_1

/******************************* ���� Touch ��ص��������� ***************************/
//�����ź���Ч��ƽ
#define             Touch_PENIRQ_ActiveLevel                     0
#define             Touch_PENIRQ_Read()                          HAL_GPIO_ReadPin( Touch_PENIRQ_GPIO_PORT, Touch_PENIRQ_GPIO_PIN )

typedef	struct          //Һ������ṹ�� 
{		
	/*����ֵ��ʾ��������*/
   int16_t x;			//��¼���µĴ�������ֵ
   int16_t y; 
	
	/*���ڼ�¼��������ʱ(����)����һ�δ���λ��*/
   int16_t pre_x;		
   int16_t pre_y;
	
} strType_Touch_Coordinate;   

typedef struct 
{
    uint8_t sta;    //����״̬��0δ��ѹ��1�Ѱ�ѹ
    uint16_t x;     //�������x����
    uint16_t y;     //�������y����
}Touch_Sta_t;

extern Touch_Sta_t Touch_Dev;

/******����״̬�����******/
typedef enum
{
	Touch_STATE_RELEASE  = 0,	//�����ͷ�
	Touch_STATE_WAITING,			//��������
	Touch_STATE_PRESSED,			//��������
}enumTouchState	;

#define TOUCH_PRESSED 				1
#define TOUCH_NOT_PRESSED			0

//����������ֵ
#define DURIATION_TIME				2

/******************************* ���� Touch ��ص��ⲿȫ�ֱ��� ***************************/




/******************************** Touch �������������� **********************************/
void Touch_Init( void );
uint8_t Touch_TouchDetect(Touch_Sta_t *touch_dev);
uint8_t Touch_Get_TouchedPoint(Touch_Sta_t *touch_dev);

#endif /* __BSP_TOUCH_H */

