/**
 ****************************************************************************************
 *
 * @file app_sec.c
 *
 * @brief Application Security Entry Point
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

#include "rwip_config.h"


#include <string.h>
#include "co_math.h"
#include "gapc_task.h"      // GAP Controller Task API Definition
#include "gap.h"            // GAP Definition
#include "gapc.h"           // GAPC Definition
#include "prf_types.h"
#include "llc_llcp.h"

#include "app.h"            // Application API Definition
#include "app_sec.h"        // Application Security API Definition
#include "app_task.h"       // Application Manager API Definitionde 
#include "ke_timer.h"
#if (NVDS_SUPPORT)
#include "nvds.h"           // NVDS API Definitions
#else
#include "flash.h"
#endif
#include "co_utils.h"
#include "uart.h"
/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */



/// Application Security Environment Structure
struct app_sec_env_tag app_sec_env;

bool app_get_bonding_flag(void);

bool app_sec_get_enc_flag(void);




/*
 * GLOBAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */

void app_sec_init()
{
	struct app_sec_tag app_nvr_tag;
	flash_read(FLASH_SPACE_TYPE_NVR, 0x8000, sizeof(app_nvr_tag), (uint8_t*)&app_nvr_tag);
	if(app_nvr_tag.bond == 0x1)
	{
		app_sec_env.bonded = true;
	}
	else
	{
		app_sec_env.bonded = false;
	}

	app_sec_env.peer_pairing = false;
	app_sec_env.peer_encrypt = false;
	app_sec_env.bond_lost = false;
	app_sec_env.pairing_fail = false;
	UART_PRINTF("app_sec_env.bonded = %d\r\n", app_sec_env.bonded);
}

bool app_sec_get_bond_status(void)
{
	return app_sec_env.bonded;
}


bool app_get_bonding_flag(void)
{
	UART_PRINTF("peer pairing = %d\r\n",app_sec_env.peer_pairing);
	return app_sec_env.peer_pairing;
}


bool app_get_encrypt_flag(void)
{
	UART_PRINTF("peer encrypt = %d\r\n",app_sec_env.peer_encrypt);
	return app_sec_env.peer_encrypt;
}


void app_sec_remove_bond(void)
{
	struct app_sec_tag app_nvr_tag;
	
	// Check if we are well bonded
	if (app_sec_env.bonded == true)
	{
		// Update the environment variable
		app_sec_env.bonded = false;
		flash_read(FLASH_SPACE_TYPE_NVR, 0x8000, sizeof(app_nvr_tag), (uint8_t*)&app_nvr_tag);
		flash_erase_sector(FLASH_SPACE_TYPE_NVR, 0x8000);
		app_nvr_tag.bond = 0x0;
		memset(&app_nvr_tag.ltk, 0x0, sizeof(struct gapc_ltk));
		flash_write(FLASH_SPACE_TYPE_NVR, 0x8000, sizeof(app_nvr_tag), (uint8_t*)&app_nvr_tag);
	}
	app_sec_env.bonded = false;
	app_sec_env.peer_pairing = false;
	app_sec_env.peer_encrypt = false;
}

void app_sec_send_security_req(uint8_t conidx)
{
	if(!app_get_encrypt_flag() && !app_get_bonding_flag())
	{
		UART_PRINTF("app_sec_send_security_req\r\n");

		// Send security request
		struct gapc_security_cmd *cmd = KE_MSG_ALLOC(GAPC_SECURITY_CMD,
		                                KE_BUILD_ID(TASK_GAPC, conidx), TASK_APP,
		                                gapc_security_cmd);

		cmd->operation = GAPC_SECURITY_REQ;
		cmd->auth      = GAP_AUTH_REQ_NO_MITM_BOND;
		// Send the message
		ke_msg_send(cmd);
	}
	else
	{
		ke_timer_clear(APP_SEND_SECURITY_REQ,TASK_APP);
	}
}

/*
 * MESSAGE HANDLERS
 ****************************************************************************************
 */

