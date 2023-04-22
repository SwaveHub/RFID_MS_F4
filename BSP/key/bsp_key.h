#ifndef __KEY_H
#define	__KEY_H

#include "stm32f4xx_hal.h"
#include "main.h"
//引脚定义
/*******************************************************/
#define SCANKEY_PIN                  KEY_SCAN_Pin                 
#define SCANKEY_GPIO_PORT            KEY_SCAN_GPIO_Port

/*******************************************************/

 /** 按键按下标置宏
	* 按键按下为高电平，设置 KEY_ON=1， KEY_OFF=0
	* 若按键按下为低电平，把宏设置成KEY_ON=0 ，KEY_OFF=1 即可
	*/
#define KEY_ON	1
#define KEY_OFF	0

#define KEY_PRESS	1
#define KEY_RELEASE	2

uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);

#endif /* __KEY_H */

