#include "state_machine.h"

#include <Arduino.h>

static uint8_t _debounce_duration;
static state_t _state = STATE_IDLE;
static uint64_t _state_change_time = 0;

void state_machine_init(uint16_t debounce_duration)
{
    _debounce_duration = debounce_duration;
}

bool state_machine_set_state_debouncing(state_t state)
{
    uint64_t now = millis();

    if (_state_change_time + _debounce_duration > now)
    {
        return false;
    }

    _state = state;
    _state_change_time = now;
    return true;
}

state_t state_machine_get_state()
{
    return _state;
}

void state_machine_reset()
{
    _state = STATE_IDLE;
}