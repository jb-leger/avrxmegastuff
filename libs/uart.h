#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>

void _USART_init();

#if tx
void _USART_putchar(uint8_t ch);
void _USART_putzchar(const char* zchar);
void _USART_putfloat(const float f, uint8_t decsize);
void _USART_printf(const char *fmt, ...);
#endif

#if rx
void USARTrxhandler(const char *msg);
#endif
