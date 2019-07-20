#ifndef H_ELECTROVANNE_TYPES
#define H_ELECTROVANNE_TYPES 1

typedef enum state_ev
{
    EV_MID,
    EV_OPENED,
    EV_CLOSED,
} ev_state_t;

#endif

void EVPREFIX_init();
uint8_t EVPREFIX_internal_step_close(uint16_t d);
uint8_t EVPREFIX_internal_step_open(uint16_t d);
uint8_t EVPREFIX_step_close(uint16_t d);
uint8_t EVPREFIX_step_open(uint16_t d);
void EVPREFIX_close();
void EVPREFIX_open();
void EVPREFIX_vidange();
