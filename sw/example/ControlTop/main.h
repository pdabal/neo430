#ifndef main_h
#define main_h

#include <neo430.h>

void cmdSet(uint16_t cmd, uint32_t *value);
void cmdGet(uint16_t cmd, uint32_t *value);

void getCmdValue(char *buffer, uint32_t *value);
void setCmdValue(char *buffer, uint32_t *value, uint16_t cmd);

uint16_t crypto_setValue(uint8_t *value, uint16_t n);

uint16_t crypto_init();
uint16_t crypto_cmd(char *buffer);

#endif
