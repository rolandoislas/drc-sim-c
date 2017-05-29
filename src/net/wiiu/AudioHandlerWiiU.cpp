//
// Created by rolando on 5/17/17.
//

#include "AudioHandlerWiiU.h"
#include "../../Server.h"
#include "packet/AudioPacketWiiU.h"
#include "../../util/logging/Logger.h"
#include "../server/packet/CommandPacketServer.h"

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
}
