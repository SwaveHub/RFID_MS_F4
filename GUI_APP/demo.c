/**
  ******************************************************************************
  * @file    demo.c
  * @author  SwaveYu
  * @version V1.0
  * @date    2023-03-09
  * @brief   lvgl demo
  *	    修改记录 :
  *		版本号  日期              作者             说明
  *         V1.0    2023-03-09  SwaveYu     正式发布
  ******************************************************************************
  * @attention
  *
  *
  *
  *
  *
  ******************************************************************************
  */

/******************************** 头文件 *************************************/
#include "demo.h"

/****************************** 私有宏定义 ***********************************/

/******************************* 全局变量 ************************************/

/***************************** 私有全局变量 **********************************/
static lv_obj_t *label_time;

/******************************* 私有函数 ************************************/

/**
 * @brief   创建用户交互界面
 * @param  无
 * @retval   无
 */
void demo_create(void)
{
    printf("demo_start\r\n");
    screen_home_create();    
}

/**
 * @brief   刷新时间
 * @param  无
 * @retval   无
 */
void time_update(void)
{
    static uint16_t last_minute;
    
    RTC_Get(&system_time.year, &system_time.month, &system_time.day, &system_time.hour, &system_time.minute, &system_time.second);
    if (system_time.minute != last_minute) {
        last_minute = system_time.minute;
    } else return;
    
    lv_obj_t* scr = lv_scr_act();//获取当前活跃的屏幕对象

    //创建 label_time 标签
    static lv_style_t label_text_style;
    lv_style_copy(&label_text_style,&lv_style_plain);
    label_text_style.text.font = &lv_font_roboto_16;
    label_time = lv_label_create(scr,NULL);//创建标签
    lv_label_set_long_mode(label_time,LV_LABEL_LONG_BREAK);//设置长文本模式
    lv_obj_set_width(label_time,100);
    lv_label_set_align(label_time,LV_LABEL_ALIGN_CENTER);//文本居中对齐
    lv_label_set_style(label_time, LV_LABEL_STYLE_MAIN, &label_text_style);
    uint8_t buf[30];
    memset(buf, 0, sizeof(buf));
    lv_label_set_body_draw(label_time, true);
    sprintf(buf, "%u:%02u\n%u/%u/%u", system_time.hour, system_time.minute, 
                                                        system_time.year, system_time.month, system_time.day);
    lv_label_set_text(label_time, buf);//设置文本
    lv_obj_set_pos(label_time, 375, 5);    
}

 /*********************************END OF FILE*********************************/
