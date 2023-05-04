  /**
  ******************************************************************************
  * @file    bsp_internal_flash.c
  * @author  STMicroelectronics
  * @version V1.0
  * @date    2023-xx-xx
  * @brief   internal flash �ײ�Ӧ�ú���bsp 
  ******************************************************************************
  * @attention �ڲ�����д������ݴ�С���ṩ���ⲿʹ�õĶ�д���������ݴ�С���������֣�32λ����С
  *                     
  * 
  * 
  * 
  *
  ******************************************************************************
  */
#include "./flash/bsp_internal_flash.h"
#include "Unix.h"
#include "usart.h"

static void InternalFlash_UpdateRecord(item_info_t* item_info, uint32_t dataLen);

static uint8_t InternalFlash_GetFlashSector(uint32_t addr);
static uint32_t InternalFlash__ReadWord(uint32_t addr);

/**
   * @brief  ��������ĵ�ַ���������ڵ�FlashSector
   * @param  addr����ַ
   * @retval ��ַ���ڵ�FlashSector
   */
static uint8_t InternalFlash_GetFlashSector(uint32_t addr)
{
	if(addr<ADDR_FLASH_SECTOR_1)return FLASH_SECTOR_0;
	else if(addr<ADDR_FLASH_SECTOR_2)return FLASH_SECTOR_1;
	else if(addr<ADDR_FLASH_SECTOR_3)return FLASH_SECTOR_2;
	else if(addr<ADDR_FLASH_SECTOR_4)return FLASH_SECTOR_3;
	else if(addr<ADDR_FLASH_SECTOR_5)return FLASH_SECTOR_4;
	else if(addr<ADDR_FLASH_SECTOR_6)return FLASH_SECTOR_5;
	else if(addr<ADDR_FLASH_SECTOR_7)return FLASH_SECTOR_6;
	else if(addr<ADDR_FLASH_SECTOR_8)return FLASH_SECTOR_7;
	else if(addr<ADDR_FLASH_SECTOR_9)return FLASH_SECTOR_8;
	else if(addr<ADDR_FLASH_SECTOR_10)return FLASH_SECTOR_9;
	else if(addr<ADDR_FLASH_SECTOR_11)return FLASH_SECTOR_10;   
	return FLASH_SECTOR_11;	
}

/**
 @brief �ڲ�Flash��ȡָ����ַ����(32λ����) 
 @param address -[in] ��ȡ�ĵ�ַ
 @return ��Ӧ����
*/
static uint32_t InternalFlash__ReadWord(uint32_t addr)
{
	return *(volatile uint32_t*)addr; 
}


/**
 @brief �ڲ�Flash��ȡ
 @param address -[in] ��ȡ�ĵ�ַ
 @param pData -[out] ָ����Ҫ����������
 @param dataLen -[in] ���ݳ���
 @return �����ɹ����ֽ���
*/
void Internal_ReadFlash(uint32_t addrStart, uint32_t *pData, uint32_t dataLen)
{
	uint32_t i;
	for(i = 0; i < dataLen; i++)
	{
		pData[i] = InternalFlash__ReadWord(addrStart);//��ȡ4���ֽ�.
		addrStart += 4;//ƫ��4���ֽ�.	
	}
}

/**
 @brief �ڲ�Flashд��
 @param address -[in] д��ĵ�ַ
 @param pData -[in] ָ����Ҫ����������
 @param dataLen -[in] ���ݳ���
 @return ��
*/
void Internal_WriteFlash(uint32_t addrStart, const uint32_t *pData, uint32_t dataLen)
{   
	FLASH_EraseInitTypeDef FlashEraseInit;
	HAL_StatusTypeDef FlashStatus=HAL_OK;
	uint32_t SectorError=0;
	uint32_t addrx=0;
	uint32_t endaddr=0;	
	if(addrStart<STM32_FLASH_BASE||addrStart%4)return;	//�Ƿ���ַ
    
	HAL_FLASH_Unlock();             //����	
	addrx=addrStart;				//д�����ʼ��ַ
	endaddr=addrStart+dataLen*4;	//д��Ľ�����ַ
    
	if(FlashStatus==HAL_OK)
	{
		 while(addrStart<endaddr)//д����
		 {
			if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,addrStart,*pData)!=HAL_OK)//д������
			{ 
				break;	//д���쳣
			}
			addrStart+=4;
			pData++;
		}  
	}
	HAL_FLASH_Lock();           //����       
}

