#ifndef __RFID_H
#define	__RFID_H

#include "stm32f4xx_hal.h"
#include ".\rfid\bsp_rf200_rfid.h"

#define MAX_TAG_FILTER_NUM  50

#define SCAN_DISABLE  0
#define SCAN_ENABLE  1

enum {
    SCAN_MODE_INSTORAGE_SINGLE,
    SCAN_MODE_INSTORAGE_MULTI,
    SCAN_MODE_OUTSTORAGE_SINGLE,
    SCAN_MODE_OUTSTORAGE_MULTI,    
};


extern uint8_t tag_cnt;    // 标签计数
extern uint8_t rfid_scan_enable;  // 扫描使能
extern uint8_t rfid_scan_mode;  // 扫描模式

typedef struct {// 筛选出的标签
    uint8_t len;
    TAG_STA_t tag[MAX_TAG_FILTER_NUM];
}tag_filter_t;

extern tag_filter_t tag_filter;

void RFID_GetSingleTID(void);
void RFID_FilterTag(uint8_t *cnt);

#endif /* __RFID_H */

