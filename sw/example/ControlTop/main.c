// Biblioteki standardowe
#include <stdio.h>
#include <stdint.h>
#include <string.h>
// Biblioteka NEO430
#include <neo430.h>
// Biblioteka modułów WB
#include "wb_io.h"
#include "wb_crypto.h"
//
#include "main.h"

// Stałe interpretera wiersza poleceń
#define CMD_NUM 12
#define CMD_LENGTH 5
#define CMD_VALUE_LENGTH WB_CRYPTO_REG_VALUE_COUNT
#define CMD_RESULT_LENGTH WB_CRYPTO_REG_RESULT_COUNT

// Stałe interfejsu szeregowego
#define BAUD_RATE 19200   // Prędkość transmisji USART
#define BUFFER_LENGTH 128 // Długość bufora dla USART

// Makra wiersza poleceń
#define CMD buffer
#define CMD_TYPE buffer[CMD_LENGTH - 1]
#define CMD_VALUE &buffer[CMD_LENGTH + 2]

// Zmienne globalne
uint16_t value[CMD_VALUE_LENGTH]; // Bufor dla wartości VALUE/RESULT
char buffer[BUFFER_LENGTH];       // Bufor dla UART
char *cmdList[CMD_NUM] = {        // Zbiór rozpoznawanych komend
    ":STA", ":RST", ":CTR", ":VAL",
    ":RES", ":ACT", ":TRI",
    ":LED", ":DIP",
    ":HEI", ":HEO", ":HET"};

// #define USE_SPRINTF

/* ------------------------------------------------------------
 * INFO Main function
 * ------------------------------------------------------------ */
int main(void)
{
  uint16_t length;

  memset(buffer, 0, BUFFER_LENGTH);                // Zerowanie bufora odczytu UART
  neo430_uart_setup(BAUD_RATE);                    // Uruchomienie UART
  neo430_uart_br_print("\nCtrlM - hello world\n"); // komunikat powitalny
  crypto_init();                                   // Inicjalizacja modułu CRYPTO
  while (1)
  {
    neo430_uart_br_print("\nCONTROL_DUT:> ");            // Znak zachęty dla komend - gotowość do pracy
    length = neo430_uart_scan(buffer, BUFFER_LENGTH, 0); // Wczytanie komendy
    if (length < CMD_LENGTH)                             // Sprawdzenie minimalnej długości komendy
    {                                                    // Za mała długość oznacza ponowne wczytanie komendy po 10 ms
      neo430_uart_br_print("\n\tCommand to short");      // Informacja o błędzie
    }
    else // Próba wykonania komendy
    {
      crypto_cmd(buffer);           // Wywołanie interpretera komend
      neo430_uart_br_print(buffer); // Wyświetlenie komunikatu zwrotnego z funkcji crypto_cmd
    }
  }
}

//-----------------------------------------------------------------------------
// Inicjalizacja modułu CRYPTO
void crypto_init()
{
  uint16_t value = WB_CRYPTO_DEFAULT_INIT_VAL;
  wb_crypto_reset();              // Komenda RESET
  wb_crypto_ctrl_set(0x0000);     // Wyzerowanie CTRL
  wb_crypto_value_set(&value, 1); // Ustawienie startowe VALUE
}

// Przetwarzanie odebranych komend
void crypto_cmd(char *buffer)
{
  uint16_t cmd = 0; // Numer komendy

  CMD = strupr(buffer);               // Zmiana wszystkich znaków na duże litery
  for (cmd = 0; cmd < CMD_NUM; cmd++) // Przeszukiwanie listy komend
  {
    if (!strncmp(CMD, cmdList[cmd], CMD_LENGTH - 1)) // Pierwsze dopasowanie przerywa pętle
      break;
  }
  switch (CMD_TYPE) // Sprawdzenie typu komendy - SET/GET
  {
  case '=':      // Komenda SET
    cmdSet(cmd); // Wykonanie komendy
    break;
  case '?':      // Komenda GET
    cmdGet(cmd); // Wykonanie komendy
    break;
  default: // W każdym innym przypadku
#ifndef USE_SPRINTF
    sprintf(buffer, "Bad command parameter");
#else
    strcpy(buffer, "Bad command parameter");
#endif
    break;
  }
}
// Interpretacja komend ustawiających
void cmdSet(uint16_t cmd)
{
  uint16_t length = getCmdValue(CMD_VALUE, value); // Odczytanie przesłanej wartości z komendą =0x...
  switch (cmd)
  {
  case 0:
    wb_crypto_start_set(*value);
    break;
  case 1:
    wb_crypto_reset();
    break;
  case 2:
    wb_crypto_ctrl_set(*value);
    break;
  case 3:
    wb_crypto_value_set(value, length);
    break;
  case 7:
    wb_io_led_set(*value);
    break;
  default:
#ifndef USE_SPRINTF
    sprintf(buffer, "\nBad command value\n");
#else
    strcpy(buffer, "\nBad command value\n");
#endif
#ifndef USE_SPRINTF
    sprintf(buffer, "\nOK\n");
#else
    strcpy(buffer, "\nOK\n");
#endif
    break;
  }
}