/**
 @brief �ڲ�Flashҳ����
 @param pageAddress -[in] ��������ʼ��ַ
 @param nbPages -[in] ����ҳ��
 @return 0 - �ɹ���-1 - ʧ��
*/
int Internal_EraseSectors(uint32_t pageAddress, uint32_t nbSectors)
{
	FLASH_EraseInitTypeDef FlashEraseInit;
	uint32_t SectorError=0;    
    
	HAL_FLASH_Unlock();             //����   
    FlashEraseInit.TypeErase=FLASH_TYPEERASE_SECTORS;       //�������ͣ��������� 
    FlashEraseInit.Sector=InternalFlash_GetFlashSector(pageAddress);   //Ҫ����������
    FlashEraseInit.NbSectors=nbSectors;                             //��������������
    FlashEraseInit.VoltageRange=FLASH_VOLTAGE_RANGE_3;      //��ѹ��Χ��VCC=2.7~3.6V֮��!!
    if(HAL_FLASHEx_Erase(&FlashEraseInit,&SectorError)!=HAL_OK) 
    {
        HAL_FLASH_Lock();           //����
        return -1;//����������	
    }
    FLASH_WaitForLastOperation(FLASH_WAITETIME);                //�ȴ��ϴβ������    
    HAL_FLASH_Lock();           //����
    return 0;
}

/**
 @brief �ڲ�Flash���ҿ�д���ǩ��¼����ʼ��ַ
 @param addrStart -[in] ����������ʼ��ַ
 @param dataLen -[in] ���ݳ���
 @return ��д���ǩ��¼����ʼ��ַ
*/
uint32_t InternalFlash_FindStartAddr(uint32_t addrStart, uint32_t dataLen)
{
    uint32_t mark = 0;
    uint32_t cnt = 0;
    
    do {
        Internal_ReadFlash(addrStart + cnt * dataLen * 4, &mark, 1);
         cnt++;
    } while (mark != 0xFFFFFFFF);
    
    return addrStart + --cnt * dataLen * 4;
}

/**
 @brief �ڲ�Flash����ָ����ǩTID��¼����ʼ��ַ
 @param addrStart -[in] ����������ʼ��ַ
 @param dataLen -[in] ���ݳ���
 @return ָ����ǩTID��¼����ʼ��ַ
*/
uint32_t InternalFlash_FindTIDAddr(uint8_t *tid, uint32_t addrStart, uint32_t dataLen)
{
    uint32_t cnt = 0;
    item_info_t *item_info_new = (item_info_t *)malloc(sizeof(item_info_t));
    
    do {
        Internal_ReadFlash(FLASH_USER_START_ADDR + cnt * dataLen * 4, (uint32_t *)item_info_new, dataLen);
        if (memcmp(tid, item_info_new->TID, 12) == 0) {
            free(item_info_new);
            return FLASH_USER_START_ADDR + cnt * dataLen * 4;
        }
         cnt++;
    } while (item_info_new->mark != 0xFFFFFFFF);
    
    return 0;
}

/**
 @brief �ڲ�Flashд���ǩ��¼
 @param head_ref -[in] ָ����Ҫ��������������ͷ
 @param dataLen -[in] �������������ݳ���
 @return ��
*/
void InternalFlash_WriteRecord(item_info_t** head_ref, uint32_t dataLen)
{
    item_info_t* current = *head_ref;
    uint32_t cnt = 0;
    uint32_t addrStart = 0;
    
    while (current != NULL) {
        //���ݲ�ͬ��mark���ò�ͬ�ķ�ʽд��¼��flash
        if (current->mark == MARK_INSTORAGE) {
            addrStart = InternalFlash_FindStartAddr(FLASH_USER_START_ADDR, dataLen); //���ҿ�д���ǩ��¼����ʼ��ַ
            if (addrStart >= FLASH_USER_START_ADDR && addrStart <= FLASH_USER_END_ADDR - dataLen) { //��ַ�Ϸ�
                if (0 == InternalFlash_FindTIDAddr(current->TID, FLASH_USER_START_ADDR, dataLen))   //���µı�ǩ
                    Internal_WriteFlash(addrStart , (uint32_t *)current, dataLen);
            }
        } else if (current->mark == MARK_OUTSTORAGE) {
            InternalFlash_UpdateRecord(current, dataLen);
        }
        current = current->next;
        cnt++;
    }
}

