#ifndef __PROTOCOL_H
#define	__PROTOCOL_H

/******************************** 头文件 *************************************/
#include "stdint.h"

/******************************** 宏定义 *************************************/
#define FORMAT_SET  "SET:"
#define FORMAT_SET_LEN  (strlen(FORMAT_SET))

#define PARM_NUM  5

/***************************** 全局变量声明 **********************************/

/* 定义协议处理模式 */
typedef enum {
    PROTOCOL_MODE_DEFAULT,
}PROTOCOL_MODETypeDef ;

/******************************* 函数声明 ************************************/
int Protocol_Decode(PROTOCOL_MODETypeDef mode, uint8_t *buf, uint16_t len);
    
#endif /* __PROTOCOL_H */
