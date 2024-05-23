#pragma once

#define BLE_DEV_NAME "CAN keyboard C3"

#define CAN_TX		0
#define CAN_RX		1

struct car_button {
    const long packet_id;
    const uint8_t data[8];
};

const car_button CAR_BUTTON_UP = {.packet_id = 0x058F, .data = {0x89, 0x01, 0x41, 0xA2, 0xA2, 0xA2, 0xA2, 0xA2}};
const car_button CAR_BUTTON_DOWN = {.packet_id = 0x058F, .data = {0x89, 0x01, 0x01, 0xA2, 0xA2, 0xA2, 0xA2, 0xA2}};
