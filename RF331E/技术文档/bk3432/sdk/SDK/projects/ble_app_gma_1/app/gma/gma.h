
#ifndef __GMA_H__
#define __GMA_H__

#include "gma_include.h"

#if GMA_SUPPORT
typedef enum{
    START_MIC = 0,
    STOP_MIC,
}mic_status_e;

#define GMA_PAYLOAD_HEAD    4
#define GMA_VOICE_HEAD      5

// GMA result
#define SUCCESS     0
#define FAIL        1

// GMA confirm net result
#define BOND        1
#define DEBOND      0

// GMA voice parameter
#define  VOICE_SEND_MAX     50
#define  MIC_NUM            1
#define  REF_NUM            0

typedef enum
{
    // GMA general command type
    CMD_DEV_REPORT              = 0x01,
    CMD_APP_REQ                 = 0x02,
    CMD_APP_RSP                 = 0x03,
    CMD_DEV_REQ                 = 0x04,
    CMD_DEV_RSP                 = 0x05,
    CMD_APP_CMD                 = 0x06,
    CMD_APP_MANU_REQ            = 0x07,
    CMD_APP_MANU_RSP            = 0x08,
    CMD_DEV_ERROR               = 0x0F,

    // GMA authentication command type
    CMD_AUTH_START_REQ          = 0x10,         // app->device
    CMD_AUTH_START_RSP          = 0x11,         // device->app
    CMD_AUTH_RESULT_REQ         = 0x12,         // app->device
    CMD_AUTH_RESULT_RSP         = 0x13,         // device->app
    CMD_AUTH_CONFIRM_NET_REQ    = 0x14,         // app->device
    CMD_AUTH_CONFIRM_NET_RSP    = 0x15,         // device->app

    // GMA ota command type
    CMD_OTA_FW_VER_REQ          = 0x20,         // app->device
    CMD_OTA_FW_VER_RSP          = 0x21,         // device->app
    CMD_OTA_START_REQ           = 0x22,         // app->device
    CMD_OTA_START_RSP           = 0x23,         // device->app
    CMD_OTA_FRAME_LEN_RPT       = 0x24,         // device->app
    CMD_OTA_END_CHECK_REQ       = 0x25,         // app->device
    CMD_OTA_END_CHECK_RSP       = 0x26,         // device->app
    CMD_OTA_PDU                 = 0x2F,         // app->device

    // GMA voice command type
    CMD_MIC_VOICE_SEND          = 0x30,         // device->app
    CMD_DEV_INFO_REQ            = 0x32,         // app->device
    CMD_DEV_INFO_RSP            = 0x33,         // device->app
    CMD_ACTIVE_REQ              = 0x34,         // app->device
    CMD_ACTIVE_RSP              = 0x35,         // device->app
    CMD_STATUS_SET_REQ          = 0x36,         // app->device
    CMD_STATUS_SET_RSP          = 0x37,         // device->app
    CMD_A2DP_SET_REQ            = 0x38,         // app->device
    CMD_A2DP_SET_RSP            = 0x39,         // device->app
    CMD_HFP_SET_REQ             = 0x3A,         // app->device
    CMD_HFP_SET_RSP             = 0x3B,         // device->app
    CMD_MIC_READY_REQ           = 0x3E,         // device->app
    CMD_MIC_READY_RSP           = 0x3F,         // app->device
    CMD_DEV_PARAM_REQ           = 0x40,         // app->device
    CMD_DEV_PARAM_RSP           = 0x41,         // device->app
}GMA_CMD_E;

typedef enum
{
    CONNCT_READY = 0,
    MIC_START,
    MIC_STOP,
    PLAY_TTS,
    STOP_TTS,
    RECORD_START,
    RECORD_STOP,
}MIC_STATUS_E;

typedef enum
{
    A2DP_STATUS = 0,
    A2DP_VOICE
}A2DP_STATUS_E;

typedef enum
{
    HFP_STATUS = 0,
    HFP_TEL_NUM,
    HFP_HANDLE,
    HFP_REFUSE,
}HFP_STATUS_E;

