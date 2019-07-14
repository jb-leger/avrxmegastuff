
#if tx
    volatile uint8_t USARTPREFIX_txbuffer[UART_txlen];
    volatile uint16_t USARTPREFIX_txidxB;
    volatile uint16_t USARTPREFIX_txidxA;
#endif

#if rx
    volatile uint8_t USARTPREFIX_rxbuffer[UART_rxlen];
    volatile uint16_t USARTPREFIX_rxidx;
#endif

void USARTPREFIX_init()
{
    UART_port.OUTSET = UART_txpin;
    UART_port.DIRSET = UART_txpin;

    uint16_t bscale = 0;
    if(UART_baudrate<600)
        bscale++;
    if(UART_baudrate<300)
        bscale++;
    if(UART_baudrate<150)
        bscale++;
    if(UART_baudrate<75)
        bscale++;
    if(UART_baudrate<36)
        bscale++;
    if(UART_baudrate<18)
        bscale++;
    if(UART_baudrate<9)
        bscale++;

    uint16_t bsel = (F_CPU-((((uint32_t)8)<<bscale) * UART_baudrate)) / ((((uint32_t)16)<<bscale) * UART_baudrate);

    UART_USART.BAUDCTRLA = bsel;
    UART_USART.BAUDCTRLB = (bscale << USART_BSCALE_gp) | (bsel >> 8);

    UART_USART.CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_PMODE_DISABLED_gc;
    if(UART_chsize==5)
        UART_USART.CTRLC |= USART_CHSIZE_5BIT_gc;
    else if(UART_chsize==6)
        UART_USART.CTRLC |= USART_CHSIZE_6BIT_gc;
    else if(UART_chsize==7)
        UART_USART.CTRLC |= USART_CHSIZE_7BIT_gc;
    else if(UART_chsize==8)
        UART_USART.CTRLC |= USART_CHSIZE_8BIT_gc;
    else
        UART_USART.CTRLC |= USART_CHSIZE_9BIT_gc;
    UART_USART.CTRLA = (UART_rxen?USART_RXCINTLVL_MED_gc:USART_RXCINTLVL_OFF_gc);
    UART_USART.CTRLB = (UART_rxen?USART_RXEN_bm:0x00) | (UART_txen?USART_TXEN_bm:0x00);
}

#if rx
ISR(UART_RXC_vect)
{
    uint8_t received = UART_USART.DATA;
    if(received==UART_rxtrigger)
    {
        USARTPREFIX_rxbuffer[USARTPREFIX_rxidx]=0;
        UART_rxhandler((char*)USARTPREFIX_rxbuffer);
        USARTPREFIX_rxidx=0;
    }
    else
    {
        USARTPREFIX_rxbuffer[USARTPREFIX_rxidx] = received;
        USARTPREFIX_rxidx++;
        USARTPREFIX_rxidx%=UART_rxlen;
    }
}
#endif
    
#if tx
ISR(UART_DRE_vect)
{
    if(USARTPREFIX_txidxA == USARTPREFIX_txidxB)
    {
        // empty
        UART_USART.CTRLA = (UART_USART.CTRLA & ~USART_DREINTLVL_gm) | USART_DREINTLVL_OFF_gc;
    }
    else
    {
        UART_USART.DATA = USARTPREFIX_txbuffer[USARTPREFIX_txidxA];
        USARTPREFIX_txidxA++;
        USARTPREFIX_txidxA%=UART_txlen;
    }
}

void USARTPREFIX_putchar(uint8_t ch)
{
    while((USARTPREFIX_txidxB+1)%UART_txlen == USARTPREFIX_txidxA);

    USARTPREFIX_txbuffer[USARTPREFIX_txidxB] = ch;
    USARTPREFIX_txidxB++;
    USARTPREFIX_txidxB%=UART_txlen;

    UART_USART.CTRLA = (UART_USART.CTRLA & ~USART_DREINTLVL_gm) | USART_DREINTLVL_MED_gc;
}

void USARTPREFIX_putzchar(const char* zchar)
{
    uint16_t i=0;
    while(zchar[i]!=0)
    {
        USARTPREFIX_putchar(zchar[i]);
        i++;
    }
}

void USARTPREFIX_putfloat(const float f, uint8_t decsize)
{
    int32_t fl;

    char buf[64];
    
    if(decsize>0)
    {
        fl=f;
        float decimal = f-fl;
        if(decimal<0)
            decimal*=-1;
        
        char bufformat[64];
        uint8_t i;
        for(i=0;i<decsize;i++)
            decimal*=10;
        int16_t n;
        n = snprintf(bufformat,64,"%%li.%%0%ulu",decsize);
        if(n<64)
        {
            n = snprintf(buf,64,bufformat,fl,(uint32_t)(decimal));
            if(n<63)
                USARTPREFIX_putzchar(buf);
        }
    }
    else
    {
        if(f>=0)
            fl = f+.5;
        else
            fl = f-.5;

        int16_t n = snprintf(buf,64,"%li.",fl);
        if(n<63)
            USARTPREFIX_putzchar(buf);
    }
}

void USARTPREFIX_printf(const char *fmt, ...)
{
    char buf[UART_txlen];

    va_list ap;
    va_start(ap, fmt);
    int16_t n = vsnprintf(buf, UART_txlen, fmt, ap);
    va_end(ap);

    if (n < UART_txlen)
        USARTPREFIX_putzchar(buf);
}

#endif
