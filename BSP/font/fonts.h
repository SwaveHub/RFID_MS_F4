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

//������Ϣ�����ַ,ռ33���ֽ�,��1���ֽ����ڱ���ֿ��Ƿ����.����ÿ8���ֽ�һ��,�ֱ𱣴���ʼ��ַ���ļ���С														   
extern uint32_t FONTINFOADDR;	
//�ֿ���Ϣ�ṹ�嶨��
//���������ֿ������Ϣ����ַ����С��
__packed typedef struct 
{
	uint8_t fontok;				//�ֿ���ڱ�־��0XAA���ֿ��������������ֿⲻ����
	uint32_t ugbkaddr; 			//unigbk�ĵ�ַ
	uint32_t ugbksize;			//unigbk�Ĵ�С	 
	uint32_t f12addr;			//gbk12��ַ	
	uint32_t gbk12size;			//gbk12�Ĵ�С	 
	uint32_t f16addr;			//gbk16��ַ
	uint32_t gbk16size;			//gbk16�Ĵ�С		 
	uint32_t f24addr;			//gbk24��ַ
	uint32_t gkb24size;			//gbk24�Ĵ�С 
}_font_info; 


extern _font_info ftinfo;	//�ֿ���Ϣ�ṹ��

/*******************����********** ����ʾ������ʾ���ַ���С ***************************/
#define      WIDTH_CH_CHAR		                32	    //�����ַ���� 
#define      HEIGHT_CH_CHAR		              	32		  //�����ַ��߶� 



#define LINE(x) ((x) * (((sFONT *)LCD_GetFont())->Height))

//LINEYͳһʹ�ú�����ģ�ĸ߶�
#define LINEY(x) ((x) * (WIDTH_CH_CHAR))




//0��ʾʹ��SD����ģ�������ʾFLASH��ģ,����SD����ģ���ļ�ϵͳ���ٶ����ܶࡣ

#define GBKCODE_FLASH 						1

#if GBKCODE_FLASH
	/*ʹ��FLASH��ģ*/
	/*�����ֿ�洢��FLASH����ʼ��ַ*/
	/*FLASH*/
	#define GBKCODE_START_ADDRESS   512*4096


	/*��ȡ�ֿ�ĺ���*/
	//�����ȡ�����ַ���ģ����ĺ�������ucBufferΪ�����ģ��������usCharΪ�����ַ��������룩
	#define      GetGBKCode( ucBuffer, usChar )  GetGBKCode_from_EXFlash( ucBuffer, usChar )  
	int GetGBKCode_from_EXFlash( uint8_t * pBuffer, uint16_t c);
    uint8_t font_init(void);
#else
	/*ʹ��SD��ģ*/


	/*SD����ģ·��*/
	#define GBKCODE_FILE_NAME			"0:/Font/GBK.FON"


	/*��ȡ�ֿ�ĺ���*/
	//�����ȡ�����ַ���ģ����ĺ�������ucBufferΪ�����ģ��������usCharΪ�����ַ��������룩

	#define GetGBKCode( ucBuffer, usChar )  GetGBKCode_from_sd( ucBuffer, usChar )
	int GetGBKCode_from_sd ( uint8_t * pBuffer, uint16_t c);

#endif

extern  const unsigned char asc2_1206[95][12];
extern  const unsigned char asc2_1608[95][16];
extern  const unsigned char asc2_2412[95][36];
extern  const unsigned char asc2_3216[95][128];    
#endif /*end of __FONT_H    */
