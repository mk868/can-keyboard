#include <Arduino.h>
#include <CAN.h>

#include "state_machine.h"
#include "multimedia.h"

static void on_serial_receive();
static void on_can_receive(int packetSize);

void setup()
{
    state_machine_init(100);

    Serial.begin(115200);
    Serial.onReceive(on_serial_receive);
    Serial.println("Hi");

    multimedia_init();

    if (!CAN.begin(500E3))
    {
        while (true)
        {
            delay(1000);
            Serial.println("CAN error");
        }
    }
    Serial.println("CAN ok");

    CAN.onReceive(on_can_receive);

    delay(200);
    Serial.println("ready");
}

void loop()
{
    state_t state = state_machine_get_state();
    if (state == STATE_PREV_TRACK)
    {
        multimedia_prev_track();
    }
    if (state == STATE_NEXT_TRACK)
    {
        multimedia_next_track();
    }
    if (state == STATE_RESTART)
    {
        ESP.restart();
    }

    state_machine_reset();
}

void on_serial_receive()
{
    while (Serial.available())
    {
        int ch = Serial.read();

        if (ch == ',')
        {
            state_machine_set_state_debouncing(STATE_PREV_TRACK);
        }
        if (ch == '.')
        {
            state_machine_set_state_debouncing(STATE_NEXT_TRACK);
        }
        if (ch == 'r')
        {
            state_machine_set_state_debouncing(STATE_RESTART);
        }
    }
}

const int CAN_PREV_TRACK[8] = {137, 1, 65, 162, 162, 162, 162, 162};
const int CAN_NEXT_TRACK[8] = {137, 1, 1, 162, 162, 162, 162, 162};

void on_can_receive(int packetSize)
{
    long id = CAN.packetId();
    int buff[packetSize];

    if (CAN.packetRtr())
    {
        return;
    }

    for (int i = 0; CAN.available(); i++)
    {
        buff[i] = CAN.read();
    }
    if (packetSize != 8)
    {
        return;
    }

    if (id == 1423)
    {
        if (memcmp(CAN_PREV_TRACK, buff, sizeof(CAN_PREV_TRACK)) == 0)
        {
            state_machine_set_state_debouncing(STATE_PREV_TRACK);
        }
        if (memcmp(CAN_NEXT_TRACK, buff, sizeof(CAN_PREV_TRACK)) == 0)
        {
            state_machine_set_state_debouncing(STATE_NEXT_TRACK);
        }
    }
}
