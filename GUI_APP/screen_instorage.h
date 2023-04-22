#ifndef __SCREEN_IN_STORAGE_H
#define	__SCREEN_IN_STORAGE_H

/******************************** 头文件 *************************************/
#include "lvgl/lvgl.h"

/******************************** 宏定义 *************************************/


/***************************** 全局变量声明 **********************************/


/******************************* 函数声明 ************************************/
void screen_instorage_create(void);
void screen_instorage_record_add(uint8_t *tid);

#endif /* __SCREEN_IN_STORAGE_H */
