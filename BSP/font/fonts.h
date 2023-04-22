#ifndef __FONT_H
#define __FONT_H       

#include "stm32f4xx_hal.h"



/** @defgroup FONTS_Exported_Types
  * @{
  */ 
typedef struct _tFont
{    
  const uint8_t *table;
  uint16_t Width;
  uint16_t Height;
  
} sFONT;

extern sFONT Font24x48;
extern sFONT Font16x32;
extern sFONT Font8x16;

//字体信息保存地址,占33个字节,第1个字节用于标记字库是否存在.后续每8个字节一组,分别保存起始地址和文件大小														   
extern uint32_t FONTINFOADDR;	
//字库信息结构体定义
//用来保存字库基本信息，地址，大小等
__packed typedef struct 
{
	uint8_t fontok;				//字库存在标志，0XAA，字库正常；其他，字库不存在
	uint32_t ugbkaddr; 			//unigbk的地址
	uint32_t ugbksize;			//unigbk的大小	 
	uint32_t f12addr;			//gbk12地址	
	uint32_t gbk12size;			//gbk12的大小	 
	uint32_t f16addr;			//gbk16地址
	uint32_t gbk16size;			//gbk16的大小		 
	uint32_t f24addr;			//gbk24地址
	uint32_t gkb24size;			//gbk24的大小 
}_font_info; 


extern _font_info ftinfo;	//字库信息结构体

/*******************中文********** 在显示屏上显示的字符大小 ***************************/
#define      WIDTH_CH_CHAR		                32	    //中文字符宽度 
#define      HEIGHT_CH_CHAR		              	32		  //中文字符高度 



#define LINE(x) ((x) * (((sFONT *)LCD_GetFont())->Height))

//LINEY统一使用汉字字模的高度
#define LINEY(x) ((x) * (WIDTH_CH_CHAR))




//0表示使用SD卡字模，非零表示FLASH字模,由于SD卡字模有文件系统，速度慢很多。

#define GBKCODE_FLASH 						1

#if GBKCODE_FLASH
	/*使用FLASH字模*/
	/*中文字库存储在FLASH的起始地址*/
	/*FLASH*/
	#define GBKCODE_START_ADDRESS   512*4096


	/*获取字库的函数*/
	//定义获取中文字符字模数组的函数名，ucBuffer为存放字模数组名，usChar为中文字符（国标码）
	#define      GetGBKCode( ucBuffer, usChar )  GetGBKCode_from_EXFlash( ucBuffer, usChar )  
	int GetGBKCode_from_EXFlash( uint8_t * pBuffer, uint16_t c);
    uint8_t font_init(void);
#else
	/*使用SD字模*/


	/*SD卡字模路径*/
	#define GBKCODE_FILE_NAME			"0:/Font/GBK.FON"


	/*获取字库的函数*/
	//定义获取中文字符字模数组的函数名，ucBuffer为存放字模数组名，usChar为中文字符（国标码）

	#define GetGBKCode( ucBuffer, usChar )  GetGBKCode_from_sd( ucBuffer, usChar )
	int GetGBKCode_from_sd ( uint8_t * pBuffer, uint16_t c);

#endif

extern  const unsigned char asc2_1206[95][12];
extern  const unsigned char asc2_1608[95][16];
extern  const unsigned char asc2_2412[95][36];
extern  const unsigned char asc2_3216[95][128];    
#endif /*end of __FONT_H    */
