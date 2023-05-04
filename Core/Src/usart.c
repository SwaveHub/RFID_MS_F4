/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
uint8_t aRxBuffer;			//�����жϻ���

uint8_t Uart1_Rx_Buf[UART1_REC_LEN];   //��������
uint8_t Uart1_Rx_Cnt = 0;		//���ջ������
uint8_t Uart1_Rx_Sta = 0; //��������״̬ 0��δ���յ�����  1���������յ�һ������

uint8_t Uart2_Rx_Buf[UART2_REC_LEN];   //��������
uint8_t Uart2_Rx_Cnt = 0;		//���ջ������

uint8_t Uart3_Rx_Buf[UART3_REC_LEN];   //��������
uint8_t Uart3_Rx_Cnt = 0;		//���ջ������
uint8_t Uart3_Rx_Sta = 0; //��������״̬ 0��δ���յ�����  1���������յ�һ������
/* USER CODE END 0 */

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */
    HAL_UART_Receive_IT(&huart1, (uint8_t *)&aRxBuffer, 1);
  /* USER CODE END USART1_Init 2 */

}
/* USART2 init function */

void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */
    HAL_UART_Receive_IT(&huart2, (uint8_t *)&aRxBuffer, 1);
  /* USER CODE END USART2_Init 2 */

}
/* USART3 init function */

