//
// Created by rolando on 5/27/17.
//

#ifndef DRC_SIM_C_INPUT_H
#define DRC_SIM_C_INPUT_H


#include <tgmath.h>
#include <cstdint>
#include "net/wiiu/packet/InputPacketWiiU.h"

typedef struct {
    uint16_t buttons;
    timespec time;
} InputButton;

typedef struct {
    uint8_t extra_buttons;
    timespec time;
} InputButtonExtra;

typedef struct {
    uint16_t left_x;
    uint16_t left_y;
    uint16_t right_x;
    uint16_t right_y;
    timespec time;
} InputJoystick;

typedef struct {
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
    timespec time;
} InputTouchpad;

typedef struct {
    bool blow;
    timespec time;
} InputMicBlow;

class Input {

public:
    Input();

    static void run();

    void update();

    timespec last_sent_time;

    double HID_UPDATE_INTERVAL = 1000000000. / 180.; // Nanoseconds

    void send_hid_update();

    uint16_t seq_id = 0;

    uint16_t get_button_input();

    uint8_t get_extra_button_input();

    double get_joystick_input(int id);

    bool running;

    InputPacketHeaderWiiU *packet;

    void get_touch_input(InputTouchpad *coords);

    uint16_t translate_bounds(int old_value, int old_min, int old_max, int new_min, int new_max);
    static InputButton *input_button;
    static InputButtonExtra *input_button_extra;
    static InputJoystick *input_joystick;
    static InputTouchpad *input_touchpad;
    static InputMicBlow *input_mic_blow;

    bool is_input_within_time(timespec input_time);

    time_t get_delta_vs_current(timespec _time);

    static void set_input_button(uint16_t button);

    static void set_input_button_extra(uint8_t button_extra);

    static void set_input_joystick(uint16_t left_x, uint16_t left_y, uint16_t right_x, uint16_t right_y);

    static void set_input_mic_blow(bool blow);

    static void set_input_touch(uint16_t x, uint16_t y, uint16_t width, uint16_t height);
};


#endif //DRC_SIM_C_INPUT_H
