#ifndef __INTERNAL_FLASH_H
#define __INTERNAL_FLASH_H

#include "stm32f4xx_hal.h"
#include "linked_list.h"

//FLASH 扇区的起始地址
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) 	//扇区0起始地址, 16 KB
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) 	//扇区1起始地址, 16 KB
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) 	//扇区2起始地址, 16 KB
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) 	//扇区3起始地址, 16 KB
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) 	//扇区4起始地址, 64 KB
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) 	//扇区5起始地址, 128 KB
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) 	//扇区6起始地址, 128 KB
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) 	//扇区7起始地址, 128 KB
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) 	//扇区8起始地址, 128 KB
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) 	//扇区9起始地址, 128 KB
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) 	//扇区10起始地址,128 KB
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) 	//扇区11起始地址,128 KB

/* STM32内部FLASH的起始地址 */
#define STM32_FLASH_BASE            ADDR_FLASH_SECTOR_0
/* 要擦除内部FLASH的起始地址 */
#define FLASH_USER_START_ADDR   ADDR_FLASH_SECTOR_6
/* 要擦除内部FLASH的结束地址 */
#define FLASH_USER_END_ADDR     ADDR_FLASH_SECTOR_11

#define STM32FLASH_USER_SECTORS_SIZE 0x20000U//用户使用的扇区FLASH大小
#define FLASH_WAITETIME  50000          //FLASH等待超时时间

void Internal_WriteFlash(uint32_t addrStart, const uint32_t *pData, uint32_t dataLen);
void Internal_ReadFlash(uint32_t addrStart, uint32_t *pData, uint32_t dataLen);
int Internal_EraseSectors(uint32_t pageAddress, uint32_t nbSectors);
    
void InternalFlash_WriteRecord(item_info_t** head_ref, uint32_t dataLen);
void InternalFlash_ReadRecord(uint32_t dataLen);
void InternalFlash_ResetRecord(uint32_t addrStart, uint32_t addrEnd);

uint32_t InternalFlash_FindTIDAddr(uint8_t *tid, uint32_t addrStart, uint32_t dataLen);
#endif /* __INTERNAL_FLASH_H */

