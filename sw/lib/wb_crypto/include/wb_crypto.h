#ifndef wb_crypto_h
#define wb_crypto_h

#include <neo430.h>

//----------------------------------------
#define WB_CRYPTO_BASEADDRES 0x00000000

// Reg addres
#define WB_CRYPTO_REG_VALUE 0x00000000
#define WB_CRYPTO_REG_CTRL 0x00000004
#define WB_CRYPTO_REG_RESULT 0x0000008
#define WB_CRYPTO_REG_STATUS 0x00000010

#define WB_CRYPTO_REG_HEADER_OUT 0x00000040
#define WB_CRYPTO_REG_HEADER_DIR 0x00000044
#define WB_CRYPTO_REG_HEADER_IN 0x00000048

// Reg bits
#define WB_CRYPTO_REG_CTRL_START_BIT 2
#define WB_CRYPTO_REG_CTRL_RESET_BIT 3

#define WB_CRYPTO_REG_STATUS_ACTIVE_BIT 0
#define WB_CRYPTO_REG_STATUS_TRIG_BIT 1

#define WB_CRYPTO_REG_CTRL_MASK 0x0003
#define WB_CRYPTO_REG_CTRL_START_MASK 0x0004
#define WB_CRYPTO_REG_CTRL_RESET_MASK 0x0008

#define WB_CRYPTO_REG_STATUS_ACTIVE_MASK 0x0001
#define WB_CRYPTO_REG_STATUS_TRIG_MASK 0x0002

#define WB_CRYPTO_RESET_DELAY_MS 100

#define WB_CRYPTO_DEFAULT_INIT_VAL 7

void wb_crypto_start_set(uint16_t value);
uint16_t wb_crypto_start_get();

void wb_crypto_reset();

void wb_crypto_ctrl_set(uint16_t value);
uint16_t wb_crypto_ctrl_get();

void wb_crypto_value_set(uint16_t *value);
void wb_crypto_value_get(uint16_t *value);

uint16_t wb_crypto_status_get();
uint16_t wb_crypto_status_active_get();
uint16_t wb_crypto_status_trig_get();

void wb_crypto_result_get(uint16_t *value);

#endif