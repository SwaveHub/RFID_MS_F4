#ifndef __DEMO_H
#define	__DEMO_H

/******************************** 头文件 *************************************/
#include "./lvgl_port/lv_port_disp.h"
#include "./lvgl_port/lv_port_indev.h"
#include "./lvgl/lvgl.h"

#include "screen_home.h"
#include "screen_instorage.h"
#include "screen_outstorage.h"

#include "bsp.h"
/******************************** 宏定义 *************************************/


/***************************** 全局变量声明 **********************************/
enum {
    LVGL_TIME_REFRESH = 1,
    LVGL_INSTORAGE_SINGLE_RECORD_ADD,
    LVGL_INSTORAGE_MULTI_RECORD_ADD,
    LVGL_OUTSTORAGE_SINGLE_RECORD_ADD,
    LVGL_OUTSTORAGE_MULTI_RECORD_ADD,
};

/******************************* 函数声明 ************************************/
void demo_create(void);
void time_update(void);

#endif /* __DEMO_H */
