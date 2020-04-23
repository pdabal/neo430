#include "wb_crypto.h"

void wb_crypto_start_set(uint16_t value)
{
    uint16_t ctrl = neo430_wishbone16_read(WB_CRYPTO_BASEADDRES + WB_CRYPTO_REG_CTRL);
    if (value == 0)
    {
        neo430_wishbone16_write(WB_CRYPTO_BASEADDRES + WB_CRYPTO_REG_CTRL, (ctrl & (!WB_CRYPTO_REG_CTRL_START_MASK)));
    }
    else
    {
        neo430_wishbone16_write(WB_CRYPTO_BASEADDRES + WB_CRYPTO_REG_CTRL, (ctrl | (1 << WB_CRYPTO_REG_CTRL_START_BIT)));
    }
}
uint16_t wb_crypto_start_get()
{
    return ((neo430_wishbone16_read(WB_CRYPTO_BASEADDRES + WB_CRYPTO_REG_CTRL) & WB_CRYPTO_REG_CTRL_START_MASK) >> WB_CRYPTO_REG_CTRL_START_BIT);
}

void wb_crypto_reset()
{
    uint16_t ctrl;
    ctrl = neo430_wishbone16_read(WB_CRYPTO_BASEADDRES + WB_CRYPTO_REG_CTRL);
    neo430_wishbone16_write(WB_CRYPTO_BASEADDRES + WB_CRYPTO_REG_CTRL, (ctrl | (1 << WB_CRYPTO_REG_CTRL_RESET_BIT)));
    neo430_cpu_delay_ms(WB_CRYPTO_RESET_DELAY_MS);
    ctrl = neo430_wishbone16_read(WB_CRYPTO_BASEADDRES + WB_CRYPTO_REG_CTRL);
    neo430_wishbone16_write(WB_CRYPTO_BASEADDRES + WB_CRYPTO_REG_CTRL, (ctrl & (!WB_CRYPTO_REG_CTRL_RESET_MASK)));
}

void wb_crypto_ctrl_set(uint16_t value)
{
    uint16_t ctrl;
    ctrl = neo430_wishbone16_read(WB_CRYPTO_BASEADDRES + WB_CRYPTO_REG_CTRL);
    neo430_wishbone16_write(WB_CRYPTO_BASEADDRES + WB_CRYPTO_REG_CTRL, (ctrl | (value & WB_CRYPTO_REG_CTRL_MASK)));
}
uint16_t wb_crypto_ctrl_get()
{
    return neo430_wishbone16_read(WB_CRYPTO_BASEADDRES + WB_CRYPTO_REG_CTRL) & WB_CRYPTO_REG_CTRL_MASK;
}

void wb_crypto_value_set(uint16_t *value)
{
    neo430_wishbone16_write(WB_CRYPTO_BASEADDRES + WB_CRYPTO_REG_VALUE, *value);
}
void wb_crypto_value_get(uint16_t *value)
{
    *value = neo430_wishbone16_read(WB_CRYPTO_BASEADDRES + WB_CRYPTO_REG_VALUE);
}

uint16_t wb_crypto_status_get()
{
    return neo430_wishbone16_read(WB_CRYPTO_BASEADDRES + WB_CRYPTO_REG_STATUS);
}
uint16_t wb_crypto_status_active_get()
{
    return ((neo430_wishbone16_read(WB_CRYPTO_BASEADDRES + WB_CRYPTO_REG_STATUS) & WB_CRYPTO_REG_STATUS_ACTIVE_MASK) >> WB_CRYPTO_REG_STATUS_ACTIVE_BIT);
}
uint16_t wb_crypto_status_trig_get()
{
    return ((neo430_wishbone16_read(WB_CRYPTO_BASEADDRES + WB_CRYPTO_REG_STATUS) & WB_CRYPTO_REG_STATUS_TRIG_MASK) >> WB_CRYPTO_REG_STATUS_TRIG_BIT);
}

void wb_crypto_result_get(uint16_t *value)
{
    *value = neo430_wishbone16_read(WB_CRYPTO_BASEADDRES + WB_CRYPTO_REG_RESULT);
}
