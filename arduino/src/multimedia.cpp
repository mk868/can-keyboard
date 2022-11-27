#include "multimedia.h"

#include <Arduino.h>

#include <BleKeyboard.h>

static BleKeyboard bleKeyboard("CAN keyboard");

void multimedia_init()
{
    bleKeyboard.begin();
}

void multimedia_prev_track()
{

    Serial.print("PREV TRACK: ");
    if (!bleKeyboard.isConnected())
    {
        Serial.println("NC");
        return;
    }
    bleKeyboard.write(KEY_MEDIA_PREVIOUS_TRACK);
    Serial.println("sent");
}

void multimedia_next_track()
{
    Serial.printf("NEXT TRACK: ");
    if (!bleKeyboard.isConnected())
    {
        Serial.println("NC");
        return;
    }
    bleKeyboard.write(KEY_MEDIA_NEXT_TRACK);
    Serial.println("sent");
}
