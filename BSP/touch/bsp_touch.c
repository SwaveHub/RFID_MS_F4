/**
  ******************************************************************************
  * @file    bsp_touch.c
  * @version V1.0
  * @date    2023-xx-xx
  * @brief   触摸屏驱动
  ******************************************************************************
  * @attention
  *
  * 
  * 
  * 
  *
  ******************************************************************************
  */ 

#include "bsp_touch.h"
#include "bsp.h"

Touch_Sta_t Touch_Dev = {
    .sta = 0,  
    .x = 0,
    .y = 0,
};

/**
  * @brief  Touch 初始化函数
  * @param  无
  * @retval 无
  */	
void Touch_Init( void )
{
    GTP_Init_Panel();
}


/**
  * @brief  触摸屏检测状态机
  * @retval 触摸状态
	*   该返回值为以下值之一：
  *     @arg TOUCH_PRESSED :触摸按下
  *     @arg TOUCH_NOT_PRESSED :无触摸
  */
uint8_t Touch_TouchDetect(Touch_Sta_t *touch_dev)
{
    GTP_GetSta(touch_dev);
    return touch_dev->sta;
}

/**
  * @brief  获取触摸点的坐标
  * @param  Touch_Sta_t *touch_dev ：该指针存放获取到的触摸点坐标,及触摸状态信息
  * @retval 获取情况
	*   该返回值为以下值之一：
  *     @arg 1 :获取成功
  *     @arg 0 :获取失败
  */
uint8_t Touch_Get_TouchedPoint(Touch_Sta_t *touch_dev)
{
    GTP_GetSta(touch_dev);
    return touch_dev->sta;    
}

/***************************end of file*****************************/