void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */
  HAL_UART_Receive_IT(&huart3, (uint8_t *)&aRxBuffer, 1);
  /* USER CODE END USART3_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspInit 0 */

  /* USER CODE END USART2_MspInit 0 */
    /* USART2 clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART2 interrupt Init */
    HAL_NVIC_SetPriority(USART2_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspInit 1 */

  /* USER CODE END USART2_MspInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspInit 0 */

  /* USER CODE END USART3_MspInit 0 */
    /* USART3 clock enable */
    __HAL_RCC_USART3_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**USART3 GPIO Configuration
    PB10     ------> USART3_TX
    PB11     ------> USART3_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* USART3 interrupt Init */
    HAL_NVIC_SetPriority(USART3_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspInit 1 */

  /* USER CODE END USART3_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspDeInit 0 */

  /* USER CODE END USART2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();

    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2|GPIO_PIN_3);

    /* USART2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspDeInit 1 */

  /* USER CODE END USART2_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspDeInit 0 */

  /* USER CODE END USART3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART3_CLK_DISABLE();

    /**USART3 GPIO Configuration
    PB10     ------> USART3_TX
    PB11     ------> USART3_RX
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10|GPIO_PIN_11);

    /* USART3 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspDeInit 1 */

  /* USER CODE END USART3_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
/**
  * ��������: �ض���c�⺯��printf��DEBUG_USARTx
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
PUTCHAR_PROTOTYPE
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1 , 0xffff);
    return ch;
}

/**
  * ��������: �ض���c�⺯��getchar,scanf��DEBUG_USARTx
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
GETCHAR_PROTOTYPE
{
  uint8_t ch = 0;
  HAL_UART_Receive(&huart1, &ch, 1, 0xffff);
  return ch;
}

/**
  * ��������: ���ڵ��жϻص�����
  * �������: UART_HandleTypeDef *huart
  * �� �� ֵ: ��
  * ˵    ������
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(huart);
  /* NOTE: This function Should not be modified, when the callback is needed,
           the HAL_UART_TxCpltCallback could be implemented in the user file
   */
    /* �Դ���1���жϷ��������� */
    if (huart->Instance == USART1) { 
        if(Uart1_Rx_Cnt >= 255)  //����ж�
        {
            Uart1_Rx_Cnt = 0;
            memset(Uart1_Rx_Buf,0x00,sizeof(Uart1_Rx_Buf));
            HAL_UART_Transmit(&huart1, (uint8_t *)"�������", 10,0xFFFF); 	
        }
        else
        {
            /* ��ǰ����δ������ǰ�������µ����� */
            if (!Uart1_Rx_Sta) {  
                Uart1_Rx_Buf[Uart1_Rx_Cnt++] = aRxBuffer;   //��������ת��
                
                if((Uart1_Rx_Buf[Uart1_Rx_Cnt-2] == 0x0D)&&(Uart1_Rx_Buf[Uart1_Rx_Cnt-1] == 0x0A)) //�жϽ���λ
                {
                    Uart1_Rx_Buf[Uart1_Rx_Cnt-2] = '\0';    //�����жϽ���λ��\r\n������յ�����
                    Uart1_Rx_Cnt = 0;
                    Uart1_Rx_Sta = 1;
                }                       
            }
        }
        HAL_UART_Receive_IT(&huart1, (uint8_t *)&aRxBuffer, 1);   //�ٿ��������ж�
    }	
	
    /* �Դ���2���жϷ��������� */
    if (huart->Instance == USART2) {
        if((Uart2_Rx_Cnt == 0)&&(aRxBuffer == 0xBB))// frame start must 0xbb
		{
			Uart2_Rx_Buf[0] = aRxBuffer;
			Uart2_Rx_Cnt++; 
		}
        else if((Uart2_Rx_Cnt>0)&&(Uart2_Rx_Cnt < UART2_REC_LEN))
        {
            Uart2_Rx_Buf[Uart2_Rx_Cnt] = aRxBuffer; 
            Uart2_Rx_Cnt++; 
        }
        HAL_UART_Receive_IT(&huart2, (uint8_t *)&aRxBuffer, 1);   //�ٿ��������ж�
    }

    /* �Դ���3���жϷ��������� */
    if (huart->Instance == USART3) { 
        if(Uart3_Rx_Cnt >= 255)  //����ж�
        {
            Uart3_Rx_Cnt = 0;
            memset(Uart3_Rx_Buf,0x00,sizeof(Uart3_Rx_Buf));
            HAL_UART_Transmit(&huart3, (uint8_t *)"�������", 10,0xFFFF); 	
        }
        else
        {
            /* ��ǰ����δ������ǰ�������µ����� */
            if (!Uart3_Rx_Sta) {  
                Uart3_Rx_Buf[Uart3_Rx_Cnt++] = aRxBuffer;   //��������ת��
                
                if((Uart3_Rx_Buf[Uart3_Rx_Cnt-2] == 0x0D)&&(Uart3_Rx_Buf[Uart3_Rx_Cnt-1] == 0x0A)) //�жϽ���λ
                {
                    Uart3_Rx_Buf[Uart3_Rx_Cnt-2] = '\0';    //�����жϽ���λ��\r\n������յ�����
                    Uart3_Rx_Cnt = 0;
                    Uart3_Rx_Sta = 1;
                }                       
            }
        }
        HAL_UART_Receive_IT(&huart3, (uint8_t *)&aRxBuffer, 1);   //�ٿ��������ж�
    }    
}


/**
  * ��������: ר������RFID��д�����д���ͨ��ʱʹ�õĺ���
  * �������: unsigned char *dat, unsigned char len
  * �� �� ֵ: ��
  * ˵    ������
  */
void PutString(unsigned char *dat, unsigned char len)
{
	unsigned int    i;
	for(i = 0; i < len; i++) {
		  HAL_UART_Transmit(&huart2, dat++, 1 , 0xffff);
	}
}

/**********************************************************************************************************
USART_RXD_Data_Process�����ڽ��պ���
���ܣ�����һ֡����
**********************************************************************************************************/
void USART_RXD_Data_Process(void)
{
	unsigned int    length  ;    //  data length                     
	if (Uart2_Rx_Cnt>5) 
	{
		length = Uart2_Rx_Buf[4];
		if(Uart2_Rx_Cnt>=(length+7)&&Uart2_Rx_Buf[length+6] == 0x7e)//revice frame end
		{
			Usart_buff_copy(Uart2_Rx_Buf,length+7);
			Uart2_Rx_Cnt = 0;
		}
	} 
}
/* USER CODE END 1 */