// Odczytanie wartości z bufora do tablicy value
uint16_t getCmdValue(char *buffer, uint16_t *value)
{
  uint16_t length = strlen(buffer);     // Uzyskanie informacji o liczbie znaków w liczbie
  length = (length > 64) ? 64 : length; // Sprawdzenie maksymalnej długości wartości do wczytania
  uint16_t w = (length / 4);            // Liczba słów do wpisu
  uint16_t wr = (length % 4);           // Sprawdzenie reszty z dzielenia
  uint16_t ret = 0;
  clear(value, CMD_VALUE_LENGTH); // Wyczyszczenie bufora do zapisu liczby
  if (w == 0)                     // Jeżeli długość liczby mniejsza niż 4 znaki
  {
    *value = (uint16_t)neo430_hexstr_to_uint(buffer, wr); // Konwersja S2B
    ret++;
  }
  else // Dla długości większej lub rówenej 4
  {
    uint16_t *tmp = value + w - 1;   // Określenie pozycji najbardziej znaczącego pełnego słowa
    for (uint16_t i = 0; i < w; i++) // Odczytywanie kolejnych słów
    {
      if (wr != 0) // Jeżeli liczba znaków nie jest wielokrotnością 4 - pierwsza iteracja
      {
        tmp++;                                              // Wskazanie na kolejne słowo (niepełne)
        *tmp = (uint16_t)neo430_hexstr_to_uint(buffer, wr); // Konwersja niepełnego słowa
        buffer += wr;                                       // Przesunięcie wskaźnika o liczbę odczytanych znaków
        tmp--;                                              // Wskazanie na młodsze słowo pełne
        wr = 0;                                             // Wyzerowanie reszty
        ret++;
      }
      // Wczytanie pełnego słowa
      {
        *tmp = (uint16_t)neo430_hexstr_to_uint(buffer, 4);
        buffer += 4;
        tmp--;
      }
    }
  }
  return (ret + w);
}

void cmdGet(uint16_t cmd)
{
  switch (cmd)
  {
  case 0:
    *value = (uint16_t)wb_crypto_start_get();
    break;
  case 1:
    *value = (uint16_t)0xCAFFE;
    break;
  case 2:
    *value = (uint16_t)wb_crypto_ctrl_get();
    break;
  case 3:
    wb_crypto_value_get(value, CMD_VALUE_LENGTH);
    break;
  case 4:
    wb_crypto_result_get(value);
    break;
  case 5:
    *value = (uint16_t)wb_crypto_status_active_get();
    break;
  case 6:
    *value = (uint16_t)wb_crypto_status_trig_get();
    break;
  case 7:
    *value = (uint16_t)wb_io_led_get();
    break;
  case 8:
    *value = (uint16_t)wb_io_dipsw_get();
    break;
  default:
    break;
  }
  setCmdValue(buffer, value, cmd);
}

void setCmdValue(char *buffer, uint16_t *value, uint16_t cmd)
{
  uint32_t write = 0;
#ifndef USE_SPRINTF
  write = sprintf(buffer, "\n%s = 0x", cmdList[cmd]); // Nagłówek odpowiedzi
#else
  strcpy(buffer, "\n");
  strcat(buffer, cmdList[cmd]);
  strcat(buffer, " = 0x");
#endif
  if ((cmd != 3) && (cmd != 4)) // Dla komend innych niż :VAL? i :RES? zapis jednego słowa
  {
#ifndef USE_SPRINTF
    sprintf(buffer + write, "%x", *value); // Zapisanie słowa do bufora
#else
    sprintf(buffer + write, "%x", *value); // Zapisanie słowa do bufora
                                           // strcat(buffer, );
#endif
  }
  else // Dla komend :VAL? i :RES? zapis określonej liczby słów
  {
    uint16_t n = (cmd == 3) ? CMD_VALUE_LENGTH : CMD_RESULT_LENGTH; // Określenie liczby słów do zapisania
    uint16_t *tmp = value + n - 1;                                  // Określenie pozycji najbardziej znaczącego pełnego słowa
    for (size_t i = 0; i < n; i++)
    {
#ifndef USE_SPRINTF
      write += sprintf(buffer + write, "%.4x", *tmp);
#else
      write += sprintf(buffer + write, "%.4x", *tmp);
      // strcat(buffer, );
#endif
      tmp--;
    }
  }
  clear(value, CMD_VALUE_LENGTH); // Wyczyszczenie bufora liczby
}

void clear(uint16_t *value, uint16_t n)
{
  for (uint16_t i = 0; i < n; i++)
  {
    *value = 0;
    value++;
  }
}
#ifdef USE_SPRINTF
static int num_hex_digits(unsigned n)
{
  if (!n)
    return 1;

  int ret = 0;
  for (; n; n >>= 4)
  {
    ++ret;
  }
  return ret;
}

void make_hex_string_learning(unsigned n, char *s)
{
  const char hex_lookup[] = "0123456789abcdef";
  int len = num_hex_digits(n);

  if (len & 1)
  {
    *s++ = '0';
  }
  s[len] = '\0';

  for (--len; len >= 0; n >>= 4, --len)
  {
    s[len] = hex_lookup[n & 0xf];
  }
}
#endif
