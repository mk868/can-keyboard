#include <Arduino.h>

#include <CAN.h>
#include <Wire.h>

const uint8_t CAN_PREV_TRACK[8] = {137, 1, 65, 162, 162, 162, 162, 162};
const uint8_t CAN_NEXT_TRACK[8] = {137, 1, 1, 162, 162, 162, 162, 162};

const int PIN_PREV = 11;
const int PIN_NEXT = 12;

void setup() {
    Serial.begin(0);

    delay(1000);
    Serial.println("Hello");

    if (!CAN.begin(500E3)) {
        while (true) {
            Serial.println("CAN ERROR");
            delay(1000);
        }
    }

    Serial.println("CAN ok");

    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(PIN_PREV, INPUT_PULLUP);
    pinMode(PIN_NEXT, INPUT_PULLUP);
}

void loop() {
    delay(100);

    auto ch = Serial.read();

    if (ch == 'p' || !digitalRead(PIN_PREV)) {
        Serial.println("prev");

        CAN.beginPacket(1423);
        CAN.write(CAN_PREV_TRACK, sizeof(CAN_PREV_TRACK));
        CAN.endPacket();

        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
        delay(500);
    }
    if (ch == 'n' || !digitalRead(PIN_NEXT)) {
        Serial.println("next");

        CAN.beginPacket(1423);
        CAN.write(CAN_NEXT_TRACK, sizeof(CAN_NEXT_TRACK));
        CAN.endPacket();

        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
        delay(500);
    }
}
