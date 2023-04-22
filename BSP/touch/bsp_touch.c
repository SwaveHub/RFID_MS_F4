/**
  ******************************************************************************
  * @file    bsp_touch.c
  * @version V1.0
  * @date    2023-xx-xx
  * @brief   ����������
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
  * @brief  Touch ��ʼ������
  * @param  ��
  * @retval ��
  */	
void Touch_Init( void )
{
    GTP_Init_Panel();
}


/**
  * @brief  ���������״̬��
  * @retval ����״̬
	*   �÷���ֵΪ����ֵ֮һ��
  *     @arg TOUCH_PRESSED :��������
  *     @arg TOUCH_NOT_PRESSED :�޴���
  */
uint8_t Touch_TouchDetect(Touch_Sta_t *touch_dev)
{
    GTP_GetSta(touch_dev);
    return touch_dev->sta;
}

/**
  * @brief  ��ȡ�����������
  * @param  Touch_Sta_t *touch_dev ����ָ���Ż�ȡ���Ĵ���������,������״̬��Ϣ
  * @retval ��ȡ���
	*   �÷���ֵΪ����ֵ֮һ��
  *     @arg 1 :��ȡ�ɹ�
  *     @arg 0 :��ȡʧ��
  */
uint8_t Touch_Get_TouchedPoint(Touch_Sta_t *touch_dev)
{
    GTP_GetSta(touch_dev);
    return touch_dev->sta;    
}

/***************************end of file*****************************/


