void LCDPREFIX_init()
{
    LCD_DATA_PORT.DIRSET = LCD_DATA_PIN7 | LCD_DATA_PIN6 | LCD_DATA_PIN5 | LCD_DATA_PIN4;
    LCD_E_PORT.DIRSET = LCD_E_PIN;
    LCD_RS_PORT.DIRSET = LCD_RS_PIN;

    _delay_ms(100);

    LCD_RS_PORT.OUTCLR = LCD_RS_PIN;
    LCD_E_PORT.OUTCLR = LCD_E_PIN;

    LCDPREFIX_write(LCD_CMD_FUNCTIONRESET);
    _delay_ms(10);

    LCDPREFIX_write(LCD_CMD_FUNCTIONRESET);
    _delay_us(200);

    LCDPREFIX_write(LCD_CMD_FUNCTIONRESET);
    _delay_us(200);
 
    LCDPREFIX_write(LCD_CMD_FUNCTIONSET4BIT);
    _delay_us(80);

    LCDPREFIX_write_instruction(LCD_CMD_FUNCTIONSET4BIT);
    _delay_us(80);

    LCDPREFIX_write_instruction(LCD_CMD_DISPLAYOFF);
    _delay_us(80);

    LCDPREFIX_write_instruction(LCD_CMD_CLEAR);
    _delay_ms(4);

    LCDPREFIX_write_instruction(LCD_CMD_ENTRYMODE);
    _delay_us(80);

    LCDPREFIX_write_instruction(LCD_CMD_DISPLAYON);
    _delay_us(80);
LCDCUSTOMCHARS}

void LCDPREFIX_write_zchar(char* what)
{
    LCDPREFIX_write_instruction(LCD_CMD_CLEAR);
    _delay_ms(4);
    
    LCDPREFIX_write_instruction(LCD_CMD_SETCURSOR | LCD_CMD_LINEONE);
    _delay_us(80);

    uint8_t i=0;
    for(i=0; i<16; i++)
    {
        if(what[i]==0)
            break;
        if(what[i]=='\n')
            break;
        LCDPREFIX_write_char(what[i]);
        _delay_us(80);
    }
    
    if(what[i]!=0)
    {
        LCDPREFIX_write_instruction(LCD_CMD_SETCURSOR | LCD_CMD_LINETWO);
        _delay_us(80);
        i++;
        uint8_t istart = i;
        while(i-istart<16)
        {
            if(what[i]==0)
                break;
            if(what[i]=='\n')
                break;
            LCDPREFIX_write_char(what[i]);
            _delay_us(80);
            i++;
        }
    }
}


void LCDPREFIX_write_lines(char* line0, char* line1)
{
    LCDPREFIX_write_instruction(LCD_CMD_CLEAR);
    _delay_ms(4);

    LCDPREFIX_write_instruction(LCD_CMD_SETCURSOR | LCD_CMD_LINEONE);
    _delay_us(80);

    LCDPREFIX_write_a_line(line0);
    
    LCDPREFIX_write_instruction(LCD_CMD_SETCURSOR | LCD_CMD_LINETWO);
    _delay_us(80);
    
    LCDPREFIX_write_a_line(line1);
}


void LCDPREFIX_write_a_line(char* what)
{
    uint8_t i=0;
    for(i=0; i<16; i++)
    {
        if(what[i]==0)
            break;
        if(what[i]=='\n')
            break;
        LCDPREFIX_write_char(what[i]);
        _delay_us(80);
    }
}   


void LCDPREFIX_write_char(uint8_t what)
{
    LCD_RS_PORT.OUTSET = LCD_RS_PIN;
    LCD_E_PORT.OUTCLR = LCD_E_PIN;
    LCDPREFIX_write(what);
    LCDPREFIX_write(what << 4);
}

void LCDPREFIX_write_instruction(uint8_t what)
{
    LCD_RS_PORT.OUTCLR = LCD_RS_PIN;
    LCD_E_PORT.OUTCLR = LCD_E_PIN;
    LCDPREFIX_write(what);
    LCDPREFIX_write(what << 4);
}

void LCDPREFIX_write(uint8_t what)
{
    LCD_DATA_PORT.OUTCLR = LCD_DATA_PIN7 | LCD_DATA_PIN6 | LCD_DATA_PIN5 | LCD_DATA_PIN4;
    uint8_t outset = 0x00;
    if(what & (1<<7))
        outset |= LCD_DATA_PIN7;
    if(what & (1<<6))
        outset |= LCD_DATA_PIN6;
    if(what & (1<<5))
        outset |= LCD_DATA_PIN5;
    if(what & (1<<4))
        outset |= LCD_DATA_PIN4;
    LCD_DATA_PORT.OUTSET = outset;

    LCD_E_PORT.OUTSET = LCD_E_PIN;
    _delay_us(1);
    LCD_E_PORT.OUTCLR = LCD_E_PIN;
    _delay_us(1);
}
