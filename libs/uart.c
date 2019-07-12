
#if tx
    volatile uint8_t _USART_txbuffer[USARTtxlen];
    volatile uint16_t _USART_txidxB;
    volatile uint16_t _USART_txidxA;
#endif

#if rx
    volatile uint8_t _USART_rxbuffer[USARTrxlen];
    volatile uint16_t _USART_rxidx;
#endif

void _USART_init()
{
    USARTport.OUTSET = USARTtxpin;
    USARTport.DIRSET = USARTtxpin;

    uint16_t bscale = 0;
    if(USARTbaudrate<600)
        bscale++;
    if(USARTbaudrate<300)
        bscale++;
    if(USARTbaudrate<150)
        bscale++;
    if(USARTbaudrate<75)
        bscale++;
    if(USARTbaudrate<36)
        bscale++;
    if(USARTbaudrate<18)
        bscale++;
    if(USARTbaudrate<9)
        bscale++;

    uint16_t bsel = (F_CPU-((((uint32_t)8)<<bscale) * USARTbaudrate)) / ((((uint32_t)16)<<bscale) * USARTbaudrate);

    _USART.BAUDCTRLA = bsel;
    _USART.BAUDCTRLB = (bscale << USART_BSCALE_gp) | (bsel >> 8);

    _USART.CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_PMODE_DISABLED_gc;
    if(USARTchsize==5)
        _USART.CTRLC |= USART_CHSIZE_5BIT_gc;
    else if(USARTchsize==6)
        _USART.CTRLC |= USART_CHSIZE_6BIT_gc;
    else if(USARTchsize==7)
        _USART.CTRLC |= USART_CHSIZE_7BIT_gc;
    else if(USARTchsize==8)
        _USART.CTRLC |= USART_CHSIZE_8BIT_gc;
    else
        _USART.CTRLC |= USART_CHSIZE_9BIT_gc;
    _USART.CTRLA = (USARTrxen?USART_RXCINTLVL_MED_gc:USART_RXCINTLVL_OFF_gc);
    _USART.CTRLB = (USARTrxen?USART_RXEN_bm:0x00) | (USARTtxen?USART_TXEN_bm:0x00);
}

#if rx
ISR(_USART_RXC_vect)
{
    uint8_t received = _USART.DATA;
    if(received==USARTrxtrigger)
    {
        _USART_rxbuffer[_USART_rxidx]=0;
        USARTrxhandler((char*)_USART_rxbuffer);
        _USART_rxidx=0;
    }
    else
    {
        _USART_rxbuffer[_USART_rxidx] = received;
        _USART_rxidx++;
        _USART_rxidx%=USARTrxlen;
    }
}
#endif
    
#if tx
ISR(_USART_DRE_vect)
{
    if(_USART_txidxA == _USART_txidxB)
    {
        // empty
        _USART.CTRLA = (_USART.CTRLA & ~USART_DREINTLVL_gm) | USART_DREINTLVL_OFF_gc;
    }
    else
    {
        _USART.DATA = _USART_txbuffer[_USART_txidxA];
        _USART_txidxA++;
        _USART_txidxA%=USARTtxlen;
    }
}

void _USART_putchar(uint8_t ch)
{
    while((_USART_txidxB+1)%USARTtxlen == _USART_txidxA);

    _USART_txbuffer[_USART_txidxB] = ch;
    _USART_txidxB++;
    _USART_txidxB%=USARTtxlen;

    _USART.CTRLA = (_USART.CTRLA & ~USART_DREINTLVL_gm) | USART_DREINTLVL_MED_gc;
}

void _USART_putzchar(const char* zchar)
{
    uint16_t i=0;
    while(zchar[i]!=0)
    {
        _USART_putchar(zchar[i]);
        i++;
    }
}

void _USART_putfloat(const float f, uint8_t decsize)
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
                _USART_putzchar(buf);
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
            _USART_putzchar(buf);
    }
}

void _USART_printf(const char *fmt, ...)
{
    char buf[USARTtxlen];

    va_list ap;
    va_start(ap, fmt);
    int16_t n = vsnprintf(buf, USARTtxlen, fmt, ap);
    va_end(ap);

    if (n < USARTtxlen)
        _USART_putzchar(buf);
}

#endif
