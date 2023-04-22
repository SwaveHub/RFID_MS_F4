/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bsp.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId LVGL_TaskHandle;
osThreadId LedToggle_TaskHandle;
osThreadId Protocol_TaskHandle;
osThreadId KeyScan_TaskHandle;
osThreadId RFIDScan_TaskHandle;
osMessageQId KeySta_QueueHandle;
osMessageQId LVGL_Refresh_QueueHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void Start_LVGL_Task(void const * argument);
void Start_LedToggle_Task(void const * argument);
void Start_Protocol_Task(void const * argument);
void Start_KeyScan_Task(void const * argument);
void Start_RFIDScan_Task(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* definition and creation of KeySta_Queue */
  osMessageQDef(KeySta_Queue, 1, uint8_t);
  KeySta_QueueHandle = osMessageCreate(osMessageQ(KeySta_Queue), NULL);
    
  /* definition and creation of LVGL_Refresh_Queue */
  osMessageQDef(LVGL_Refresh_Queue, 3, uint8_t);
  LVGL_Refresh_QueueHandle = osMessageCreate(osMessageQ(LVGL_Refresh_Queue), NULL);
  
  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of LVGL_Task */
  osThreadDef(LVGL_Task, Start_LVGL_Task, osPriorityHigh, 0, 512);
  LVGL_TaskHandle = osThreadCreate(osThread(LVGL_Task), NULL);

  /* definition and creation of LedToggle_Task */
  osThreadDef(LedToggle_Task, Start_LedToggle_Task, osPriorityIdle, 0, 128);
  LedToggle_TaskHandle = osThreadCreate(osThread(LedToggle_Task), NULL);

  /* definition and creation of Protocol_Task */
  osThreadDef(Protocol_Task, Start_Protocol_Task, osPriorityAboveNormal, 0, 128);
  Protocol_TaskHandle = osThreadCreate(osThread(Protocol_Task), NULL);

  /* definition and creation of KeyScan_Task */
  osThreadDef(KeyScan_Task, Start_KeyScan_Task, osPriorityIdle, 0, 128);
  KeyScan_TaskHandle = osThreadCreate(osThread(KeyScan_Task), NULL);

  /* definition and creation of RFIDScan_Task */
  osThreadDef(RFIDScan_Task, Start_RFIDScan_Task, osPriorityRealtime, 0, 128);
  RFIDScan_TaskHandle = osThreadCreate(osThread(RFIDScan_Task), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_Start_LVGL_Task */
/**
  * @brief  Function implementing the LVGL_Task thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_Start_LVGL_Task */
void Start_LVGL_Task(void const * argument)
{
  /* USER CODE BEGIN Start_LVGL_Task */
    osEvent lv_evt;        
  /* Infinite loop */
  for(;;)
  {
    lv_evt = osMessageGet(LVGL_Refresh_QueueHandle, 10);
    switch (lv_evt.value.v) {
        case LVGL_TIME_REFRESH:
            time_update();
            break;
        case LVGL_INSTORAGE_SINGLE_RECORD_ADD:
            screen_instorage_record_add(TAG[0].TID);
            break;
        case LVGL_INSTORAGE_MULTI_RECORD_ADD:
            for (uint32_t i = 0; i < tag_filter.len; i++) {
                screen_instorage_record_add(tag_filter.tag[i].TID);
            }
            break;
        case LVGL_OUTSTORAGE_SINGLE_RECORD_ADD:
            screen_outstorage_record_add(TAG[0].TID);
            break;
        case LVGL_OUTSTORAGE_MULTI_RECORD_ADD:
            for (uint32_t i = 0; i < tag_filter.len; i++) {
                screen_outstorage_record_add(tag_filter.tag[i].TID);
            }            
            break;
        default: 
            break;
    }
    lv_task_handler();//lvgl的事务处理
    osDelay(1);      
  }
  /* USER CODE END Start_LVGL_Task */
}

/* USER CODE BEGIN Header_Start_LedToggle_Task */
/**
* @brief Function implementing the LedToggle_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_LedToggle_Task */
void Start_LedToggle_Task(void const * argument)
{
  /* USER CODE BEGIN Start_LedToggle_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(500);
    HAL_GPIO_TogglePin(LED0_GPIO_Port,LED0_Pin);
    osDelay(500);
    osMessagePut(LVGL_Refresh_QueueHandle, LVGL_TIME_REFRESH, 20); 
  }
  /* USER CODE END Start_LedToggle_Task */
}

/* USER CODE BEGIN Header_Start_Protocol_Task */
/**
* @brief Function implementing the Protocol_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_Protocol_Task */
void Start_Protocol_Task(void const * argument)
{
  /* USER CODE BEGIN Start_Protocol_Task */
  /* Infinite loop */
  for(;;)
  {
    if (Uart1_Rx_Sta == 1) {
        printf("Uart1_Rx:%s\r\n", Uart1_Rx_Buf);
        Protocol_Decode(PROTOCOL_MODE_DEFAULT, Uart1_Rx_Buf, strlen(Uart1_Rx_Buf));  
        Uart1_Rx_Sta = 0;
        RTC_Get(&system_time.year, &system_time.month, &system_time.day, 
            &system_time.hour, &system_time.minute, &system_time.second);
        printf("%d-%d-%d %d:%d:%d\r\n", system_time.year, system_time.month, system_time.day,
            system_time.hour, system_time.minute, system_time.second);          
    }   
    osDelay(10);    
  }
  /* USER CODE END Start_Protocol_Task */
}

/* USER CODE BEGIN Header_Start_KeyScan_Task */
/**
* @brief Function implementing the KeyScan_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_KeyScan_Task */
void Start_KeyScan_Task(void const * argument)
{
  /* USER CODE BEGIN Start_KeyScan_Task */
  /* Infinite loop */
  for(;;)
  {
	/*检测是否有按键按下 */
	if(HAL_GPIO_ReadPin(SCANKEY_GPIO_PORT,SCANKEY_PIN) == KEY_ON )  
	{
        osMessagePut(KeySta_QueueHandle, KEY_PRESS, osWaitForever);        
		/*等待按键释放 */
		while(HAL_GPIO_ReadPin(SCANKEY_GPIO_PORT,SCANKEY_PIN) == KEY_ON);   
        osMessagePut(KeySta_QueueHandle, KEY_RELEASE, osWaitForever); 
	}
    osDelay(20);
  }
  /* USER CODE END Start_KeyScan_Task */
}

/* USER CODE BEGIN Header_Start_RFIDScan_Task */
/**
* @brief Function implementing the RFIDScan_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_RFIDScan_Task */
void Start_RFIDScan_Task(void const * argument)
{
  /* USER CODE BEGIN Start_RFIDScan_Task */
    uint8_t scan_sta = KEY_RELEASE;
    osEvent evt;    
  /* Infinite loop */
  for(;;)
  {
    evt = osMessageGet(KeySta_QueueHandle, 20);
    if (rfid_scan_enable == SCAN_ENABLE) { //RFID扫描使能
        if (evt.value.v == KEY_PRESS) {
            scan_sta = KEY_PRESS;
        } else if (evt.value.v == KEY_RELEASE) {
            scan_sta = KEY_RELEASE;
            if (rfid_scan_mode == SCAN_MODE_INSTORAGE_MULTI || rfid_scan_mode == SCAN_MODE_OUTSTORAGE_MULTI) { //RFID连续扫描模式，松开按键后进行标签筛选及显示
                RFID_FilterTag(&tag_cnt);
                    if (rfid_scan_mode == SCAN_MODE_INSTORAGE_MULTI) {
                        osMessagePut(LVGL_Refresh_QueueHandle, LVGL_INSTORAGE_MULTI_RECORD_ADD, 20); 
                    }
                    else  {
                        osMessagePut(LVGL_Refresh_QueueHandle, LVGL_OUTSTORAGE_MULTI_RECORD_ADD, 20); 
                    }
            }
            tag_cnt = 0;
            HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
        }
        if (scan_sta != KEY_RELEASE) { //RFID扫描按键未松开
            if (rfid_scan_mode == SCAN_MODE_INSTORAGE_SINGLE || rfid_scan_mode == SCAN_MODE_OUTSTORAGE_SINGLE) { //RFID单次扫描模式
                if (tag_cnt == 0) {
                    RFID_GetSingleTID();
                    HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);        
                    if (tag_cnt != 0) {
                        if (rfid_scan_mode == SCAN_MODE_INSTORAGE_SINGLE) {
                            osMessagePut(LVGL_Refresh_QueueHandle, LVGL_INSTORAGE_SINGLE_RECORD_ADD, 20); 
                        } else {
                            osMessagePut(LVGL_Refresh_QueueHandle, LVGL_OUTSTORAGE_SINGLE_RECORD_ADD, 20); 
                        }
                    }
                }          
            } else if (rfid_scan_mode == SCAN_MODE_INSTORAGE_MULTI || rfid_scan_mode == SCAN_MODE_OUTSTORAGE_MULTI) { //RFID连续扫描模式
                RFID_GetSingleTID();
                HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);                
            }
        }        
    }
    osDelay(10);
  }
  /* USER CODE END Start_RFIDScan_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

