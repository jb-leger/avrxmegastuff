#include <util/delay.h>
#include <util/crc16.h>

#define DS18B20_CMD_CONVERTTEMP 0x44
#define DS18B20_CMD_RSCRATCHPAD 0xbe
#define DS18B20_CMD_WSCRATCHPAD 0x4e
#define DS18B20_CMD_CPYSCRATCHPAD 0x48
#define DS18B20_CMD_RECEEPROM 0xb8
#define DS18B20_CMD_RPWRSUPPLY 0xb4
#define DS18B20_CMD_SEARCHROM 0xf0
#define DS18B20_CMD_READROM 0x33
#define DS18B20_CMD_MATCHROM 0x55
#define DS18B20_CMD_SKIPROM 0xcc
#define DS18B20_CMD_ALARMSEARCH 0xec

uint8_t _DS_reset();
void _DS_writebit(uint8_t bit);
uint8_t _DS_readbit(void);
void _DS_writebyte(uint8_t byte);
uint8_t _DS_readbyte(void);
int8_t _DS_gettemp(int16_t* temp, uint16_t timeout_ms);
