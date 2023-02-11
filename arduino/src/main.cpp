#include <Arduino.h>
#include <CAN.h>

#include "state_machine.h"
#include "multimedia.h"
#include "car_buttons.h"

static void on_serial_receive();
static void on_can_receive(int packetSize);

static const int GREEN_LED = 23;

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

    pinMode(GREEN_LED, OUTPUT);
    digitalWrite(GREEN_LED, HIGH);
}

void loop()
{
    state_t state = state_machine_get_state();
    if (state == STATE_PREV_TRACK)
    {
        digitalWrite(GREEN_LED, LOW);
        multimedia_prev_track();
        digitalWrite(GREEN_LED, HIGH);
    }
    if (state == STATE_NEXT_TRACK)
    {
        digitalWrite(GREEN_LED, LOW);
        multimedia_next_track();
        digitalWrite(GREEN_LED, HIGH);
    }
    if (state == STATE_RESTART)
    {
        digitalWrite(GREEN_LED, LOW);
        ESP.restart();
    }
    state_machine_reset();
}

static void on_serial_receive()
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

static void on_can_receive(int packetSize)
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

    if (id == CAR_BUTTON_UP.packet_id && memcmp(CAR_BUTTON_UP.data, buff, sizeof(CAR_BUTTON_UP.data)) == 0)
    {
        state_machine_set_state_debouncing(STATE_PREV_TRACK);
    }
    if (id == CAR_BUTTON_DOWN.packet_id && memcmp(CAR_BUTTON_DOWN.data, buff, sizeof(CAR_BUTTON_DOWN.data)) == 0)
    {
        state_machine_set_state_debouncing(STATE_NEXT_TRACK);
    }
}