typedef enum
{
    BATTARY_LEVEL = 0,
    BATTARY_STATUS,
    SET_FM_FREQUENCY,
    GET_FM_FREQUENCY,
    FIRMWARE_VERSION,
    DEV_NAME,
    MIC_STATUS
}DEV_PARAM_E;

typedef struct
{
    uint8_t next_msgid:4;
    uint8_t init_flag:1;
    uint8_t auth_flag:1;
    uint8_t speech_flag:1;
    uint8_t :1;
    uint8_t remote_random[16];
    uint8_t ble_key[16];
    uint8_t ble_mac[6];
    uint8_t dev_random[16];
}__attribute__((packed))gma_para_proc_s;

typedef struct
{
	uint8_t msg_id:4;
	uint8_t save_flag:1;
	uint8_t version:3;
	uint8_t cmd;
	uint8_t fn:4;
	uint8_t total_fn:4;
	uint8_t f_len;
}__attribute__((packed))gma_frame_head_s;

typedef struct
{
	gma_frame_head_s gma_frame_head;
	uint8_t data[1];
}__attribute__((packed))gma_recv_data_s;

typedef struct
{
    uint8_t mac[6];
    uint32_t pid;
    uint8_t secret[16];
}__attribute__((packed))ali_para_s;

typedef struct
{
    uint8_t mobile_type;
    uint16_t gma_ver;
}__attribute__((packed))gma_mobile_info_s;

typedef struct
{
    uint16_t ability;
    uint8_t audio_coding;
    uint16_t gma_ver;
    uint8_t classic_mac[6];
}__attribute__((packed))gma_dev_info_s;

typedef struct
{
    uint8_t type;
    uint8_t len;
    uint8_t value;
}__attribute__((packed))gma_tlv_s;

typedef struct
{
    uint8_t random[16];
    uint8_t digest[16];
}__attribute__((packed))gma_active_s;

typedef struct
{
	// Secure OAD uses the Signature for image validation instead of calculating a CRC, but the use
	// of CRC==CRC-Shadow for quick boot-up determination of a validated image is still used.
	uint32_t crc;       // CRC must not be 0x0000 or 0xFFFF.
	// User-defined Image Version Number - default logic uses simple a '!=' comparison to start an OAD.
	uint16_t ver;
	uint16_t len;        // Image length in 4-byte blocks (i.e. HAL_FLASH_WORD_SIZE blocks).
	uint32_t  uid;     // User-defined Image Identification bytes.
	uint8_t  crc_status;     // cur image crc status
	uint8_t  sec_status;     // cur image sec status
	uint16_t  rom_ver;     // Reserved space for future use.
} img_hdr_t;

// GMA SPP_128bit UUID
#define GMA_SPP_UUID                       {0xEB,0x3E,0x0A,0xF3,0x57,0xF4,0x47,0x89,0xAB,0x55,0x86,0x50,0x85,0x80,0x29,0x6A}

// GMA attribute handle
#define GMA_READ_HANDLE                     0x1002
#define GMA_WRITE_HANDLE                    0x1004
#define GMA_INDICATE_HANDLE                 0x1006
#define GMA_WRITEWITHNORESP_HANDLE          0x1009
#define GMA_NOTIFY_HANDLE                   0x100B

#define SEC_IMAGE_OAD_HEADER_APP_FADDR   (0x10410)
#define SEC_BACKUP_OAD_HEADER_FADDR		 (0x1C000) //116kb * 1024




void gma_init(void);
void gma_flag_clear(void);
void gma_recv_decode(uint8_t *buf, uint16_t len);
void gma_send_dev_report(uint8_t msg_id, uint8_t *data, uint8_t len);
void gma_send_dev_req(uint8_t msg_id, uint8_t *data, uint8_t len);
void gma_send_dev_error(uint8_t msg_id, uint8_t *data, uint8_t len);
#if GMA_OTA
void gma_ota_write_flash(void);
uint8_t gma_ota_is_ongoing(void);
#endif
#endif

#endif /* __GMA_H__ */
