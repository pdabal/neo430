#ifndef main_h
#define main_h

#include <neo430.h>

void cmdSet(uint16_t cmd);
void cmdGet(uint16_t cmd);

uint16_t getCmdValue(char *buffer, uint16_t *value);
void setCmdValue(char *buffer, uint16_t *value, uint16_t cmd);

uint16_t crypto_setValue(uint8_t *value, uint16_t n);

void crypto_init();
void crypto_cmd(char *buffer);

void clear(uint16_t *value, uint16_t n);

#endif
