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

uint8_t EVPREFIX_internal_step_close(uint16_t d, uint8_t force)
{
    if(!force)
        if(EVPREFIX_internal_status == EV_CLOSED)
            return 1;

    if(d<EV_ACTION_LIMIT)
        return 0;
    
    if(EVPREFIX_internal_status == EV_OPENED)
        d = 500;
    EVPREFIX_internal_status = EV_MID;
    
    EVPREFIX_callback_close();
    uint16_t k;
    EV_CMD_PORT.OUTSET = EV_CMD_CLOSE_bm;
    for(k=0;k<d;k++)
        _delay_ms(1);
    if(!force)
        if(!(EV_SENSOR_PORT.IN & EV_SENSOR_CLOSE_bm))
            EVPREFIX_internal_status = EV_CLOSED;
    EV_CMD_PORT.OUTCLR = EV_CMD_CLOSE_bm;

    if(EVPREFIX_internal_status == EV_CLOSED)
        return 1;
    return 0;
}

uint8_t EVPREFIX_internal_step_open(uint16_t d, uint8_t force)
{
    if(!force)
        if(EVPREFIX_internal_status == EV_OPENED)
            return 1;

    if(d<EV_ACTION_LIMIT)
        return 0;
    
    if(EVPREFIX_internal_status == EV_CLOSED)
        d = 500;
    EVPREFIX_internal_status = EV_MID;
    
    EVPREFIX_callback_open();
    uint16_t k;
    EV_CMD_PORT.OUTSET = EV_CMD_OPEN_bm;
    for(k=0;k<d;k++)
        _delay_ms(1);
    if(!force)
        if(!(EV_SENSOR_PORT.IN & EV_SENSOR_OPEN_bm))
            EVPREFIX_internal_status = EV_OPENED;
    EV_CMD_PORT.OUTCLR = EV_CMD_OPEN_bm;

    if(EVPREFIX_internal_status == EV_OPENED)
        return 1;
    return 0;
}

uint8_t EVPREFIX_step_close(uint16_t d, uint8_t force)
{
    uint16_t steps = d/(d/101+1);
    uint16_t k;
    uint8_t ret;
    for(k=0;k<d;k+=steps)
    {
        if(k+steps<=d)
            ret = EVPREFIX_internal_step_close(steps, force);
        else
            ret = EVPREFIX_internal_step_close(d-k, force);
        if(ret)
            return ret;
    }
    return ret;
}

uint8_t EVPREFIX_step_open(uint16_t d, uint8_t force)
{
    uint16_t steps = d/(d/101+1);
    uint16_t k;
    uint8_t ret;
    for(k=0;k<d;k+=steps)
    {
        if(k+steps<=d)
            ret = EVPREFIX_internal_step_open(steps, force);
        else
            ret = EVPREFIX_internal_step_open(d-k, force);
        if(ret)
            return ret;
    }
    return ret;
}

void EVPREFIX_close()
{
    uint16_t k;
    for(k=0;k<1000;k++)
        if(EVPREFIX_internal_step_close(10,0))
            break;
    EVPREFIX_internal_status = EV_CLOSED;
}

void EVPREFIX_force_close()
{
    uint16_t k;
    for(k=0;k<1000;k++)
        if(EVPREFIX_internal_step_close(10,1))
            break;
    EVPREFIX_internal_status = EV_CLOSED;
}

void EVPREFIX_open()
{
    uint16_t k;
    for(k=0;k<1000;k++)
        if(EVPREFIX_internal_step_open(10,0))
            break;
    EVPREFIX_internal_status = EV_OPENED;
}

void EVPREFIX_vidange()
{
    uint16_t kclose;
    uint16_t kopen;
    EVPREFIX_close();
    for(kopen=0;kopen<1000;kopen++)
        if(EVPREFIX_internal_step_open(10,0))
            break;
    for(kclose=0;kclose<1000;kclose++)
        if(EVPREFIX_internal_step_close(10,0))
            break;
    uint8_t kmin;
    if(kopen<900)
        kmin = kopen/2;
    else
    {
        if(kclose<900)
            kmin = kclose/2;
        else
            kmin = 50;
    }

    uint16_t k2=0;
    for(k2=0;k2<kmin;k2++)
        if(EVPREFIX_internal_step_open(10,1))
            break;
}
