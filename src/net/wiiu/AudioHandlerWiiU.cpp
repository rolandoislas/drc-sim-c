//
// Created by rolando on 5/17/17.
//

#include <cstring>
#include <arpa/inet.h>
#include "AudioHandlerWiiU.h"
#include "../../Server.h"
#include "packet/AudioPacketWiiU.h"
#include "../../util/logging/Logger.h"
#include "../../util/BitUtil.h"
#include "../../Gamepad.h"
#include "../../data/Constants.h"
#include "../../Input.h"

AudioHandlerWiiU::AudioHandlerWiiU() {
    rand_audio = new unsigned char[512];
    for (int byte = 0; byte < 512; ++byte)
      rand_audio[byte] = (unsigned char) (rand() % 255);
}

void AudioHandlerWiiU::update(unsigned char *packet, size_t packet_size, sockaddr_in *from_address,
                          unsigned int *address_size) {
    const AudioPacketWiiU &audio_packet = AudioPacketWiiU(packet, packet_size);
    if (audio_packet.header->type != audio_packet.TYPE_AUDIO)
        return;
    bool seq_ok = update_seq_id(audio_packet.header->seq_id);
    if (!seq_ok)
        Logger::debug(Logger::AUDIO, "Audio is lagging");
    if (audio_packet.header->format != 1 or audio_packet.header->mono) {
        Logger::debug(Logger::AUDIO, "Skipping audio packet: format is not 48kHz stereo.");
        return;
    }

    if (audio_packet.header->vibrate)
        Server::broadcast_command(CommandPacketServer::COMMAND_INPUT_VIBRATE);

    Server::broadcast_audio(audio_packet.header->payload, audio_packet.header->payload_size);

    if (Input::get_mic_blow_input())
        send_mic_blow();
}

void AudioHandlerWiiU::send_mic_blow() {
    AudioPacketHeaderWiiU packet;
    packet.format = 6;
    packet.mono = 1;
    packet.vibrate = false;
    packet.type = 0;
    packet.seq_id = (unsigned int) seq_id_expected;
    packet.payload_size = 512;
    packet.timestamp = 0;
    memcpy(packet.payload, rand_audio, 512);
    int size = sizeof(AudioPacketHeaderWiiU) - sizeof(AudioPacketHeaderWiiU::payload);
    int payload_size = packet.payload_size;
#if __BYTE_ORDER == LITTLE_ENDIAN
    packet.format = BitUtil::reverse(packet.format, 3);
    packet.payload_size = BitUtil::reverse(packet.payload_size, 16);
    packet.seq_id = ((packet.seq_id & 0xff) << 2) | ((((packet.seq_id >> 8) << 1) | (packet.seq_id >> 9)) & 0x3);

    unsigned char *header = new unsigned char[size];
    memcpy(header, &packet, (size_t) size);
    for (int byte = 0; byte < size; byte += 2)
        header[byte] = (unsigned char) BitUtil::reverse(header[byte], 8);
    memcpy(&packet, header, (size_t) size);
    delete [] header;
#endif

    Gamepad::sendwiiu(Gamepad::socket_aud, &packet, (size_t) (size + payload_size), PORT_WII_AUD);
}
