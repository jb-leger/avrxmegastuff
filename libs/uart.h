#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void
USARTPREFIX_init();

#if tx
void
USARTPREFIX_putchar(uint8_t ch);
void
USARTPREFIX_putzchar(const char* zchar);
void
USARTPREFIX_putfloat(const float f, uint8_t decsize);
void
USARTPREFIX_printf(const char* fmt, ...);
#endif

#if rx
void
UART_rxhandler(const char* msg);
#endif
