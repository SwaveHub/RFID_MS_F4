/**--------------File Info------------------------------------------------------------------------------
File name:			  rf200_drv.h
** Last modified Date:  2018-05-30
** Last Version:		V1.02
** Descriptions:		RF200 FRAME HANDLE
**------------------------------------------------------------------------------------------------------
** Created date:		2018-05-30
** Version:			  	V1.02
** Descriptions:		RF200 FRAME HANDLE
**
********************************************************************************************************/
#ifndef __RF200_DRV_H
#define __RF200_DRV_H

#ifdef  _RF200_DRV_MANL
#define _EXT_RF200_DRV
#else 
#define _EXT_RF200_DRV    extern 
#endif

#define MAX_TAG_NUM			200

#define SendCmdFrame(ArrayX) PutString(ArrayX,ArrayX[4]+7)

#include "stdint.h"

//帧数据结构体
typedef struct {
	uint8_t byteLen;
	uint8_t dat[127];
} frame_data_t;

//帧类型枚举体
enum frame_type {
	FRAME_CMD = 	0x00,
	FRAME_RES = 	0x01,
	FRAME_INFO = 	0x02,
	FRAME_ERROR = 0xFF
};

enum cmd_code {
	CMD_GET_MODULE_INFO	= 0x03,
	CMD_SINGLE_ID 		= 	0x22,
	CMD_MULTI_ID 		= 		0x27,
	CMD_STOP_MULTI 		= 	0x28,
	CMD_READ_DATA 		= 	0x39,
	CMD_WRITE_DATA 		= 	0x49,
	CMD_LOCK_UNLOCK 	= 	0x82,
	CMD_KILL 			= 			0x65,
	CMD_SET_REGION      = 0x07,
	CMD_INSERT_FHSS_CHANNEL = 0xA9,
	CMD_GET_RF_CHANNEL  = 0xAA,
	CMD_SET_RF_CHANNEL  = 0xAB,
	CMD_SET_CHN2_CHANNEL= 0xAF,
	CMD_SET_US_CHANNEL 	= 0xAC,		  // For RFCONN Conference
	CMD_OPEN_PA		 	= 		0xAE,		  // For RFCONN Conference
	CMD_SET_FHSS        = 0xAD,
	CMD_SET_POWER 	    = 0xB6,
	CMD_GET_POWER 	    = 0xB7,
	CMD_GET_SELECT_PARA	= 0x0B,
	CMD_SET_SELECT_PARA = 0x0C,
	CMD_GET_QUERY_PARA 	= 0x0D,
	CMD_SET_QUERY_PARA 	= 0x0E,
	CMD_SET_CW 			= 		0xB0,
	CMD_SET_BLF 		= 		0xBF,
	CMD_FAIL 			= 			0xFF,
	CMD_SUCCESS 		= 		0x00,
	CMD_SET_SFR 		= 		0xFE,
	CMD_READ_SFR 		= 		0xFD,
	CMD_INIT_SFR 		= 		0xEC,

	CMD_READ_MEM		= 		0xFB,
	CMD_SET_INV_MODE    = 0x12,

	CMD_SCAN_JAMMER     = 0xF2,
	CMD_SCAN_RSSI       = 0xF3,
	CMD_AUTO_ADJUST_CH  = 0xF4,

	CMD_SET_MODEM_PARA  = 0xF0,
	CMD_READ_MODEM_PARA = 0xF1,
	CMD_SET_ENV_MODE    = 0xF5,
	CMD_TEST_RESET      = 0x55,

	CMD_POWERDOWN_MODE  = 0x17,
	CMD_SET_SLEEP_TIME  = 0x1D,
	CMD_RESTART         = 0x19,
};

enum fail_code {
//	FAIL_READ_MULTI_TAG = 0x0B,
	FAIL_INVALID_PARA = 0x0E,
	FAIL_INVENTORY_TAG_TIMEOUT = 0x15,
	FAIL_INVALID_CMD = 0x17,

