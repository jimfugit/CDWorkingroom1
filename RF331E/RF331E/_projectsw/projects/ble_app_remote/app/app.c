/**
 ****************************************************************************************
 *
 * @file app.c
 *
 * @brief Application entry point
 *
 * Copyright (C) RivieraWaves 2009-2015
 *
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup APP
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"             // SW configuration

#if (BLE_APP_PRESENT)
#include <string.h>
#include "app_task.h"                // Application task Definition
#include "app.h"                     // Application Definition
#include "gap.h"                     // GAP Definition
#include "gapm_task.h"               // GAP Manager Task API
#include "gapc_task.h"               // GAP Controller Task API

#include "co_bt.h"                   // Common BT Definition
#include "co_math.h"                 // Common Maths Definition
#include "ke_timer.h"
#include "app_wlist.h"
#include "app_sec.h"
#include "app_dis.h"                 // Device Information Service Application Definitions
#include "app_batt.h"                // Battery Application Definitions
#include "app_hid.h"                 // HID Application Definitions
#include "app_oads.h"                 // Application oads Definition
#if (NVDS_SUPPORT)
#include "nvds.h"                    // NVDS Definitions
#endif
#include "app_sec.h"
#include "rf.h"
#include "uart.h"
#include "adc.h"
#include "gpio.h"
#include "flash.h"
#include "wdt.h"
/*
 * DEFINES
 ****************************************************************************************
 */
#define APP_DEVICE_NAME_LENGTH_MAX      (20)


/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

typedef void (*appm_add_svc_func_t)(void);

//extern uint8_t app_sec_buff[sizeof(APP_SEC_DATA_T)];

/*
 * ENUMERATIONS
 ****************************************************************************************
 */

/// List of service to add in the database
enum appm_svc_list
{
    APPM_SVC_HIDS,
    APPM_SVC_DIS,
    APPM_SVC_BATT,
    APPM_SVC_LIST_STOP ,
};

/*
 * LOCAL VARIABLES DEFINITIONS
 ****************************************************************************************
 */

/// Application Task Descriptor
static const struct ke_task_desc TASK_DESC_APP = {NULL, &appm_default_handler,
	       appm_state, APPM_STATE_MAX, APP_IDX_MAX
};

/// List of functions used to create the database
static const appm_add_svc_func_t appm_add_svc_func_list[APPM_SVC_LIST_STOP] =
{
	(appm_add_svc_func_t)app_hid_add_hids,
	(appm_add_svc_func_t)app_dis_add_dis,
	(appm_add_svc_func_t)app_batt_add_bas,
};

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/// Application Environment Structure
struct app_env_tag app_env;

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

void appm_init()
{
	// Reset the application manager environment
	

	// Create APP task
	ke_task_create(TASK_APP, &TASK_DESC_APP);

	// Initialize Task state
	ke_state_set(TASK_APP, APPM_INIT);


}

bool appm_add_svc(void)
{
	// Indicate if more services need to be added in the database
	bool more_svc = false;

	// Check if another should be added in the database
	if (app_env.next_svc != APPM_SVC_LIST_STOP)
	{
		ASSERT_INFO(appm_add_svc_func_list[app_env.next_svc] != NULL, app_env.next_svc, 1);

		// Call the function used to add the required service
		appm_add_svc_func_list[app_env.next_svc]();

		// Select following service to add
		app_env.next_svc++;
		more_svc = true;
	}

	return more_svc;
}


/*设备主动断开连接函数*/
void appm_disconnect(void)
{
	struct gapc_disconnect_cmd *cmd = KE_MSG_ALLOC(GAPC_DISCONNECT_CMD,
	                                  KE_BUILD_ID(TASK_GAPC, app_env.conidx), TASK_APP,
	                                  gapc_disconnect_cmd);

	cmd->operation = GAPC_DISCONNECT;
	cmd->reason    = CO_ERROR_REMOTE_USER_TERM_CON;

	// Send the message
	ke_msg_send(cmd);
}


