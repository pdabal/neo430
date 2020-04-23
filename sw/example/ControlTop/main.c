// #################################################################################################
// #  < Blinking LED example program >                                                             #
// # ********************************************************************************************* #
// #  Displays an 8-bit counter on the high-active LEDs connected to the parallel output port.     #
// # ********************************************************************************************* #
// # This file is part of the NEO430 Processor project: https://github.com/stnolting/neo430        #
// # Copyright by Stephan Nolting: stnolting@gmail.com                                             #
// #                                                                                               #
// # This source file may be used and distributed without restriction provided that this copyright #
// # statement is not removed from the file and that any derivative work contains the original     #
// # copyright notice and the associated disclaimer.                                               #
// #                                                                                               #
// # This source file is free software; you can redistribute it and/or modify it under the terms   #
// # of the GNU Lesser General Public License as published by the Free Software Foundation,        #
// # either version 3 of the License, or (at your option) any later version.                       #
// #                                                                                               #
// # This source is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;      #
// # without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.     #
// # See the GNU Lesser General Public License for more details.                                   #
// #                                                                                               #
// # You should have received a copy of the GNU Lesser General Public License along with this      #
// # source; if not, download it from https://www.gnu.org/licenses/lgpl-3.0.en.html                #
// # ********************************************************************************************* #
// # Stephan Nolting, Hannover, Germany                                                 17.11.2018 #
// #################################################################################################

// Libraries
#include <stdio.h>
#include <stdint.h>
#include <string.h>
// Biblioteka NEO430
#include <neo430.h>
// Biblioteka modułów WB
#include "wb_io.h"
#include "wb_crypto.h"

#include "main.h"

// Parametry interpretera wiersza poleceń
#define CMD_NUM 9
#define CMD_LENGTH 5
#define CMD_VALUE_LENGTH 1

// Parametry interfejsu szeregowego
#define BAUD_RATE 19200  // Prędkość transmisji USART
#define BUFFER_LENGTH 70 // Długość bufora dla USART

// Makra wiersza poleceń
#define CMD buffer
#define CMD_TYPE buffer[CMD_LENGTH - 1]
#define CMD_VALUE &buffer[CMD_LENGTH + 2]

// Zmienne globalne
char buffer[BUFFER_LENGTH]; // Bufor dla UART
char *cmdList[CMD_NUM] = {  // Zbiór rozpoznawanych komend
    ":STA", ":RST", ":CTR", ":VAL",
    ":RES", ":ACT", ":TRI",
    ":LED", ":DIP"};

/* ------------------------------------------------------------
 * INFO Main function
 * ------------------------------------------------------------ */
int main(void)
{
  uint16_t length;

  memset(buffer, 0, BUFFER_LENGTH); // Zerowanie bufora odczytu

  // Uruchomienie UART
  neo430_uart_setup(BAUD_RATE);
  neo430_uart_br_print("\nCtrlM - hello world\n");

  // Inicjalizacja modułu CRYPTO
  uint16_t crypto_init();

  while (1)
  {
    neo430_uart_br_print("\nCONTROL_DUT:> ");            // Znak zachęty dla komend - gotowość do pracy
    length = neo430_uart_scan(buffer, BUFFER_LENGTH, 0); // Wczytanie komendy
    if (length < CMD_LENGTH)                             // Długość 0 oznacza ponowne wczytanie komendy po 10 ms
    {
      neo430_uart_br_print("\n\tCommand to short");
      continue;
    }
    else // Próba wykonania komendy
    {
      crypto_cmd(buffer);           // Wykonanie komendy
      neo430_uart_br_print(buffer); // Wyświetlenie komunikatu zwrotnego z funkcji crypto_cmd
    }
  }
  return 0;
}

//-----------------------------------------------------------------------------
uint16_t crypto_init()
{
  uint16_t result = 0, value = WB_CRYPTO_DEFAULT_INIT_VAL;
  wb_crypto_reset();
  wb_crypto_ctrl_set(0x0000);
  wb_crypto_value_set(&value);
  return result;
}

// Przetwarzanie odebranych komend
uint16_t crypto_cmd(char *buffer)
{
  uint16_t result = 0;
  uint16_t cmd = 0;
  uint32_t value[CMD_VALUE_LENGTH];

  // Wyczyszczenie pojemnika na wartości do wpisu odczytu
  memset(value, 0, CMD_VALUE_LENGTH * sizeof(uint32_t));
  // Zmiana wszystkich znaków na duże litery
  CMD = strupr(buffer);
  // Wykrycie przesłanej komendy
  for (uint16_t i = 0; i < CMD_NUM; i++)
  {
    if (!strncmp(CMD, cmdList[i], CMD_LENGTH - 1))
    {
      cmd = i + 1;
      break;
    }
  }
  // Sprawdzenie typu komendy - set/get
  switch (CMD_TYPE)
  {
  case '=':                        // Ustawienie wartości
    getCmdValue(CMD_VALUE, value); // Odczytanie przesłanej wartości z komendą =0x...
    cmdSet(cmd, value);            // Wykonanie komendy
    sprintf(buffer, "\nOK\n");
    break;
  case '?': // Odczytanie wartości
    memset(value, 0, CMD_VALUE_LENGTH * sizeof(uint32_t));
    cmdGet(cmd, value); // Wykonanie komendy
    setCmdValue(buffer, value, cmd);
    break;
  default:
    sprintf(buffer, "Nieprawidłowy parametr komendy");
    result = 1;
    break;
  }
  return result;
}

void cmdGet(uint16_t cmd, uint32_t *value)
{
  switch (cmd)
  {
  case 1:
    *value = (uint32_t)wb_crypto_start_get();
    break;
  case 2:
    *value = 0xCAFFE;
    break;
  case 3:
    *value = (uint32_t)wb_crypto_ctrl_get();
    break;
  case 4:
    wb_crypto_value_get((uint16_t *)value);
    break;
  case 5:
    wb_crypto_result_get((uint16_t *)value);
    break;
  case 6:
    *value = (uint32_t)wb_crypto_status_active_get();
    break;
  case 7:
    *value = (uint32_t)wb_crypto_status_trig_get();
    break;
  case 8:
    *value = (uint32_t)wb_io_led_get();
    break;
  case 9:
    *value = (uint32_t)wb_io_dipsw_get();
    break;
  default:
    break;
  }
}

void cmdSet(uint16_t cmd, uint32_t *value)
{
  switch (cmd)
  {
  case 1:
    wb_crypto_start_set((uint16_t)*value);
    break;
  case 2:
    wb_crypto_reset();
    break;
  case 3:
    wb_crypto_ctrl_set((uint16_t)*value);
    break;
  case 4:
    wb_crypto_value_set((uint16_t *)value);
    break;
  case 8:
    wb_io_led_set((uint16_t)*value);
    break;
  default:
    break;
  }
}

// Odczytanie wartości z bufora do tablicy value
void getCmdValue(char *buffer, uint32_t *value)
{
  *value = neo430_hexstr_to_uint(buffer, strlen(buffer));
}

void setCmdValue(char *buffer, uint32_t *value, uint16_t cmd)
{
  sprintf(buffer, "\n%s = 0x%lx\n", cmdList[cmd - 1], *value);
}