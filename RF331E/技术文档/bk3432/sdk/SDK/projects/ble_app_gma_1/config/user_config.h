/**
 *******************************************************************************
 *
 * @file user_config.h
 *
 * @brief Application configuration definition
 *
 * Copyright (C) RivieraWaves 2009-2016
 *
 *******************************************************************************
 */
 
#ifndef _USER_CONFIG_H_
#define _USER_CONFIG_H_


 
 /******************************************************************************
  *############################################################################*
  * 							SYSTEM MACRO CTRL                              *
  *############################################################################*
  *****************************************************************************/

//如果需要使用GPIO进行调试，需要打开这个宏
#define GPIO_DBG_MSG					0
//UART使能控制宏
#define UART_PRINTF_EN					1
//蓝牙硬件调试控制
#define DEBUG_HW						0




 
/*******************************************************************************
 *#############################################################################*
 *								APPLICATION MACRO CTRL                         *
 *#############################################################################*
 *******************************************************************************/
 
//连接参数更新控制
#define UPDATE_CONNENCT_PARAM  			1

//最小连接间隔
#define BLE_UAPDATA_MIN_INTVALUE		20
//最大连接间隔 
#define BLE_UAPDATA_MAX_INTVALUE		40
//连接Latency
#define BLE_UAPDATA_LATENCY				0
//连接超时
#define BLE_UAPDATA_TIMEOUT				600


//设备名称
#define APP_DFLT_DEVICE_NAME           ("BK3432-GATT")


 //广播包UUID配置
#define APP_FFF0_ADV_DATA_UUID        "\x03\x03\xF0\xFF"
#define APP_FFF0_ADV_DATA_UUID_LEN    (4)

//扫描响应包数据
#define APP_SCNRSP_DATA        "\x0c\x08\x42\x4B\x33\x34\x33\x32\x2D\x47\x41\x54\x54" //BK3432-GATT"
#define APP_SCNRSP_DATA_LEN     (13)


//广播参数配置
/// Advertising channel map - 37, 38, 39
#define APP_ADV_CHMAP           (0x07)
/// Advertising minimum interval - 100ms (160*0.625ms)
#define APP_ADV_INT_MIN         (160)
/// Advertising maximum interval - 100ms (160*0.625ms)
#define APP_ADV_INT_MAX         (160)
/// Fast advertising interval
#define APP_ADV_FAST_INT        (32)



#define HEADER_APP_FADDR   65  ////65k
/*******************************************************************************
 *#############################################################################*
 *								DRIVER MACRO CTRL                              *
 *#############################################################################*
 ******************************************************************************/

//DRIVER CONFIG
#define UART_DRIVER						1
#define UART2_DRIVER				  0
#define GPIO_DRIVER						1
#define AUDIO_DRIVER					0
#define RTC_DRIVER						0
#define ADC_DRIVER						0
#define I2C_DRIVER						0
#define PWM_DRIVER						1

#define GMA_SUPPORT     1 //Alibaba GMA protocol
#if GMA_SUPPORT
#define GMA_AES         1 //Alibaba GMA aes
#define GMA_OTA         1 //Alibaba GMA aes
#include "gma.h"
#endif


#endif /* _USER_CONFIG_H_ */
