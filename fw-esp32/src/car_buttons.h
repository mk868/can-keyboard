
#pragma once

struct car_button
{
    const long packet_id;
    const int data[8];
};

const struct car_button CAR_BUTTON_UP = {.packet_id = 0x058F, .data = {0x89, 0x01, 0x41, 0xA2, 0xA2, 0xA2, 0xA2, 0xA2}};
const struct car_button CAR_BUTTON_DOWN = {.packet_id = 0x058F, .data = {0x89, 0x01, 0x01, 0xA2, 0xA2, 0xA2, 0xA2, 0xA2}};
