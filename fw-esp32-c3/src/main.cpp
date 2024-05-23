#include <Arduino.h>
#include <ESP32-TWAI-CAN.hpp>

#include "state_machine.h"
#include "multimedia.h"
#include "config.h"

static void on_can_receive(const CanFrame&frame);

static const int GREEN_LED = 6;
static const int RED_LED = 5;

void setup() {
    state_machine_init(100);

    Serial.begin(115200);
    Serial.println("Hi");

    multimedia_init();

    ESP32Can.setPins(CAN_TX, CAN_RX);
    ESP32Can.setSpeed(TWAI_SPEED_500KBPS);
    if (!ESP32Can.begin()) {
        while (true) {
            delay(1000);
            Serial.println("CAN error");
        }
    }
    Serial.println("CAN ok");

    delay(200);
    Serial.println("ready");

    pinMode(GREEN_LED, OUTPUT);
    pinMode(RED_LED, OUTPUT);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, LOW);
}

void loop() {
    CanFrame rxFrame;
    if (ESP32Can.readFrame(rxFrame, 100)) {
        on_can_receive(rxFrame);
    }

    state_t state = state_machine_get_state();
    if (state == STATE_PREV_TRACK) {
        digitalWrite(GREEN_LED, HIGH);
        multimedia_prev_track();
        digitalWrite(GREEN_LED, LOW);
    }
    if (state == STATE_NEXT_TRACK) {
        digitalWrite(GREEN_LED, HIGH);
        multimedia_next_track();
        digitalWrite(GREEN_LED, LOW);
    }
    if (state == STATE_RESTART) {
        digitalWrite(GREEN_LED, HIGH);
        ESP.restart();
    }
    state_machine_reset();
}

static void on_can_receive(const CanFrame&frame) {
    const uint32_t id = frame.identifier;
    if (frame.data_length_code != 8) {
        return;
    }

    if (id == CAR_BUTTON_UP.packet_id && !memcmp(CAR_BUTTON_UP.data, frame.data, sizeof(CAR_BUTTON_UP.data))) {
        Serial.println("PREV_TRACK frame");
        state_machine_set_state_debouncing(STATE_PREV_TRACK);
    }
    if (id == CAR_BUTTON_DOWN.packet_id && !memcmp(CAR_BUTTON_DOWN.data, frame.data, sizeof(CAR_BUTTON_DOWN.data))) {
        Serial.println("NEXT_TRACK frame");
        state_machine_set_state_debouncing(STATE_NEXT_TRACK);
    }
}
