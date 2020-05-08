#ifndef wb_crypto_h
#define wb_crypto_h

#include <neo430.h>

//----------------------------------------
#define DUT_VALUE_LENGTH 256
#define DUT_RESULT_LENGTH 16

#define WB_ADDR_LENGTH 8

#define WB_CRYPTO_REG_VALUE_COUNT ((DUT_VALUE_LENGTH / 16) != 0) ? (DUT_VALUE_LENGTH / 16) : 1
#define WB_CRYPTO_REG_RESULT_COUNT ((DUT_RESULT_LENGTH / 16) != 0) ? (DUT_RESULT_LENGTH / 16) : 1

//-------------------------------------------------------------------------------------------------
// Regs
#define WB_CRYPTO_BASEADDRES 0x00000000
// For READ/WRITE
#define WB_CRYPTO_REG_CTRL (WB_CRYPTO_BASEADDRES + 0x00000000)
#define WB_CRYPTO_REG_VALUE (WB_CRYPTO_REG_CTRL + 0x00000004)
#define WB_CRYPTO_REG_VALUE_END (WB_CRYPTO_REG_VALUE_COUNT * 0x00000004)
// For READ
#define WB_CRYPTO_REG_STATUS (WB_CRYPTO_REG_VALUE_END + 0x00000004)
#define WB_CRYPTO_REG_RESULT (WB_CRYPTO_REG_STATUS + 0x00000004)

//-------------------------------------------------------------------------------------------------
// Regs bits
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

void wb_crypto_value_set(uint16_t *value, uint16_t n);
void wb_crypto_value_get(uint16_t *value, uint16_t n);

uint16_t wb_crypto_status_get();
uint16_t wb_crypto_status_active_get();
uint16_t wb_crypto_status_trig_get();

void wb_crypto_result_get(uint16_t *value);

//----------------------------------------
struct wb_crypto_reg_t
{
    volatile uint32_t ctrl;
    volatile uint32_t value[WB_CRYPTO_REG_VALUE_COUNT];
    volatile uint32_t status;
    volatile uint32_t result[WB_CRYPTO_REG_RESULT_COUNT];
};
#define WB_CRYPTO ((wb_crypto_reg_t *)WB_CRYPTO_BASEADDRES)
#endif