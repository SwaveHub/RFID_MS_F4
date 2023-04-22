#ifndef __DEMO_H
#define	__DEMO_H

/******************************** ͷ�ļ� *************************************/
#include "./lvgl_port/lv_port_disp.h"
#include "./lvgl_port/lv_port_indev.h"
#include "./lvgl/lvgl.h"

#include "screen_home.h"
#include "screen_instorage.h"
#include "screen_outstorage.h"

#include "bsp.h"
/******************************** �궨�� *************************************/


/***************************** ȫ�ֱ������� **********************************/
enum {
    LVGL_TIME_REFRESH = 1,
    LVGL_INSTORAGE_SINGLE_RECORD_ADD,
    LVGL_INSTORAGE_MULTI_RECORD_ADD,
    LVGL_OUTSTORAGE_SINGLE_RECORD_ADD,
    LVGL_OUTSTORAGE_MULTI_RECORD_ADD,
};

/******************************* �������� ************************************/
void demo_create(void);
void time_update(void);

#endif /* __DEMO_H */
