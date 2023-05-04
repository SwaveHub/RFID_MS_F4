  /**
  ******************************************************************************
  * @file    bsp_internal_flash.c
  * @author  STMicroelectronics
  * @version V1.0
  * @date    2023-xx-xx
  * @brief   internal flash 底层应用函数bsp 
  ******************************************************************************
  * @attention 内部读出写入的数据大小及提供给外部使用的读写函数的数据大小参数均按字（32位）大小
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
   * @brief  根据输入的地址给出它所在的FlashSector
   * @param  addr：地址
   * @retval 地址所在的FlashSector
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
 @brief 内部Flash读取指定地址的字(32位数据) 
 @param address -[in] 读取的地址
 @return 对应数据
*/
static uint32_t InternalFlash__ReadWord(uint32_t addr)
{
	return *(volatile uint32_t*)addr; 
}


/**
 @brief 内部Flash读取
 @param address -[in] 读取的地址
 @param pData -[out] 指向需要操作的数据
 @param dataLen -[in] 数据长度
 @return 读出成功的字节数
*/
void Internal_ReadFlash(uint32_t addrStart, uint32_t *pData, uint32_t dataLen)
{
	uint32_t i;
	for(i = 0; i < dataLen; i++)
	{
		pData[i] = InternalFlash__ReadWord(addrStart);//读取4个字节.
		addrStart += 4;//偏移4个字节.	
	}
}

/**
 @brief 内部Flash写入
 @param address -[in] 写入的地址
 @param pData -[in] 指向需要操作的数据
 @param dataLen -[in] 数据长度
 @return 无
*/
void Internal_WriteFlash(uint32_t addrStart, const uint32_t *pData, uint32_t dataLen)
{   
	FLASH_EraseInitTypeDef FlashEraseInit;
	HAL_StatusTypeDef FlashStatus=HAL_OK;
	uint32_t SectorError=0;
	uint32_t addrx=0;
	uint32_t endaddr=0;	
	if(addrStart<STM32_FLASH_BASE||addrStart%4)return;	//非法地址
    
	HAL_FLASH_Unlock();             //解锁	
	addrx=addrStart;				//写入的起始地址
	endaddr=addrStart+dataLen*4;	//写入的结束地址
    
	if(FlashStatus==HAL_OK)
	{
		 while(addrStart<endaddr)//写数据
		 {
			if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,addrStart,*pData)!=HAL_OK)//写入数据
			{ 
				break;	//写入异常
			}
			addrStart+=4;
			pData++;
		}  
	}
	HAL_FLASH_Lock();           //上锁       
}

/**
 @brief 内部Flash页擦除
 @param pageAddress -[in] 擦除的起始地址
 @param nbPages -[in] 擦除页数
 @return 0 - 成功；-1 - 失败
*/
int Internal_EraseSectors(uint32_t pageAddress, uint32_t nbSectors)
{
	FLASH_EraseInitTypeDef FlashEraseInit;
	uint32_t SectorError=0;    
    
	HAL_FLASH_Unlock();             //解锁   
    FlashEraseInit.TypeErase=FLASH_TYPEERASE_SECTORS;       //擦除类型，扇区擦除 
    FlashEraseInit.Sector=InternalFlash_GetFlashSector(pageAddress);   //要擦除的扇区
    FlashEraseInit.NbSectors=nbSectors;                             //擦除的扇区个数
    FlashEraseInit.VoltageRange=FLASH_VOLTAGE_RANGE_3;      //电压范围，VCC=2.7~3.6V之间!!
    if(HAL_FLASHEx_Erase(&FlashEraseInit,&SectorError)!=HAL_OK) 
    {
        HAL_FLASH_Lock();           //上锁
        return -1;//发生错误了	
    }
    FLASH_WaitForLastOperation(FLASH_WAITETIME);                //等待上次操作完成    
    HAL_FLASH_Lock();           //上锁
    return 0;
}

/**
 @brief 内部Flash查找可写入标签记录的起始地址
 @param addrStart -[in] 查找区间起始地址
 @param dataLen -[in] 数据长度
 @return 可写入标签记录的起始地址
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
 @brief 内部Flash查找指定标签TID记录的起始地址
 @param addrStart -[in] 查找区间起始地址
 @param dataLen -[in] 数据长度
 @return 指定标签TID记录的起始地址
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
 @brief 内部Flash写入标签记录
 @param head_ref -[in] 指向需要操作的数据链表头
 @param dataLen -[in] 单个链表结点数据长度
 @return 无
*/
void InternalFlash_WriteRecord(item_info_t** head_ref, uint32_t dataLen)
{
    item_info_t* current = *head_ref;
    uint32_t cnt = 0;
    uint32_t addrStart = 0;
    
    while (current != NULL) {
        //根据不同的mark，用不同的方式写记录至flash
        if (current->mark == MARK_INSTORAGE) {
            addrStart = InternalFlash_FindStartAddr(FLASH_USER_START_ADDR, dataLen); //查找可写入标签记录的起始地址
            if (addrStart >= FLASH_USER_START_ADDR && addrStart <= FLASH_USER_END_ADDR - dataLen) { //地址合法
                if (0 == InternalFlash_FindTIDAddr(current->TID, FLASH_USER_START_ADDR, dataLen))   //是新的标签
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
 @brief 内部Flash上传标签记录
 @param item_info_t *item_info
 @return 无
*/
void InternalFlash_UploadRecord(item_info_t *item_info)
{
        /* 通过串口3发送cmd给上位机 */
        /* 数据格式：帧头0xFC,0xFC，帧尾\r\n, 内容："名称","货架号","层号","TID","入库时间","出库时间",
            例：'0xFC''0xFC'cola,1,1,69952000400582D1160A0000,2023yy5mm1dd21:28,NULL,'\r''\n'
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
 @brief 内部Flash读取标签记录
 @param dataLen -[in] 单个链表结点数据长度
 @return 无
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
 @brief 内部Flash重置标签记录区
 @param addrStart -[in] 查找区间起始地址
 @param addrEnd -[in] 查找区间结束地址
 @return 无
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
 @brief 内部Flash更新标签记录
 @param head_ref -[in] 指向需要操作的数据链表头
 @param dataLen -[in] 数据长度
 @return 无
*/
static void InternalFlash_UpdateRecord(item_info_t* item_info, uint32_t dataLen)
{
    item_info_t* item_info_new = malloc(sizeof(item_info_t));
    uint32_t addrStart = 0; 
    
   addrStart = InternalFlash_FindTIDAddr(item_info->TID, FLASH_USER_START_ADDR, dataLen);
    if (addrStart >= FLASH_USER_START_ADDR && addrStart <= FLASH_USER_END_ADDR - dataLen) { //地址合法
        Internal_ReadFlash(addrStart, (uint32_t *)item_info_new, dataLen);
        item_info_new->mark = item_info->mark;
        item_info_new->outstorage_time = item_info->outstorage_time;
        Internal_WriteFlash(addrStart, (uint32_t *)item_info_new, dataLen);
    }
    free(item_info_new);
}

/*********************************************END OF FILE**********************/

