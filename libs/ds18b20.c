uint8_t _DS_reset()
{
    DS18B20_port.DIRSET = DS18B20_pin;
    DS18B20_port.OUTCLR = DS18B20_pin;
	_delay_us(480); // 480 min (480)

    DS18B20_port.DIRCLR = DS18B20_pin;
	_delay_us(60); // 60-255 (60)

    uint8_t ret = DS18B20_port.IN & DS18B20_pin; // 0: ok
	_delay_us(420);  // (420)

	return ret;
}

void _DS_writebit(uint8_t bit)
{
    DS18B20_port.DIRSET = DS18B20_pin;
    DS18B20_port.OUTCLR = DS18B20_pin;
	_delay_us(1);

	// write 1: release the line
	if(bit)
        DS18B20_port.DIRCLR = DS18B20_pin;

	_delay_us(60); // 59 min
    DS18B20_port.DIRCLR = DS18B20_pin;
	_delay_us(10);
}

uint8_t _DS_readbit(void)
{
	uint8_t ret=0;

    DS18B20_port.DIRSET = DS18B20_pin;
    DS18B20_port.OUTCLR = DS18B20_pin;
	_delay_us(1);

    DS18B20_port.DIRCLR = DS18B20_pin;
	_delay_us(13);

    if(DS18B20_port.IN & DS18B20_pin)
		ret=1;
	_delay_us(60);
	return ret;
}

void _DS_writebyte(uint8_t byte)
{
	uint8_t i;
    for(i=0;i<8;i++)
    {
        _DS_writebit( byte & 0x01);
        byte >>= 1;
    }
}

uint8_t _DS_readbyte(void)
{
    uint8_t ret=0;
    uint8_t i;
    for(i=0;i<8;i++)
    {
        ret >>= 1;
        ret |= (_DS_readbit()<<7);
    }
	return ret;
}

int8_t _DS_gettemp(int16_t* temp, uint16_t timeout_ms)
{
    uint8_t buf[9];
	
    if(_DS_reset())
        return 3;
	_DS_writebyte(DS18B20_CMD_SKIPROM);
	_DS_writebyte(DS18B20_CMD_CONVERTTEMP);

    int16_t delay=0;
	while(!_DS_readbit())
    {
        if(delay>timeout_ms)
            return(2); // timeout
        _delay_ms(1);
        delay++;
    }

	if(_DS_reset())
        return(4);
	_DS_writebyte(DS18B20_CMD_SKIPROM);
	_DS_writebyte(DS18B20_CMD_RSCRATCHPAD);

    uint8_t i;
    for(i=0;i<9;i++)
        buf[i] = _DS_readbyte();

    uint8_t crc=0x00;
    for(i=0;i<8;i++)
        crc = _crc_ibutton_update(crc,buf[i]);
    
    if(crc == buf[8])
    {
	    *temp = ( (int16_t)(((uint16_t)(buf[1]))<<8) | buf[0] );
        return(0); // ok
    }
    
    return(1);
}
