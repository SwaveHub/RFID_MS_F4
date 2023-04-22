/**
  ******************************************************************************
  * @file    demo.c
  * @author  SwaveYu
  * @version V1.0
  * @date    2023-03-09
  * @brief   lvgl demo
  *	    �޸ļ�¼ :
  *		�汾��  ����              ����             ˵��
  *         V1.0    2023-03-09  SwaveYu     ��ʽ����
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
#include "demo.h"

/****************************** ˽�к궨�� ***********************************/

/******************************* ȫ�ֱ��� ************************************/

/***************************** ˽��ȫ�ֱ��� **********************************/
static lv_obj_t *label_time;

/******************************* ˽�к��� ************************************/

/**
 * @brief   �����û���������
 * @param  ��
 * @retval   ��
 */
void demo_create(void)
{
    printf("demo_start\r\n");
    screen_home_create();    
}

/**
 * @brief   ˢ��ʱ��
 * @param  ��
 * @retval   ��
 */
void time_update(void)
{
    static uint16_t last_minute;
    
    RTC_Get(&system_time.year, &system_time.month, &system_time.day, &system_time.hour, &system_time.minute, &system_time.second);
    if (system_time.minute != last_minute) {
        last_minute = system_time.minute;
    } else return;
    
    lv_obj_t* scr = lv_scr_act();//��ȡ��ǰ��Ծ����Ļ����

    //���� label_time ��ǩ
    static lv_style_t label_text_style;
    lv_style_copy(&label_text_style,&lv_style_plain);
    label_text_style.text.font = &lv_font_roboto_16;
    label_time = lv_label_create(scr,NULL);//������ǩ
    lv_label_set_long_mode(label_time,LV_LABEL_LONG_BREAK);//���ó��ı�ģʽ
    lv_obj_set_width(label_time,100);
    lv_label_set_align(label_time,LV_LABEL_ALIGN_CENTER);//�ı����ж���
    lv_label_set_style(label_time, LV_LABEL_STYLE_MAIN, &label_text_style);
    uint8_t buf[30];
    memset(buf, 0, sizeof(buf));
    lv_label_set_body_draw(label_time, true);
    sprintf(buf, "%u:%02u\n%u/%u/%u", system_time.hour, system_time.minute, 
                                                        system_time.year, system_time.month, system_time.day);
    lv_label_set_text(label_time, buf);//�����ı�
    lv_obj_set_pos(label_time, 375, 5);    
}

 /*********************************END OF FILE*********************************/
