
/**--------------File Info------------------------------------------------------------------------------
** File name:			  rf200_drv.c
** Last modified Date:  2018-05-30
** Last Version:		V1.00
** Descriptions:		RF200 FRAME HANDLE
**------------------------------------------------------------------------------------------------------
** Created date:		2018-05-30
** Version:			  	V1.00
** Descriptions:		RF200 FRAME HANDLE
**
********************************************************************************************************/
#define _RF200_DRV_MANL
#include ".\rfid\bsp_rf200_rfid.h"
#include "bsp.h"



uint8_t SingleInventCMD[]=	{0xBB,0x00,0x22,0x00,0x00,0x22,0x7E};//单次轮询指令
uint8_t MultiInventCMD[]=		{0xBB,0x00,0x27,0x00,0x03,0x22,0xFF,0xFF,0x4A,0x7E};
uint8_t StopMultiInventCMD[]=	{0xBB,0x00,0x28,0x00,0x00,0x28,0x7E };//停止多次轮询
uint8_t SetRegionUSCMD[]=		{0xBB,0x00,0x07,0x00,0x01,0x02,0x0A,0x7E};
uint8_t SetRegionCH2CMD[]=	{0xBB,0x00,0x07,0x00,0x01,0x01,0x09,0x7E};
uint8_t SetSelectModeCMD[]=	{0xBB,0x00,0x12,0x00,0x01,0x00,0x13,0x7E};
uint8_t SetSelectParaCMD[]=	{0xBB,0x00,0x0C,0x00,0x13,0x23,0x00,0x00,0x00,0x20,0x60,
                            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC2,0x7E};
uint8_t ReadDataCMD[]=			{0xBB,0x00,0x39,0x00,0x09,0x00,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x04,0x49,0x7E}; //读TID指令BB 00 39 00 09 00 00 00 00 02 00 00 00 06 4A 7E 
uint8_t WriteDataCMD[]=			{0xBB,0x00,0x49,0x00,0x11,0x00,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x04,
                             0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x01,0x02,0x03,0x04,0x05,0x06,
														 0x07,0x08,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x01,0x02,0x03,0x04,
														 0x05,0x06,0x07,0x08,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x01,0x02,
                             0x03,0x04,0x05,0x06,0x07,0x08,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,
                             0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x01,0x02,0x03,0x04,0x05,0x06,
                             0x07,0x08,0x85,0x7E };

							 
int Exist_Flag = 0;
//从串口缓存中拷贝数据
void Usart_buff_copy(uint8_t *usart_buff,uint8_t data_len)
{
	uint8_t i;
	for(i=0;i<data_len;i++) {
        RF200FRAME.dat[i]=usart_buff[i];// copy to frame handle buffer;
    }
	RF200FRAME.byteLen=data_len;
}


void Save_Tag(unsigned char *FrameDat)
{
	uint8_t i;
	printf("EPC:");
	for(i=0;i<12;i++){
		TAG[TAG_Index].EPC[i]=FrameDat[i+8];// copy to frame handle buffer;
		printf("%x ",TAG[TAG_Index].EPC[i]);
	}
	printf("\r\n");
	TAG[TAG_Index].RSSI=0XFF-FrameDat[5];
	printf("RSSI:%d\n",TAG[TAG_Index].RSSI);
	TAG[TAG_Index].flag = 1;

	TAG_Index++;
	
	if(TAG_Index>=MAX_TAG_NUM)
		TAG_Index = 0;
}
void Save_Tag_Data(unsigned char *FrameDat)
{
	uint8_t i;
	for(i=0;i<12;i++)
		TAG_READ_DATA.EPC[i]=FrameDat[i+8];// EPC copy ;
	TAG_READ_DATA.len=FrameDat[4]-15;
	for(i=0;i<TAG_READ_DATA.len;i++)
		TAG_READ_DATA.dat[i]=FrameDat[i+20];// data copy ;
}

