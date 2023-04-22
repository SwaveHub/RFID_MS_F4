#ifndef __SCREEN_OUTSTORAGE_H
#define	__SCREEN_OUTSTORAGE_H

/******************************** 头文件 *************************************/
#include "lvgl/lvgl.h"

/******************************** 宏定义 *************************************/


/***************************** 全局变量声明 **********************************/


/******************************* 函数声明 ************************************/
void screen_outstorage_create(void);
void screen_outstorage_record_add(uint8_t *tid);

#endif /* __SCREEN_OUTSTORAGE_H */
