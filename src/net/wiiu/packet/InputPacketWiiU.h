//
// Created by rolando on 5/27/17.
//

#ifndef DRC_SIM_C_INPUTPACKETWIIU_H
#define DRC_SIM_C_INPUTPACKETWIIU_H


#include <stdint-gcc.h>

#pragma pack(1)

typedef struct {
    // Little endian
    int16_t x;
    int16_t y;
    int16_t z;
} InputPacketAccelerometerWiiU;

typedef struct {
    // Little endian
    signed roll : 24;
    signed yaw : 24;
    signed pitch : 24;
} InputPacketGyroscopeWiiU;

typedef struct {
    signed char unknown[6];
} InputPacketMagnetWiiU;

typedef struct {
    // Big endian
    struct {
        struct {
            unsigned pad : 1;
            unsigned extra : 3;
            unsigned value : 12;
        } coords[2];
    } points[10];
} InputPacketTouchscreenWiiU;

typedef struct {
    // Big endian
    uint16_t seq_id;
    uint16_t buttons;
    uint8_t power_status;
    uint8_t battery_charge;
    uint16_t stick_left_x;
    uint16_t stick_left_y;
    uint16_t stick_right_x;
    uint16_t stick_right_y;
    uint8_t audio_volume;
    InputPacketAccelerometerWiiU accelerometer;
    InputPacketGyroscopeWiiU gyroscope;
    InputPacketMagnetWiiU magnet;
    InputPacketTouchscreenWiiU touchscreen; // byte 36 - 76
    unsigned char unknown_0[4];
    uint8_t extra_buttons;
    unsigned char unknown_1[46];
    uint8_t fw_version_neg;
} InputPacketHeaderWiiU;

#pragma pack(0)

class InputPacketWiiU {

};


#endif //DRC_SIM_C_INPUTPACKETWIIU_H
