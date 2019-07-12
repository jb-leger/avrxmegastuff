#include <avr/io.h>

void init_osc()
{
    // We enable oscillator 32M and 32K
    OSC.CTRL |= OSC_RC32MEN_bm | OSC_RC32KEN_bm;
    
    // No prescalers
    _PROTECTED_WRITE( CLK.PSCTRL, CLK_PSADIV_1_gc | CLK_PSBCDIV_1_1_gc );
    
    // We wait until oscillators are ready
    while((OSC.STATUS & OSC_RC32MRDY_bm) == 0);
    while((OSC.STATUS & OSC_RC32KRDY_bm) == 0);
    
    // calbration with interal 32k osc
    OSC.DFLLCTRL = ( OSC.DFLLCTRL & ~OSC_RC32MCREF_gm );
    
    // enable calibration
    DFLLRC32M.CTRL |= DFLL_ENABLE_bm;
    
    // change clock source
    _PROTECTED_WRITE( CLK.CTRL, ( CLK.CTRL & ~CLK_SCLKSEL_gm ) | CLK_SCLKSEL_RC32M_gc);
}
