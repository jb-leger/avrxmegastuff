volatile MFtype MFPREFIX_circbuf[MF_len];
volatile MFidxtype MFPREFIX_circidx;
volatile uint8_t MFPREFIX_filled;

void MFPREFIX_fill(MFtype value)
{
    for(MFPREFIX_circidx = 0; MFPREFIX_circidx<MF_len; MFPREFIX_circidx++)
        MFPREFIX_circbuf[MFPREFIX_circidx] = value;
    MFPREFIX_circidx=0;
    MFPREFIX_filled = 1;
}

void MFPREFIX_update(MFtype value)
{
    MFPREFIX_circbuf[MFPREFIX_circidx] = value;
    MFPREFIX_circidx++;
    MFPREFIX_circidx%=MF_len;
}

MFtype MFPREFIX_read()
{
    MFtype temp[MF_len];

    MFidxtype i;
    for(i=0; i<MF_len; i++)
        temp[i] = MFPREFIX_circbuf[i];

    MFidxtype j;

    for(i=0; i<=MF_len/2; i++)
    {
        for(j=i+1; j<MF_len; j++)
        {
            if(temp[i]>temp[j])
            {
                MFtype swap = temp[i];
                temp[i] = temp[j];
                temp[j] = swap;
            }
        }
    }

    return temp[MF_len/2];
}

