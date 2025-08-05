#include <util/crc16.h>
#include <util/delay.h>

uint8_t
_DS_reset();
void
_DS_writebit(uint8_t bit);
uint8_t
_DS_readbit(void);
void
_DS_writebyte(uint8_t byte);
uint8_t
_DS_readbyte(void);
int8_t
_DS_gettemp(int16_t* temp, uint16_t timeout_ms);