	FAIL_FHSS_FAIL = 0x20,

	FAIL_ACCESS_PWD_ERROR = 0x16,

	FAIL_READ_MEMORY_NO_TAG = 0x09,
	FAIL_READ_ERROR_CODE_BASE = 0xA0,

	FAIL_WRITE_MEMORY_NO_TAG = 0x10,
	FAIL_WRITE_ERROR_CODE_BASE = 0xB0,

	FAIL_LOCK_NO_TAG = 0x13,
	FAIL_LOCK_ERROR_CODE_BASE = 0xC0,

	FAIL_KILL_NO_TAG = 0x12,
	FAIL_KILL_ERROR_CODE_BASE = 0xD0,
    
  FAIL_WATCHDOG_OVERFLOW = 0x05
};
enum Membank_code {
	Membank_USER =3,
	Membank_EPC =1,
	Membank_TID =2,
	Membank_RFU =0
};

//芯片状态
typedef struct {
	uint8_t MultiInventFlag;//0:stop;1:run
	uint8_t InventATagFlag;//0:no tag ;1: got tag
	uint8_t ReadDataFlag;//0:fail;1:success
	uint8_t WriteDataFlag;//0:fail;1:success
	uint8_t SetRegionFlag;//0:fail;1:success
	uint8_t SetPowerFlag;//0:fail;1:success
	uint8_t SetSelectModeFlag;//0:fail;1:success
	uint8_t SetSelectParaFlag;//0:fail;1:success
	uint32_t TotalTags;//tag counter;
} RF200_STA_t;

//标签状态
typedef struct {
	uint8_t TID[12];    
	uint8_t EPC[12];
	uint8_t RSSI;
	uint8_t flag;//0:empty; 1:full; 
} TAG_STA_t;

typedef struct {
	uint8_t AccessPW[4]; //ACCESS PASSWORD
	uint8_t MemBank;
	uint8_t Offset;
	uint8_t len;
	uint8_t *dat;
} TAG_WR_PARA_t;

//标签数据
typedef struct {
	uint8_t EPC[12]; 
	uint8_t len;
	uint8_t dat[64];
} TAG_READ_DATA_t;

_EXT_RF200_DRV uint8_t SingleInventCMD[];
_EXT_RF200_DRV uint8_t MultiInventCMD[];
_EXT_RF200_DRV uint8_t StopMultiInventCMD[];
_EXT_RF200_DRV uint8_t SetRegionUSCMD[];
_EXT_RF200_DRV uint8_t SetRegionCH2CMD[];
_EXT_RF200_DRV uint8_t SetSelectModeCMD[];
_EXT_RF200_DRV uint8_t SetSelectParaCMD[];
_EXT_RF200_DRV uint8_t ReadDataCMD[];
_EXT_RF200_DRV uint8_t WriteDataCMD[];
_EXT_RF200_DRV int Exist_Flag;

_EXT_RF200_DRV frame_data_t RF200FRAME;
_EXT_RF200_DRV RF200_STA_t RF200Status;
_EXT_RF200_DRV TAG_STA_t TAG[MAX_TAG_NUM];
_EXT_RF200_DRV unsigned int TAG_Index;
_EXT_RF200_DRV TAG_WR_PARA_t TAG_WR_PARA;
_EXT_RF200_DRV TAG_READ_DATA_t TAG_READ_DATA;

_EXT_RF200_DRV void Usart_buff_copy(uint8_t *usart_buff,uint8_t data_len);
_EXT_RF200_DRV uint8_t RF200_FRAME_RX_HANDLE(void);

_EXT_RF200_DRV void TagWriteProcess(uint8_t *t_EPC,TAG_WR_PARA_t t_TAG_WR_PARA);
_EXT_RF200_DRV void TagReadProcess(uint8_t *t_EPC,TAG_WR_PARA_t t_TAG_WR_PARA);
_EXT_RF200_DRV uint8_t EPC_Compare(uint8_t *t_EPC1,uint8_t *t_EPC2);

#endif