/**
 @brief �ڲ�Flash�ϴ���ǩ��¼
 @param item_info_t *item_info
 @return ��
*/
void InternalFlash_UploadRecord(item_info_t *item_info)
{
        /* ͨ������3����cmd����λ�� */
        /* ���ݸ�ʽ��֡ͷ0xFC,0xFC��֡β\r\n, ���ݣ�"����","���ܺ�","���","TID","���ʱ��","����ʱ��",
            ����'0xFC''0xFC'cola,1,1,69952000400582D1160A0000,2023yy5mm1dd21:28,NULL,'\r''\n'
        */
    uint8_t cmd[100] = {0};
    uint8_t str[20] = {0};
    memset(cmd, 0, sizeof(cmd));
    cmd[0] = cmd[1] = 0xFC;
    
    strcat(cmd, item_info->name);
    strcat(cmd, ",");
    
    sprintf(str, "%d", item_info->shelf);
    strcat(cmd, str);
    strcat(cmd, ",");
    
    sprintf(str, "%d", item_info->layer);
    strcat(cmd, str);
    strcat(cmd, ",");
    
   for (int i = 0; i < sizeof(item_info->TID); i++) {
        sprintf(str, "%02X", item_info->TID[i]);
        strcat(cmd, str);
    }
    strcat(cmd, ",");
    
    if (item_info->instorage_time != 0xFFFFFFFF) {
        Unix_To_YMD_Time(&system_time, item_info->instorage_time);
        sprintf(str, "%dyy%dmm%ddd%d:%d", system_time.year, system_time.month, system_time.day, system_time.hour, system_time.minute);
    } else {
        sprintf(str, "NULL");
    }
    strcat(cmd, str);
    strcat(cmd, ",");
    
    if (item_info->outstorage_time != 0xFFFFFFFF) {
        Unix_To_YMD_Time(&system_time, item_info->outstorage_time);
        sprintf(str, "%dyy%dmm%ddd%d:%d", system_time.year, system_time.month, system_time.day, system_time.hour, system_time.minute);
    } else {
        sprintf(str, "NULL");
    }
    strcat(cmd, str);
    strcat(cmd, ",");
    strcat(cmd, "\r\n");
    
    HAL_UART_Transmit(&huart3, cmd, strlen(cmd),0xFFFF); 	
    printf("%s", cmd);
}

/**
 @brief �ڲ�Flash��ȡ��ǩ��¼
 @param dataLen -[in] �������������ݳ���
 @return ��
*/
void InternalFlash_ReadRecord(uint32_t dataLen)
{
    item_info_t *item_info_new = (item_info_t *)malloc(sizeof(item_info_t));
    
    uint8_t *p = (uint8_t *)item_info_new;
    uint32_t cnt = 0;
    
    do {
        Internal_ReadFlash(FLASH_USER_START_ADDR + cnt * dataLen * 4, (uint32_t *)item_info_new, dataLen);
        if(item_info_new->mark != 0xFFFFFFFF) {
           for (int i = 0; i < dataLen * 4; i++) {
                printf("%02X ", p[i]);
            }
            printf("\r\n");
            
            InternalFlash_UploadRecord(item_info_new);
        } else {
            free(item_info_new);
            return;
        }
         cnt++;
    } while (1);    
}

/**
 @brief �ڲ�Flash���ñ�ǩ��¼��
 @param addrStart -[in] ����������ʼ��ַ
 @param addrEnd -[in] �������������ַ
 @return ��
*/
void InternalFlash_ResetRecord(uint32_t addrStart, uint32_t addrEnd)
{
    uint32_t nbSectors = (addrEnd - addrStart) / STM32FLASH_USER_SECTORS_SIZE;
    uint32_t mark;
    uint32_t addr;
//    Internal_EraseSectors(addrStart, nbSectors);
    for (uint8_t i = 0; i < nbSectors; i++) {
        addr = addrStart + i * STM32FLASH_USER_SECTORS_SIZE;
        Internal_ReadFlash(addr, &mark, 1);    
        if (mark != 0xFFFFFFFF) Internal_EraseSectors(addr, 1);
        else return;
    }
}

/**
 @brief �ڲ�Flash���±�ǩ��¼
 @param head_ref -[in] ָ����Ҫ��������������ͷ
 @param dataLen -[in] ���ݳ���
 @return ��
*/
static void InternalFlash_UpdateRecord(item_info_t* item_info, uint32_t dataLen)
{
    item_info_t* item_info_new = malloc(sizeof(item_info_t));
    uint32_t addrStart = 0; 
    
   addrStart = InternalFlash_FindTIDAddr(item_info->TID, FLASH_USER_START_ADDR, dataLen);
    if (addrStart >= FLASH_USER_START_ADDR && addrStart <= FLASH_USER_END_ADDR - dataLen) { //��ַ�Ϸ�
        Internal_ReadFlash(addrStart, (uint32_t *)item_info_new, dataLen);
        item_info_new->mark = item_info->mark;
        item_info_new->outstorage_time = item_info->outstorage_time;
        Internal_WriteFlash(addrStart, (uint32_t *)item_info_new, dataLen);
    }
    free(item_info_new);
}

/*********************************************END OF FILE**********************/