uint8_t RF200_FRAME_RX_HANDLE(void)
{
	if(RF200FRAME.byteLen == 0) //没有数据
	{
		Exist_Flag = 0;
		return 0;//error
	}
	if(RF200FRAME.dat[1]==FRAME_RES)//是响应帧，用来反映指令是否被执行
	{
		Exist_Flag = 0;
		switch(RF200FRAME.dat[2])//指令代码
		{
			case CMD_STOP_MULTI :
				RF200Status.MultiInventFlag = 0;//标记状态，表示芯片已经停止轮询
				break;
			case CMD_READ_DATA :
				RF200Status.ReadDataFlag = 1;//成功读取
				Save_Tag_Data(RF200FRAME.dat);//从原始数据中提取epc
				break;
			case CMD_WRITE_DATA :
				RF200Status.WriteDataFlag = 1;
				break;
			case CMD_SET_REGION :
				RF200Status.SetRegionFlag = 1;
				break;
			case CMD_SET_POWER :
				RF200Status.SetPowerFlag = 1;
				break;
			case CMD_SET_SELECT_PARA :
				RF200Status.SetSelectParaFlag = 1;
				break;
			case CMD_SET_INV_MODE : //select mode
				RF200Status.SetSelectModeFlag = 1;
				break;
			case CMD_FAIL :
				break;
			default :
				break;
		}
	}
	else if(RF200FRAME.dat[1]==FRAME_ERROR)//frame type error
	{
		Exist_Flag = 0;
		return 0;//error
	}
	else if(RF200FRAME.dat[1]==FRAME_INFO)//是通知帧，读到一个标签就发一个通知帧
	{
		Exist_Flag = 1;
		switch(RF200FRAME.dat[2])//command code
		{
			case CMD_SINGLE_ID :
				Save_Tag(RF200FRAME.dat);
			  RF200Status.InventATagFlag = 1;
			  RF200Status.TotalTags++;
				break;
			case CMD_MULTI_ID :
				Save_Tag(RF200FRAME.dat);
			  RF200Status.InventATagFlag = 1;
			  RF200Status.TotalTags++;
				break;
			default :
				break;
		}
	}
	RF200FRAME.byteLen =0;
	return 1;
}
uint8_t Rf200_checksum(uint8_t * t_dat)
{
	uint8_t checksum,i,cmdDatLen;
	cmdDatLen = t_dat[4];
  checksum = t_dat[1] + t_dat[2] + t_dat[4];
	for (i = 0; i < cmdDatLen; i++)
	{
		checksum += t_dat[i+5];
	}
	t_dat[cmdDatLen+5]=checksum;
	return 0;
}
void TagReadProcess(uint8_t *t_EPC,TAG_WR_PARA_t t_TAG_WR_PARA)
{
	uint8_t i;
    if (t_TAG_WR_PARA.MemBank == Membank_EPC) {
        for (i = 0; i < 12; i++)//set epc
        {
            SetSelectParaCMD[i+12] = t_EPC[i];
        }
        SetSelectParaCMD[5] = (SetSelectParaCMD[5]&0xf0)+t_TAG_WR_PARA.MemBank;        
        Rf200_checksum(SetSelectParaCMD);
        SendCmdFrame(SetSelectParaCMD);
        delay_ms(20);//must delay 20MS;        
    }
    
	//ACCESS PASSWORD
	ReadDataCMD[5]=t_TAG_WR_PARA.AccessPW[0];
	ReadDataCMD[6]=t_TAG_WR_PARA.AccessPW[1];
	ReadDataCMD[7]=t_TAG_WR_PARA.AccessPW[2];
	ReadDataCMD[8]=t_TAG_WR_PARA.AccessPW[3];
	//MEMBANK
	ReadDataCMD[9]=t_TAG_WR_PARA.MemBank;
	//OFFSET
	ReadDataCMD[10]=00;
	ReadDataCMD[11]=t_TAG_WR_PARA.Offset;
	//length
	ReadDataCMD[12]=00;
	ReadDataCMD[13]=t_TAG_WR_PARA.len;
	Rf200_checksum(ReadDataCMD);
	SendCmdFrame(ReadDataCMD);
//    delay_ms(20);//must delay 20MS;
}
void TagWriteProcess(uint8_t *t_EPC,TAG_WR_PARA_t t_TAG_WR_PARA)
{
	uint8_t i;
	for (i = 0; i < 12; i++)//set epc
	{
		SetSelectParaCMD[i+12] = t_EPC[i];
	}
	SetSelectParaCMD[5] = (SetSelectParaCMD[5]&0xf0)+t_TAG_WR_PARA.MemBank;
	Rf200_checksum(SetSelectParaCMD);
	SendCmdFrame(SetSelectParaCMD);
	delay_us(400);//must delay 400us;
	WriteDataCMD[4] = (t_TAG_WR_PARA.len*2)+9;// cmd data length
//ACCESS PASSWORD
	WriteDataCMD[5]=t_TAG_WR_PARA.AccessPW[0];
	WriteDataCMD[6]=t_TAG_WR_PARA.AccessPW[1];
	WriteDataCMD[7]=t_TAG_WR_PARA.AccessPW[2];
	WriteDataCMD[8]=t_TAG_WR_PARA.AccessPW[3];
	//MEMBANK
	WriteDataCMD[9]=t_TAG_WR_PARA.MemBank;
	//OFFSET
	WriteDataCMD[10]=00;
	WriteDataCMD[11]=t_TAG_WR_PARA.Offset;
	//length
	WriteDataCMD[12]=00;
	WriteDataCMD[13]=t_TAG_WR_PARA.len;
	for (i = 0; i < (t_TAG_WR_PARA.len*2); i++)//set epc
	{
		WriteDataCMD[i+14] = t_TAG_WR_PARA.dat[i];
	}
	Rf200_checksum(WriteDataCMD);
	WriteDataCMD[(t_TAG_WR_PARA.len*2)+9+6]=0x7e;//frame end
	SendCmdFrame(WriteDataCMD);
}

uint8_t EPC_Compare(uint8_t *t_EPC1,uint8_t *t_EPC2)
{
	uint8_t i;
	i=12;
	do
	{
		if(t_EPC1[i-1]!=t_EPC2[i-1])
			return 0;//different
		i--;
	}while(i);
	return 1; //same
}

