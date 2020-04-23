#include "wb_io.h"

void wb_io_led_set(uint16_t value)
{
    neo430_wishbone16_write(WB_IO_BASEADDRES + WB_IO_REG_LED, value);
}

uint16_t wb_io_led_get()
{
    uint16_t value = neo430_wishbone16_read(WB_IO_BASEADDRES + WB_IO_REG_LED);
    return value;
}

uint16_t wb_io_dipsw_get()
{
    return neo430_wishbone16_read(WB_IO_BASEADDRES + WB_IO_REG_DIPSW);
}

uint16_t wb_io_header_i_get()
{
    return neo430_wishbone16_read(WB_IO_BASEADDRES + WB_IO_REG_HEADER_IN);
}

uint16_t wb_io_header_o_get()
{
    return neo430_wishbone16_read(WB_IO_BASEADDRES + WB_IO_REG_HEADER_OUT);
}

uint16_t wb_io_header_dir_get()
{
    return neo430_wishbone16_read(WB_IO_BASEADDRES + WB_IO_REG_HEADER_T);
}

void wb_io_header_dir_set(uint16_t dir)
{
    neo430_wishbone16_write(WB_IO_BASEADDRES + WB_IO_REG_HEADER_T, dir);
}

void wb_io_header_o_set(uint16_t value)
{
    neo430_wishbone16_write(WB_IO_BASEADDRES + WB_IO_REG_HEADER_OUT, value);
}