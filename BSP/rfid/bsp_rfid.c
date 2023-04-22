/**
  ******************************************************************************
  * @file    bsp_rfid.c
  * @author  SwaveYu
  * @version V1.0
  * @date    2023-xx-xx
  * @brief   RFID扫描应用
  ******************************************************************************
  * @attention
  *
  * 
  * 
  * 
  *
  ******************************************************************************
  */ 
  
#include ".\rfid\bsp_rfid.h" 
#include ".\rfid\bsp_rf200_rfid.h" 
#include "usart.h" 

uint8_t TagAccessPassword[4]={0x00,0x00,0x00,0x00};
uint8_t TagWriteDATA[8]={0x12,0x34,0x56,0x78,0x78,0x56,0x34,0x16};

uint8_t tag_cnt = 0;    // 标签计数
tag_filter_t tag_filter;

uint8_t rfid_scan_enable = SCAN_DISABLE;  // 扫描使能
uint8_t rfid_scan_mode = SCAN_MODE_INSTORAGE_SINGLE;  // 扫描模式

/**
  * @brief  获取单个RFID电子标签的TID(96位)
  * @param  无
  * @retval 无
  */	
void RFID_GetSingleTID()
{
    if (RF200Status.ReadDataFlag == 0 && tag_cnt < MAX_TAG_NUM) {
        TAG_WR_PARA.AccessPW[0]=TagAccessPassword[0]; //ACCESS PASSWORD
        TAG_WR_PARA.AccessPW[1]=TagAccessPassword[1];
        TAG_WR_PARA.AccessPW[2]=TagAccessPassword[2];
        TAG_WR_PARA.AccessPW[3]=TagAccessPassword[3];
        TAG_WR_PARA.MemBank=Membank_TID;
        TAG_WR_PARA.Offset=0;// OFFSET 0 WORD;(2BYTE=1WORD)
        TAG_WR_PARA.len=6;//LENGTH 4 WORDS=8 BYTES;
        TAG_WR_PARA.dat=TAG_READ_DATA.dat;// data buffer
        TagReadProcess(TAG[0].TID,TAG_WR_PARA);
    }
    
    USART_RXD_Data_Process();
    RF200_FRAME_RX_HANDLE();	    
    
    if (RF200Status.ReadDataFlag == 1) {
        for(uint32_t i=0;i<12;i++){
            TAG[tag_cnt].TID[i]=TAG_READ_DATA.dat[i];// copy to frame handle buffer;
        } 
        tag_cnt++;
        RF200Status.ReadDataFlag = 0;
    }
}

/**
  * @brief  获取多个RFID电子标签的TID(96位)
  * @param  无
  * @retval 无
  */	
void RFID_GetMultiTID()
{

}

/**
  * @brief  筛选出的RFID电子标签
  * @param  处理的标签数
  * @retval 无
  */	
void RFID_FilterTag(uint8_t *cnt)
{
    uint32_t i,j;
    uint8_t same_mark[MAX_TAG_NUM] = {0};
    tag_filter.len =  0;
    
    for (i = 0; i < *cnt; i++) {
        if (same_mark[i] == 0) {
            memcpy(&(tag_filter.tag[tag_filter.len++]), TAG[i].TID, 12);
        } else continue;
        
        for (j = i; j < *cnt; j++) {
            if (same_mark[j] == 0) {
                if (memcmp(TAG[i].TID, TAG[j].TID, 12) == 0) same_mark[j] = 1;
            }
        }
    }
    
    for (uint32_t i = 0; i < tag_filter.len; i++) {
        printf("TID[%u]:", i);
        for(uint32_t j=0;j<12;j++){
            printf("%02X ",tag_filter.tag[i].TID[j]);
        } 
        printf("\r\n");
    }
   *cnt = 0;
}

///*************Single Inventory example *****************/
//void contain_test(void)
//{
//	SendCmdFrame(MultiInventCMD);
//    
//    USART_RXD_Data_Process();
//    RF200_FRAME_RX_HANDLE();		
//}

///*************Multi Inventory example *****************/
//void contain_test1(void)
//{
//	SendCmdFrame(MultiInventCMD);

//    USART_RXD_Data_Process();
//    RF200_FRAME_RX_HANDLE();		
//}
///*************Read/Write User bank example*****************/
//void contain_test2(void)
//{
//	if(first_flag){
//			SendCmdFrame(StopMultiInventCMD);//停止轮询
//			delay_ms(100);
//			//set select mode
//			SendCmdFrame(SetSelectModeCMD);//写标签之前应先设置 Select 参数，以便选择指定的标签进行写标签数据区操作
//			first_flag = 0;
//	}
//    //first step inventory a tag EPC;
//    if((flag1s == 1)&&(RF200Status.InventATagFlag == 0))// no tag
//    {
//        SendCmdFrame(SingleInventCMD);
//        printf("NO TAG\r\n");
//    }
//    //second step write TagWriteDATA array data to the first tag;
//    if((RF200Status.InventATagFlag == 1)&&(RF200Status.WriteDataFlag == 0)&&(flag1s == 1))
//    {
//        TAG_WR_PARA.AccessPW[0]=TagAccessPassword[0]; //ACCESS PASSWORD
//        TAG_WR_PARA.AccessPW[1]=TagAccessPassword[1];
//        TAG_WR_PARA.AccessPW[2]=TagAccessPassword[2];
//        TAG_WR_PARA.AccessPW[3]=TagAccessPassword[3];
//        TAG_WR_PARA.MemBank=Membank_USER;
//        TAG_WR_PARA.Offset=0;// OFFSET 0 WORD;(2BYTE=1WORD)
//        TAG_WR_PARA.len=4;//LENGTH 4 WORDS=8 BYTES;
//        TAG_WR_PARA.dat=TagWriteDATA;
//        TagWriteProcess(TAG[0].EPC,TAG_WR_PARA);//写读到的第一个标签
//        printf("write successful\r\n");
//        flag1s = 0; //1s;
//    }
//    //third step read the first tag data to TagReadDATA array;
//    if((RF200Status.InventATagFlag == 1)&&(RF200Status.WriteDataFlag == 1)&&(flag1s == 1))
//    {
//        TAG_WR_PARA.AccessPW[0]=TagAccessPassword[0]; //ACCESS PASSWORD
//        TAG_WR_PARA.AccessPW[1]=TagAccessPassword[1];
//        TAG_WR_PARA.AccessPW[2]=TagAccessPassword[2];
//        TAG_WR_PARA.AccessPW[3]=TagAccessPassword[3];
//        TAG_WR_PARA.MemBank=Membank_USER;
//        TAG_WR_PARA.Offset=0;// OFFSET 0 WORD;(2BYTE=1WORD)
//        TAG_WR_PARA.len=4;//LENGTH 4 WORDS=8 BYTES;
//        TAG_WR_PARA.dat=TAG_READ_DATA.dat;// data buffer
//        TagReadProcess(TAG[0].EPC,TAG_WR_PARA);
//        printf("read successful,the data is %s\r\n",TAG_READ_DATA.dat);
//        flag1s = 0; //1s;
//    }
//    //all done and stop running; user can check TAG_READ_DATA struct 
//    if((RF200Status.InventATagFlag == 1)&&(EPC_Compare(TAG[0].EPC,TAG_READ_DATA.EPC) == 1)&&(flag1s == 1))
//    {
//        //check the data in TAG_WR_PARA.dat or TAG_READ_DATA.dat;
//        while(1);
//    }
//    USART_RXD_Data_Process();
//    RF200_FRAME_RX_HANDLE();		
//}


/*********************************************END OF FILE**********************/

