#ifndef __INTERNAL_FLASH_H
#define __INTERNAL_FLASH_H

#include "stm32f4xx_hal.h"
#include "linked_list.h"

//FLASH ��������ʼ��ַ
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) 	//����0��ʼ��ַ, 16 KB
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) 	//����1��ʼ��ַ, 16 KB
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) 	//����2��ʼ��ַ, 16 KB
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) 	//����3��ʼ��ַ, 16 KB
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) 	//����4��ʼ��ַ, 64 KB
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) 	//����5��ʼ��ַ, 128 KB
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) 	//����6��ʼ��ַ, 128 KB
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) 	//����7��ʼ��ַ, 128 KB
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) 	//����8��ʼ��ַ, 128 KB
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) 	//����9��ʼ��ַ, 128 KB
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) 	//����10��ʼ��ַ,128 KB
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) 	//����11��ʼ��ַ,128 KB

/* STM32�ڲ�FLASH����ʼ��ַ */
#define STM32_FLASH_BASE            ADDR_FLASH_SECTOR_0
/* Ҫ�����ڲ�FLASH����ʼ��ַ */
#define FLASH_USER_START_ADDR   ADDR_FLASH_SECTOR_6
/* Ҫ�����ڲ�FLASH�Ľ�����ַ */
#define FLASH_USER_END_ADDR     ADDR_FLASH_SECTOR_11

#define STM32FLASH_USER_SECTORS_SIZE 0x20000U//�û�ʹ�õ�����FLASH��С
#define FLASH_WAITETIME  50000          //FLASH�ȴ���ʱʱ��

void Internal_WriteFlash(uint32_t addrStart, const uint32_t *pData, uint32_t dataLen);
void Internal_ReadFlash(uint32_t addrStart, uint32_t *pData, uint32_t dataLen);
int Internal_EraseSectors(uint32_t pageAddress, uint32_t nbSectors);
    
void InternalFlash_WriteRecord(item_info_t** head_ref, uint32_t dataLen);
void InternalFlash_ReadRecord(uint32_t dataLen);
void InternalFlash_ResetRecord(uint32_t addrStart, uint32_t addrEnd);

uint32_t InternalFlash_FindTIDAddr(uint8_t *tid, uint32_t addrStart, uint32_t dataLen);
#endif /* __INTERNAL_FLASH_H */

