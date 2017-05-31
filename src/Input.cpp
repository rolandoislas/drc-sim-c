//
// Created by rolando on 5/27/17.
//

#include <ctime>
#include <netinet/in.h>
#include <iostream>
#include <zconf.h>
#include "Input.h"
#include "Gamepad.h"
#include "util/BitUtil.h"
#include "data/Constants.h"
#include "util/Args.h"

InputButton *Input::input_button;
InputButtonExtra *Input::input_button_extra;
InputJoystick *Input::input_joystick;
InputTouchpad *Input::input_touchpad;
InputMicBlow *Input::input_mic_blow;

Input::Input() {
    packet = new InputPacketHeaderWiiU();
    clock_gettime(CLOCK_MONOTONIC_RAW, &last_sent_time);
    input_button = new InputButton();
    input_button_extra = new InputButtonExtra();
    input_joystick = new InputJoystick();
    input_touchpad = new InputTouchpad();
    input_mic_blow = new InputMicBlow();
}

void Input::run() {
    Input input;
    while (!Gamepad::running) {
        usleep(1000 * 1000);
    }
    input.running = true;
    while (input.running) {
        input.update();
    }
}

void Input::update() {
    time_t delta = get_delta_vs_current(last_sent_time);
    if (delta >= HID_UPDATE_INTERVAL) {
        clock_gettime(CLOCK_MONOTONIC_RAW, &last_sent_time);
        send_hid_update();
    }
}

void Input::send_hid_update() {
    memset(packet, 0, sizeof(InputPacketHeaderWiiU));
    // Touch screen
    InputTouchpad *touch_coords = new InputTouchpad();
    get_touch_input(touch_coords);
    if (touch_coords->x > 0 and touch_coords->y > 0) {
        for (int point = 0; point < 10; ++point) {
            for (int coord = 0; coord < 2; ++coord) {
                packet->touchscreen.points[point].coords[coord].value = coord == 0 ? touch_coords->x : touch_coords->y;
#if __BYTE_ORDER == LITTLE_ENDIAN
                packet->touchscreen.points[point].coords[coord].value =
                        BitUtil::reverse(packet->touchscreen.points[point].coords[coord].value, 12);
#endif
                packet->touchscreen.points[point].coords[coord].pad = 1;
            }
        }
        packet->touchscreen.points[0].coords[0].extra = 0;
        packet->touchscreen.points[0].coords[1].extra = 2;
        packet->touchscreen.points[1].coords[0].extra = 7;
        packet->touchscreen.points[1].coords[1].extra = 3;
#if __BYTE_ORDER == LITTLE_ENDIAN
        packet->touchscreen.points[0].coords[1].extra =
                BitUtil::reverse(packet->touchscreen.points[0].coords[1].extra, 3);
        packet->touchscreen.points[1].coords[0].extra =
                BitUtil::reverse(packet->touchscreen.points[1].coords[0].extra, 3);
        packet->touchscreen.points[1].coords[1].extra =
                BitUtil::reverse(packet->touchscreen.points[1].coords[1].extra, 3);

        unsigned char *touchscreen = new unsigned char[sizeof(InputPacketTouchscreenWiiU)];
        memcpy(touchscreen, &packet->touchscreen, sizeof(InputPacketTouchscreenWiiU));
        for (int byte = 0; byte < sizeof(InputPacketTouchscreenWiiU); byte += 2) {
            unsigned char first = (unsigned char) BitUtil::reverse(touchscreen[byte], 8);
            touchscreen[byte] = (unsigned char) BitUtil::reverse(touchscreen[byte + 1], 8);
            touchscreen[byte + 1] = first;
        }
        memcpy(&packet->touchscreen, touchscreen, sizeof(InputPacketTouchscreenWiiU));
        delete [] touchscreen;
#endif
    }
    // Other
    packet->seq_id = htons(seq_id);
    packet->buttons = htons(get_button_input());
    packet->power_status = 0;
    packet->battery_charge = 0;
    packet->extra_buttons = get_extra_button_input();
    packet->stick_left_x = htons((uint16_t) (8 + floor(get_joystick_input(0) * 8)));
    packet->stick_left_y = htons((uint16_t) (8 + floor(get_joystick_input(1) * 8)));
    packet->stick_right_x = htons((uint16_t) (8 + floor(get_joystick_input(2) * 8)));
    packet->stick_right_y = htons((uint16_t) (8 + floor(get_joystick_input(3) * 8)));
    packet->audio_volume = 0;
#if __BYTE_ORDER == LITTLE_ENDIAN
    packet->accelerometer.x = 0;
    packet->accelerometer.y = 0;
    packet->accelerometer.z = 0;
    packet->gyroscope.roll = 0;
    packet->gyroscope.yaw = 0;
    packet->gyroscope.pitch = 0;
#else
    // TODO big endian
#endif
    packet->fw_version_neg = 215;

    Gamepad::sendwiiu(Gamepad::socket_hid, packet, sizeof(InputPacketHeaderWiiU), PORT_WII_HID);
    seq_id = (uint16_t) ((seq_id + 1) % 65535);
}

