volatile ev_state_t EVPREFIX_internal_status;

void EVPREFIX_init()
{
    EV_CMD_PORT.DIRSET = EV_CMD_OPEN_bm | EV_CMD_CLOSE_bm;
    EV_CMD_PORT.OUTCLR = EV_CMD_OPEN_bm | EV_CMD_CLOSE_bm;
    EV_SENSOR_PORT.DIRCLR = EV_SENSOR_OPEN_bm | EV_SENSOR_CLOSE_bm;
    EV_SENSOR_OPEN_PINCTRL = PORT_OPC_PULLUP_gc;
    EV_SENSOR_CLOSE_PINCTRL = PORT_OPC_PULLUP_gc;
    EVPREFIX_internal_status = EV_MID;
}

uint8_t EVPREFIX_internal_step_close(uint16_t d)
{
    if(EVPREFIX_internal_status == EV_CLOSED)
        return 1;

    if(d<5)
        return 0;
    
    if(EVPREFIX_internal_status == EV_OPENED)
        d = 500;
    EVPREFIX_internal_status = EV_MID;
    
    EVPREFIX_callback_close();
    uint16_t k;
    EV_CMD_PORT.OUTSET = EV_CMD_CLOSE_bm;
    for(k=0;k<d;k++)
        _delay_ms(1);
    if(!(EV_SENSOR_PORT.IN & EV_SENSOR_CLOSE_bm))
        EVPREFIX_internal_status = EV_CLOSED;
    EV_CMD_PORT.OUTCLR = EV_CMD_CLOSE_bm;

    if(EVPREFIX_internal_status == EV_CLOSED)
        return 1;
    return 0;
}

uint8_t EVPREFIX_internal_step_open(uint16_t d)
{
    if(EVPREFIX_internal_status == EV_OPENED)
        return 1;

    if(d<5)
        return 0;
    
    if(EVPREFIX_internal_status == EV_CLOSED)
        d = 500;
    EVPREFIX_internal_status = EV_MID;
    
    EVPREFIX_callback_open();
    uint16_t k;
    EV_CMD_PORT.OUTSET = EV_CMD_OPEN_bm;
    for(k=0;k<d;k++)
        _delay_ms(1);
    if(!(EV_SENSOR_PORT.IN & EV_SENSOR_OPEN_bm))
        EVPREFIX_internal_status = EV_OPENED;
    EV_CMD_PORT.OUTCLR = EV_CMD_OPEN_bm;

    if(EVPREFIX_internal_status == EV_OPENED)
        return 1;
    return 0;
}

uint8_t EVPREFIX_step_close(uint16_t d)
{
    uint16_t steps = d/(d/101+1);
    uint16_t k;
    uint8_t ret;
    for(k=0;k<d;k+=steps)
    {
        if(k+steps<=d)
            ret = EVPREFIX_internal_step_close(steps);
        else
            ret = EVPREFIX_internal_step_close(d-k);
        if(ret)
            return ret;
    }
    return ret;
}

uint8_t EVPREFIX_step_open(uint16_t d)
{
    uint16_t steps = d/(d/101+1);
    uint16_t k;
    uint8_t ret;
    for(k=0;k<d;k+=steps)
    {
        if(k+steps<=d)
            ret = EVPREFIX_internal_step_open(steps);
        else
            ret = EVPREFIX_internal_step_open(d-k);
        if(ret)
            return ret;
    }
    return ret;
}

void EVPREFIX_close()
{
    uint16_t k;
    for(k=0;k<1000;k++)
        if(EVPREFIX_internal_step_close(10))
            break;
}

void EVPREFIX_open()
{
    uint16_t k;
    for(k=0;k<1000;k++)
        if(EVPREFIX_internal_step_open(10))
            break;
}

void EVPREFIX_vidange()
{
    uint16_t k;
    EVPREFIX_close();
    for(k=0;k<1000;k++)
        if(EVPREFIX_internal_step_open(10))
            break;
    EVPREFIX_close();
    uint16_t k2=0;
    for(k2=0;k2<k/2;k2++)
        if(EVPREFIX_internal_step_open(10))
            break;
}