static int gapc_bond_req_ind_handler(ke_msg_id_t const msgid,
                                     struct gapc_bond_req_ind const *param,
                                     ke_task_id_t const dest_id,
                                     ke_task_id_t const src_id)
{
	UART_PRINTF("%s \r\n",__func__);
	// Prepare the GAPC_BOND_CFM message
	struct gapc_bond_cfm *cfm = KE_MSG_ALLOC(GAPC_BOND_CFM,
	                            src_id, TASK_APP,
	                            gapc_bond_cfm);

	switch (param->request)
	{
	case (GAPC_PAIRING_REQ):
	{
		UART_PRINTF("gapc_pairing req\r\n");
		app_sec_env.peer_pairing = true;
		cfm->request = GAPC_PAIRING_RSP;
		cfm->accept  = false;

		UART_PRINTF("bond req: app_sec_env.bonded = %d\r\n", app_sec_env.bonded);

#if (!HID_CONNECT_ANY)
		// Check if we are already bonded (Only one bonded connection is supported)
		if (!app_sec_env.bonded)
#endif
		{
			cfm->accept  = true;

			// Pairing Features
			cfm->data.pairing_feat.auth      = GAP_AUTH_REQ_NO_MITM_BOND;
			cfm->data.pairing_feat.iocap     = GAP_IO_CAP_NO_INPUT_NO_OUTPUT;
			cfm->data.pairing_feat.key_size  = 16;
			cfm->data.pairing_feat.oob       = GAP_OOB_AUTH_DATA_NOT_PRESENT;
			cfm->data.pairing_feat.sec_req   = GAP_NO_SEC;
			cfm->data.pairing_feat.ikey_dist = GAP_KDIST_ENCKEY| GAP_KDIST_IDKEY;
			cfm->data.pairing_feat.rkey_dist = GAP_KDIST_ENCKEY| GAP_KDIST_IDKEY;
		}
	}
	break;

	case (GAPC_LTK_EXCH):
	{
		UART_PRINTF("gapc_ltk req\r\n");
		// Counter
		uint8_t counter;
		struct app_sec_tag app_nvr_tag;
		
		cfm->accept  = true;
		cfm->request = GAPC_LTK_EXCH;

#if (!HID_CONNECT_ANY)
		// Generate all the values
		cfm->data.ltk.ediv = (uint16_t)co_rand_word();

		for (counter = 0; counter < RAND_NB_LEN; counter++)
		{
			cfm->data.ltk.ltk.key[counter]    = (uint8_t)co_rand_word();
			cfm->data.ltk.randnb.nb[counter] = (uint8_t)co_rand_word();
		}

		for (counter = RAND_NB_LEN; counter < KEY_LEN; counter++)
		{
			cfm->data.ltk.ltk.key[counter]    = (uint8_t)co_rand_word();
		}
#else
		// Generate all the values
		cfm->data.ltk.ediv = 0x1234;

		for (counter = 0; counter < RAND_NB_LEN; counter++)
		{
			cfm->data.ltk.ltk.key[counter]    = 0x11;
			cfm->data.ltk.randnb.nb[counter] = 0x11;
		}

		for (counter = RAND_NB_LEN; counter < KEY_LEN; counter++)
		{
			cfm->data.ltk.ltk.key[counter]    = 0x11;
		}
#endif
		flash_read(FLASH_SPACE_TYPE_NVR, 0x8000, sizeof(app_nvr_tag), (uint8_t*)&app_nvr_tag);
		flash_erase_sector(FLASH_SPACE_TYPE_NVR, 0x8000);
		memset(&app_nvr_tag.ltk, 0x0, sizeof( struct gapc_ltk));
		memcpy(&app_nvr_tag.ltk,(uint8_t *)&cfm->data.ltk,  sizeof( struct gapc_ltk));
		flash_write(FLASH_SPACE_TYPE_NVR, 0x8000, sizeof(app_nvr_tag), (uint8_t*)&app_nvr_tag);

	}
	break;


	case (GAPC_IRK_EXCH):
	{
		UART_PRINTF("gapc_lrk exch\r\n");

		cfm->accept  = true;
		cfm->request = GAPC_IRK_EXCH;

		// Load IRK
		memcpy(cfm->data.irk.irk.key, app_env.loc_irk, KEY_LEN);
		// load device address
		cfm->data.irk.addr.addr_type = ADDR_PUBLIC;
		memcpy(cfm->data.irk.addr.addr.addr,(uint8_t *)&co_default_bdaddr,BD_ADDR_LEN);
	}
	break;

	case (GAPC_TK_EXCH):
	{
		// Generate a PIN Code- (Between 100000 and 999999)
		uint32_t pin_code =123456;// (100000 + (co_rand_word()%900000));

		cfm->accept  = true;
		cfm->request = GAPC_TK_EXCH;

		// Set the TK value
		memset(cfm->data.tk.key, 0, KEY_LEN);

		cfm->data.tk.key[0] = (uint8_t)((pin_code & 0x000000FF) >>  0);
		cfm->data.tk.key[1] = (uint8_t)((pin_code & 0x0000FF00) >>  8);
		cfm->data.tk.key[2] = (uint8_t)((pin_code & 0x00FF0000) >> 16);
		cfm->data.tk.key[3] = (uint8_t)((pin_code & 0xFF000000) >> 24);
	}
	break;

	default:
	{
		ASSERT_ERR(0);
	}
	break;
	}

	// Send the message
	ke_msg_send(cfm);

	return (KE_MSG_CONSUMED);
}


