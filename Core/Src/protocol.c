/**
  ******************************************************************************
  * @file    protocol.c
  * @author  SwaveYu
  * @version V1.0
  * @date    2021-09-05
  * @brief   ͨ��Э�鴦��
  *	    �޸ļ�¼ :
  *		�汾��  ����              ����             ˵��
  *         V1.0    2021-09-05  SwaveYu     ��ʽ����
  *         V1.1    2021-09-14  SwaveYu     �����˴��ڵ���Э��  
  ******************************************************************************
  * @attention
  *
  *
  *
  *
  *
  ******************************************************************************
  */

/******************************** ͷ�ļ� *************************************/
#include "bsp.h"
#include "stdlib.h"
#include "string.h"

/****************************** ˽�к궨�� ***********************************/

/******************************* ȫ�ֱ��� ************************************/

/***************************** ˽��ȫ�ֱ��� **********************************/


/******************************* ˽�к��� ************************************/


/**
 * @brief  ͨ��Э�����������
 * @param  PROTOCOL_MODETypeDef mode ����ģʽ, uint8_t *buf ���������, uint16_t len ��������ݳ���
 * @retval   ��
 */
int Protocol_Decode(PROTOCOL_MODETypeDef mode, uint8_t *buf, uint16_t len)
{
    if(mode == PROTOCOL_MODE_DEFAULT) { 
        /* �������Դ��ڵ�ʱ������ָ�ָ��ĸ�ʽΪSET:N������ʱ���룬����SET:2023-3-8-15:36:27 */
        if (memcmp(buf, FORMAT_SET, FORMAT_SET_LEN) == 0) {
            uint32_t date_time[6];
            uint32_t index = FORMAT_SET_LEN;
            uint32_t sn = 0;
            for (uint32_t i = FORMAT_SET_LEN; i < len; i++) {
                if (buf[i] == ',' || buf[i] == ';' || buf[i] == '-' || buf[i] == ':' || buf[i] == ' ') {
                    //buf[i] = '\0';
                    date_time[sn++] = atoi(buf + index);
                    index = i + 1;
                }
                else if (i == len - 1) {
                    date_time[sn] = atoi(buf + index);
                }
            }
            RTC_Set(date_time[0], date_time[1], date_time[2], date_time[3], date_time[4], date_time[5]);
            return 0;
        }
        printf("����ģʽ%dƥ��ʧ��\n", mode);
        return -1;
    }
    else {
        return 0;
    }
}

 /*********************************END OF FILE*********************************/
