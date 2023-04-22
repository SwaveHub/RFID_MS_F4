/**
  ******************************************************************************
  * @file    bsp_key.c
  * @author  STMicroelectronics
  * @version V1.0
  * @date    2023-xx-xx
  * @brief   ����Ӧ��bsp��ɨ��ģʽ��
  ******************************************************************************
  * @attention
  *
  * 
  * 
  * 
  *
  ******************************************************************************
  */ 

  

#include ".\key\bsp_key.h" 

/**

  * @brief   ����Ƿ��а�������

  * @param   ����Ķ˿ںͶ˿�λ

  *		@arg GPIOx: x�����ǣ�A...G�� 

  *		@arg GPIO_PIN ������GPIO_PIN_x��x������1...16��

  * @retval  ������״̬

  *		@arg KEY_ON:��������

  *		@arg KEY_OFF:����û����

  */

uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{			
	/*����Ƿ��а������� */
	if(HAL_GPIO_ReadPin(GPIOx,GPIO_Pin) == KEY_ON )  
	{	 
		/*�ȴ������ͷ� */
		while(HAL_GPIO_ReadPin(GPIOx,GPIO_Pin) == KEY_ON);   
		return 	KEY_ON;	 

	}
	else
		return KEY_OFF;

}

/*********************************************END OF FILE**********************/