/* 设备发起定向广播函数*/
void appm_start_dirc_adver(void)
{
	// Check if the advertising procedure is already is progress
	if (ke_state_get(TASK_APP) == APPM_READY)
	{
		struct gapm_start_advertise_cmd *cmd = KE_MSG_ALLOC(GAPM_START_ADVERTISE_CMD,
		                                       TASK_GAPM, TASK_APP,
		                                       gapm_start_advertise_cmd);

		cmd->op.addr_src    = GAPM_STATIC_ADDR;
		cmd->channel_map    = APP_ADV_CHMAP;
		cmd->intv_min = APP_ADV_FAST_INT;
		cmd->intv_max = APP_ADV_FAST_INT;
		cmd->op.code        = GAPM_ADV_UNDIRECT;
		cmd->info.host.mode = GAP_GEN_DISCOVERABLE;

		/*
		 * If the peripheral is already bonded with a central device, use the direct advertising
		 * procedure (BD Address of the peer device is stored in NVDS.
		 */
		if(app_sec_get_bond_status())
		{
			cmd->op.code   = GAPM_ADV_DIRECT_LDC;
            uint8_t bd_len = NVDS_LEN_PEER_BD_ADDRESS;

#if (NVDS_SUPPORT)
			if(nvds_get(NVDS_TAG_PEER_BD_ADDRESS, &bd_len,
			            (uint8_t *)cmd->info.direct.addr.addr) != NVDS_OK)
			{
				// An error has occurred during access to the NVDS
				ASSERT_INFO(0,NVDS_TAG_PEER_BD_ADDRESS,bd_len);
			}
#else
			struct app_sec_tag app_nvr_tag;
			flash_read(FLASH_SPACE_TYPE_NVR, 0x8000, sizeof(app_nvr_tag), (uint8_t*)&app_nvr_tag);
			memcpy(&cmd->info.direct, &app_nvr_tag.bdaddr, bd_len);
#endif

			ke_msg_send(cmd);

#if (SYSTEM_SLEEP)
			//Start the advertising timer
			ke_timer_set(APP_ADV_TIMEOUT_TIMER, TASK_APP, APP_DFLT_ADV_DURATION);
#endif

#if !(SYSTEM_SLEEP)
			ke_msg_send_basic(APP_PERIOD_TIMER,TASK_APP,TASK_APP);
#endif

			wdt_enable(0x3fff);

			UART_PRINTF("appm start dirc adver\r\n");
		}
		else
		{
			ke_msg_free(ke_param2msg(cmd));
			appm_start_advertising();

			UART_PRINTF("appm start general advertising\r\n");
		}

		ke_state_set(TASK_APP, APPM_ADVERTISING);
	}
}


uint8_t wlist_enable_flag = 0;

/* 设备发起白名单过滤广播*/
void appm_start_white_list_dvertising(void)
{   
    struct gap_bdaddr whitelist_bdaddr;
    uint8_t peer_address_len = NVDS_LEN_PEER_BD_ADDRESS;

    // Check if the advertising procedure is already is progress
    if (ke_state_get(TASK_APP) == APPM_READY)
    {      
        if(app_sec_get_bond_status())
        {
            UART_PRINTF("appm_start_white_list_dvertising\r\n");
            if(nvds_get(NVDS_TAG_PEER_BD_ADDRESS, &peer_address_len, (uint8_t *)&whitelist_bdaddr) != NVDS_OK)
            {
                // An error has occurred during access to the NVDS
                ASSERT_INFO(0,NVDS_TAG_PEER_BD_ADDRESS,peer_address_len);
            }
            
            appm_add_dev_to_wlist(whitelist_bdaddr);
            wlist_enable_flag = 1;
            appm_start_advertising();
        }
    }		
}


/* 设备发起广播函数*/
void appm_start_advertising(void)
{
#if !(SYSTEM_SLEEP)
	ke_msg_send_basic(APP_PERIOD_TIMER,TASK_APP,TASK_APP);
#endif
	// Check if the advertising procedure is already is progress
	if (ke_state_get(TASK_APP) == APPM_READY)
	{
		// Prepare the GAPM_START_ADVERTISE_CMD message
		struct gapm_start_advertise_cmd *cmd = KE_MSG_ALLOC(GAPM_START_ADVERTISE_CMD,
		                                       TASK_GAPM, TASK_APP,
		                                       gapm_start_advertise_cmd);

		cmd->op.addr_src    = GAPM_STATIC_ADDR;
		cmd->channel_map    = APP_ADV_CHMAP;
		cmd->intv_min 		= APP_ADV_INT_MIN;
		cmd->intv_max 		= APP_ADV_INT_MAX;
		cmd->op.code        = GAPM_ADV_UNDIRECT;

		cmd->info.host.mode = GAP_GEN_DISCOVERABLE;

#if (APP_WHITE_LIST_ENABLE)
        if(wlist_enable_flag)
        {
            wlist_enable_flag=0;
            cmd->info.host.adv_filt_policy=ADV_ALLOW_SCAN_WLST_CON_WLST;
        }
#endif
		/*-----------------------------------------------------------------------------------
		 * Set the Advertising Data and the Scan Response Data
		 *---------------------------------------------------------------------------------*/
		// Flag value is set by the GAP
		cmd->info.host.adv_data_len       = 0x11;//ADV_DATA_LEN;
		//cmd->info.host.scan_rsp_data_len  = SCAN_RSP_DATA_LEN;

#if (NVDS_SUPPORT)
		// Advertising Data
		if(nvds_get(NVDS_TAG_APP_BLE_ADV_DATA, &cmd->info.host.adv_data_len,
		            &cmd->info.host.adv_data[0]) != NVDS_OK)
#endif
		{
			cmd->info.host.adv_data[0] = 0x0a;// Length of ad type flags
			cmd->info.host.adv_data[1] =0x42;// GAP_AD_TYPE_FLAGS;
			cmd->info.host.adv_data[2] = 0x88;//GAP_BR_EDR_NOT_SUPPORTED; ????
      cmd->info.host.adv_data[3] = 0x8e;
      cmd->info.host.adv_data[4] = 0x8c;
			// set mode in ad_type
//			switch(cmd->info.host.mode)
//			{
//				// General discoverable mode
//			case GAP_GEN_DISCOVERABLE:
//			{
//				cmd->info.host.adv_data[2] |= GAP_LE_GEN_DISCOVERABLE_FLG;
//			}
//			break;
//			// Limited discoverable mode
//			case GAP_LIM_DISCOVERABLE:
//			{
//				cmd->info.host.adv_data[2] |= GAP_LE_LIM_DISCOVERABLE_FLG;
//			}
//			break;
//			default:
//				break; // do nothing
//			}

		}



		// Check if data can be added to the Advertising data

#if (NVDS_SUPPORT)
		// Scan Response Data
		if(nvds_get(NVDS_TAG_APP_BLE_SCAN_RESP_DATA, &cmd->info.host.scan_rsp_data_len,
		            &cmd->info.host.scan_rsp_data[0]) != NVDS_OK)
#endif


		// Send the message
		ke_msg_send(cmd);
		UART_PRINTF("appm start adver\r\n");

#if (SYSTEM_SLEEP)
		//Start the advertising timer
		ke_timer_set(APP_ADV_TIMEOUT_TIMER, TASK_APP, APP_DFLT_ADV_DURATION);
#endif

		wdt_enable(0x3fff);

		// Set the state.
		ke_state_set(TASK_APP, APPM_ADVERTISING);
	}

	// else ignore the request
}


