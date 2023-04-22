#ifndef __PROTOCOL_H
#define	__PROTOCOL_H

/******************************** ͷ�ļ� *************************************/
#include "stdint.h"

/******************************** �궨�� *************************************/
#define FORMAT_SET  "SET:"
#define FORMAT_SET_LEN  (strlen(FORMAT_SET))

#define PARM_NUM  5

/***************************** ȫ�ֱ������� **********************************/

/* ����Э�鴦��ģʽ */
typedef enum {
    PROTOCOL_MODE_DEFAULT,
}PROTOCOL_MODETypeDef ;

/******************************* �������� ************************************/
int Protocol_Decode(PROTOCOL_MODETypeDef mode, uint8_t *buf, uint16_t len);
    
#endif /* __PROTOCOL_H */