uint16_t Input::get_button_input() {
    if (!is_input_within_time(input_button->time))
        return 0;
    return input_button->buttons;
}

uint8_t Input::get_extra_button_input() {
    if (!is_input_within_time(input_button_extra->time))
        return 0;
    return input_button_extra->extra_buttons;
}

double Input::get_joystick_input(int id) {
    if (!is_input_within_time(input_joystick->time))
        return 0;
    // 1-100 is negative 101-200 is positive
    bool neg;
    switch (id) {
        case 0:
            neg = input_joystick->left_x <= 100;
            return (input_joystick->left_x - (neg ? 0 : 100)) / 100. * (neg ? -1 : 1);
        case 1:
            neg = input_joystick->left_y <= 100;
            return (input_joystick->left_y - (neg ? 0 : 100)) / 100. * (neg ? -1 : 1);
        case 2:
            neg = input_joystick->right_x <= 100;
            return (input_joystick->right_x - (neg ? 0 : 100)) / 100. * (neg ? -1 : 1);
        case 3:
            neg = input_joystick->right_y <= 100;
            return (input_joystick->right_y - (neg ? 0 : 100)) / 100. * (neg ? -1 : 1);
        default:
            return 0;
    }
}

// There is a 12px deadzone on all sides of the touchscreen.
void Input::get_touch_input(InputTouchpad *coords) {
    if (!is_input_within_time(input_touchpad->time))
        return;
    coords->x = translate_bounds(input_touchpad->x, 0, input_touchpad->width, 120, 4000);
    coords->y = translate_bounds(input_touchpad->y, 0, input_touchpad->height, 4000, 120);
}

bool Input::get_mic_blow_input() {
    if (!is_input_within_time(input_mic_blow->time))
        return false;
    return input_mic_blow->blow;
}

uint16_t Input::translate_bounds(int old_value, int old_min, int old_max, int new_min, int new_max) {
    return (uint16_t) floor((((old_value - old_min) * (new_max - new_min)) / (old_max - old_min)) + new_min);
}

bool Input::is_input_within_time(timespec input_time) {
    return get_delta_vs_current(input_time) <= Args::input_delay;
}

time_t Input::get_delta_vs_current(timespec _time) {
    timespec current_time;
    clock_gettime(CLOCK_MONOTONIC_RAW, &current_time);
    return (current_time.tv_sec * 1000000000 + current_time.tv_nsec) -
                   (_time.tv_sec * 1000000000 + _time.tv_nsec);
}

void Input::set_input_button(uint16_t button) {
    clock_gettime(CLOCK_MONOTONIC_RAW, &input_button->time);
    input_button->buttons = button;
}

void Input::set_input_button_extra(uint8_t button_extra) {
    clock_gettime(CLOCK_MONOTONIC_RAW, &input_button_extra->time);
    input_button_extra->extra_buttons = button_extra;
}

void Input::set_input_joystick(uint16_t left_x, uint16_t left_y, uint16_t right_x, uint16_t right_y) {
    clock_gettime(CLOCK_MONOTONIC_RAW, &input_joystick->time);
    input_joystick->left_x = left_x;
    input_joystick->left_y = left_y;
    input_joystick->right_x = right_x;
    input_joystick->right_y = right_y;
}

void Input::set_input_mic_blow(bool blow) {
    clock_gettime(CLOCK_MONOTONIC_RAW, &input_mic_blow->time);
    input_mic_blow->blow = blow;
}

void Input::set_input_touch(uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
    clock_gettime(CLOCK_MONOTONIC_RAW, &input_touchpad->time);
    input_touchpad->x = x;
    input_touchpad->y = y;
    input_touchpad->width = width;
    input_touchpad->height = height;
}