static int gapc_bond_ind_handler(ke_msg_id_t const msgid,
                                 struct gapc_bond_ind const *param,
                                 ke_task_id_t const dest_id,
                                 ke_task_id_t const src_id)
{
	UART_PRINTF("%s param->info = 0x%x\r\n",__func__,param->info);
	struct app_sec_tag app_nvr_tag;
	
	switch (param->info)
	{
	case (GAPC_PAIRING_SUCCEED):
	{
		// Update the bonding status in the environment
		app_sec_env.bond_lost = false;
		app_sec_env.pairing_fail = false;
#if (!HID_CONNECT_ANY)
		app_sec_env.bonded = true;
#else
		app_sec_env.bonded = false;
#endif
		UART_PRINTF("gapc pairing success\r\n");

		flash_read(FLASH_SPACE_TYPE_NVR, 0x8000, sizeof(app_nvr_tag), (uint8_t*)&app_nvr_tag);
		flash_erase_sector(FLASH_SPACE_TYPE_NVR, 0x8000);
		app_nvr_tag.bond = app_sec_env.bonded;
		memset(&app_nvr_tag.bdaddr,0x0, sizeof(struct gap_bdaddr));
		memcpy(&app_nvr_tag.bdaddr, gapc_get_bdaddr(0, SMPC_INFO_PEER), sizeof(struct gap_bdaddr));
		flash_write(FLASH_SPACE_TYPE_NVR, 0x8000, sizeof(app_nvr_tag), (uint8_t*)&app_nvr_tag);
	}
	break;

	case (GAPC_REPEATED_ATTEMPT):
	{
		app_sec_env.peer_pairing = false;
		app_sec_env.peer_encrypt = false;
		app_sec_env.pairing_fail = true;
		appm_disconnect();
	}
	break;

	case (GAPC_IRK_EXCH):
	{
		UART_PRINTF("gapc pairing GAPC_IRK_EXCH\r\n");

		flash_read(FLASH_SPACE_TYPE_NVR, 0x8000, sizeof(app_nvr_tag), (uint8_t*)&app_nvr_tag);
		flash_erase_sector(FLASH_SPACE_TYPE_NVR, 0x8000);
		app_nvr_tag.bond = 0x0;
		memset(&app_nvr_tag.irk,0x0, sizeof(struct gapc_irk));
		memcpy(&app_nvr_tag.irk,  (uint8_t *)&param->data.irk, sizeof(struct gapc_irk));
		flash_write(FLASH_SPACE_TYPE_NVR, 0x8000, sizeof(app_nvr_tag), (uint8_t*)&app_nvr_tag);

		memcpy(&app_env.peer_irk,&param->data.irk.irk.key[0],sizeof(struct gapc_irk));
		UART_PRINTF("irk.key = ");
		for(int i = 0; i<sizeof(struct gap_sec_key); i++)
		{
			UART_PRINTF("0x%x ",param->data.irk.irk.key[i]);
		}
		UART_PRINTF("\r\n");

		UART_PRINTF("addr.type = %x\r\n",param->data.irk.addr.addr_type);
		UART_PRINTF("addr.addr = ");
		for(int i = 0; i<sizeof(struct bd_addr); i++)
		{
			UART_PRINTF("0x%x ",param->data.irk.addr.addr.addr[i]);
		}
		UART_PRINTF("\r\n");
	}
	break;

	case (GAPC_PAIRING_FAILED):
	{
		UART_PRINTF("gapc pairing failed\r\n");
		app_sec_env.peer_pairing = false;
		app_sec_env.peer_encrypt = false;
		app_sec_env.pairing_fail = true;
		appm_disconnect();
	}
	break;

	default:
	{
		ASSERT_ERR(0);
	}
	break;
	}

	return (KE_MSG_CONSUMED);
}

