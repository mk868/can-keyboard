#pragma once

#include <stdlib.h>

enum state_t
{
    STATE_IDLE,
    STATE_NEXT_TRACK,
    STATE_PREV_TRACK,
    STATE_RESTART
};

void state_machine_init(uint16_t debounce_duration);
bool state_machine_set_state_debouncing(state_t state);
state_t state_machine_get_state();
void state_machine_reset();
