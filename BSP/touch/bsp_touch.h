#ifndef __BSP_TOUCH_H
#define	__BSP_TOUCH_H


#include "stm32f4xx_hal.h"

/******************************* Touch 触摸屏触摸信号指示引脚定义(不使用中断) ***************************/
#define             Touch_PENIRQ_GPIO_PORT                       GPIOB
#define             Touch_PENIRQ_GPIO_PIN                        GPIO_PIN_1

/******************************* 声明 Touch 相关的数据类型 ***************************/
//触屏信号有效电平
#define             Touch_PENIRQ_ActiveLevel                     0
#define             Touch_PENIRQ_Read()                          HAL_GPIO_ReadPin( Touch_PENIRQ_GPIO_PORT, Touch_PENIRQ_GPIO_PIN )

typedef	struct          //液晶坐标结构体 
{		
	/*负数值表示无新数据*/
   int16_t x;			//记录最新的触摸参数值
   int16_t y; 
	
	/*用于记录连续触摸时(长按)的上一次触摸位置*/
   int16_t pre_x;		
   int16_t pre_y;
	
} strType_Touch_Coordinate;   

typedef struct 
{
    uint8_t sta;    //触摸状态，0未按压，1已按压
    uint16_t x;     //触摸点的x坐标
    uint16_t y;     //触摸点的y坐标
}Touch_Sta_t;

extern Touch_Sta_t Touch_Dev;

/******触摸状态机相关******/
typedef enum
{
	Touch_STATE_RELEASE  = 0,	//触摸释放
	Touch_STATE_WAITING,			//触摸按下
	Touch_STATE_PRESSED,			//触摸按下
}enumTouchState	;

#define TOUCH_PRESSED 				1
#define TOUCH_NOT_PRESSED			0

//触摸消抖阈值
#define DURIATION_TIME				2

/******************************* 声明 Touch 相关的外部全局变量 ***************************/




/******************************** Touch 触摸屏函数声明 **********************************/
void Touch_Init( void );
uint8_t Touch_TouchDetect(Touch_Sta_t *touch_dev);
uint8_t Touch_Get_TouchedPoint(Touch_Sta_t *touch_dev);

#endif /* __BSP_TOUCH_H */