static int gapc_encrypt_req_ind_handler(ke_msg_id_t const msgid,
                                        struct gapc_encrypt_req_ind const *param,
                                        ke_task_id_t const dest_id,
                                        ke_task_id_t const src_id)
{
	struct app_sec_tag app_nvr_tag;
	if(ke_timer_active(APP_SEND_SECURITY_REQ, TASK_APP))
	{
		ke_timer_clear(APP_SEND_SECURITY_REQ, TASK_APP);
	}

	UART_PRINTF("%s \r\n",__func__);
	app_sec_env.peer_encrypt = true;

	// Prepare the GAPC_ENCRYPT_CFM message
	struct gapc_encrypt_cfm *cfm = KE_MSG_ALLOC(GAPC_ENCRYPT_CFM,
	                               src_id, TASK_APP,
	                               gapc_encrypt_cfm);

	cfm->found    = false;

	if(1)
	{
		flash_read(FLASH_SPACE_TYPE_NVR, 0x8000, sizeof(app_nvr_tag), (uint8_t*)&app_nvr_tag);

		// Check if the provided EDIV and Rand Nb values match with the stored values
		if ((param->ediv == app_nvr_tag.ltk.ediv) &&
		        !memcmp(&param->rand_nb.nb[0], (void*)app_nvr_tag.ltk.randnb.nb, sizeof(struct rand_nb)))
		{
			cfm->found    = true;
			cfm->key_size = 16;
			memcpy(&cfm->ltk, &app_nvr_tag.ltk.ltk, sizeof(struct gap_sec_key));
		}
		else
		{
			UART_PRINTF("we are bonded with another device, disconnect the link\r\n");
			app_sec_env.bond_lost = true;
			appm_disconnect();
		}
	}
	else
	{
		UART_PRINTF("bond lost,disconnect link\r\n");
		app_sec_env.bond_lost = true;
		appm_disconnect();
	}
	/*
	 * else the peer device is not known, an error should trigger a new pairing procedure.
	 */

	// Send the message
	ke_msg_send(cfm);

	return (KE_MSG_CONSUMED);
}


static int gapc_encrypt_ind_handler(ke_msg_id_t const msgid,
                                    struct gapc_encrypt_ind const *param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id)
{
	// encryption/ re-encryption succeeded
	UART_PRINTF("%s \r\n",__func__);

	return (KE_MSG_CONSUMED);
}

static int app_sec_msg_dflt_handler(ke_msg_id_t const msgid,
                                    void *param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id)
{
	// Drop the message

	return (KE_MSG_CONSUMED);
}

/*
 * LOCAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/// Default State handlers definition
const struct ke_msg_handler app_sec_msg_handler_list[] =
{
	// Note: first message is latest message checked by kernel so default is put on top.
	{KE_MSG_DEFAULT_HANDLER,  (ke_msg_func_t)app_sec_msg_dflt_handler},
	{GAPC_BOND_REQ_IND,       (ke_msg_func_t)gapc_bond_req_ind_handler},
	{GAPC_BOND_IND,           (ke_msg_func_t)gapc_bond_ind_handler},
	{GAPC_ENCRYPT_REQ_IND,    (ke_msg_func_t)gapc_encrypt_req_ind_handler},
	{GAPC_ENCRYPT_IND,        (ke_msg_func_t)gapc_encrypt_ind_handler},
};

const struct ke_state_handler app_sec_table_handler =
{&app_sec_msg_handler_list[0], (sizeof(app_sec_msg_handler_list)/sizeof(struct ke_msg_handler))};


/// @} APP
