#ifndef wb_io_h
#define wb_io_h

#include <neo430.h>

#define WB_IO_BASEADDRES 0x00000040
#define WB_IO_REG_HEADER_OUT 0x00000000
#define WB_IO_REG_HEADER_T 0x00000004
#define WB_IO_REG_LED 0x00000008
#define WB_IO_REG_HEADER_IN 0x0000000C
#define WB_IO_REG_DIPSW 0x00000010

void wb_io_led_set(uint16_t value);
uint16_t wb_io_led_get();

uint16_t wb_io_dipsw_get();

uint16_t wb_io_header_i_get();
uint16_t wb_io_header_o_get();
void wb_io_header_o_set(uint16_t value);

void wb_io_header_dir_set(uint16_t value);
uint16_t wb_io_header_dir_get();

#endif