/* 设备主动停止广播函数*/
void appm_stop_advertising(void)
{
	if (ke_state_get(TASK_APP) == APPM_ADVERTISING)
	{
		// Stop the advertising timer if needed
		if (ke_timer_active(APP_ADV_TIMEOUT_TIMER, TASK_APP))
		{
			ke_timer_clear(APP_ADV_TIMEOUT_TIMER, TASK_APP);
		}

		// Go in ready state
		ke_state_set(TASK_APP, APPM_READY);

		// Prepare the GAPM_CANCEL_CMD message
		struct gapm_cancel_cmd *cmd = KE_MSG_ALLOC(GAPM_CANCEL_CMD,
		                              TASK_GAPM, TASK_APP,
		                              gapm_cancel_cmd);
		cmd->operation = GAPM_CANCEL;

		// Send the message
		ke_msg_send(cmd);

		wdt_disable_flag = 1;
	}
	// else ignore the request
}



void appm_update_param(struct gapc_conn_param *conn_param)
{
	// Prepare the GAPC_PARAM_UPDATE_CMD message
	struct gapc_param_update_cmd *cmd = KE_MSG_ALLOC(GAPC_PARAM_UPDATE_CMD,
	                                    KE_BUILD_ID(TASK_GAPC, app_env.conidx), TASK_APP,
	                                    gapc_param_update_cmd);

	cmd->operation  = GAPC_UPDATE_PARAMS;
	cmd->intv_min   = conn_param->intv_min;
	cmd->intv_max   = conn_param->intv_max;
	cmd->latency    = conn_param->latency;
	cmd->time_out   = conn_param->time_out;

	// not used by a slave device
	cmd->ce_len_min = 0xFFFF;
	cmd->ce_len_max = 0xFFFF;

	UART_PRINTF("intv_min = %d,intv_max = %d,latency = %d,time_out = %d\r\n",
	            cmd->intv_min,cmd->intv_max,cmd->latency,cmd->time_out);

	// Send the message
	ke_msg_send(cmd);
}


void appm_update_adv_data( uint8_t* adv_buff, uint8_t adv_len, uint8_t* scan_buff, uint8_t scan_len)
{
	if (ke_state_get(TASK_APP) == APPM_ADVERTISING 
                 && (adv_len <= ADV_DATA_LEN) && (scan_len <= ADV_DATA_LEN))
	{
		struct gapm_update_advertise_data_cmd *cmd =  KE_MSG_ALLOC(
            		            GAPM_UPDATE_ADVERTISE_DATA_CMD,
            		            TASK_GAPM,
            		            TASK_APP,
            		            gapm_update_advertise_data_cmd);

		cmd->operation = GAPM_UPDATE_ADVERTISE_DATA;
		cmd->adv_data_len = adv_len;
		cmd->scan_rsp_data_len = scan_len;

		//memcpy
		memcpy(&cmd->adv_data[0], adv_buff, adv_len);
		memcpy(&cmd->scan_rsp_data[0], scan_buff, scan_len);
        
		// Send the message
		ke_msg_send(cmd);
	}
}


uint8_t appm_get_dev_name(uint8_t* name)
{
	// copy name to provided pointer
	memcpy(name, app_env.dev_name, app_env.dev_name_len);
	// return name length
	return app_env.dev_name_len;
}


void appm_switch_adver(void)
{
	app_sec_remove_bond();
	appm_stop_advertising();
}


void appm_send_seurity_req(void)
{
	UART_PRINTF("%s \r\n",__func__);
	app_sec_send_security_req(app_env.conidx);
}

#endif //(BLE_APP_PRESENT)

/// @} APP


