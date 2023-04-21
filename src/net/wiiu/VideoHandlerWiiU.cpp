//
// Created by rolando on 5/8/17.
//

#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <arpa/inet.h>
#include <thread>
#include <unistd.h>
#include "VideoHandlerWiiU.h"
#include "../../util/logging/Logger.h"
#include "../../Server.h"
#include "../../util/ImageUtil.h"
#include "../../Gamepad.h"
#include "../../data/Constants.h"

using namespace std;

VideoHandlerWiiU::VideoHandlerWiiU() {
    frame_index = 0;
    frame_decode_num = 0;
}

void VideoHandlerWiiU::update(unsigned char *packet, size_t packet_size, sockaddr_in *from_address,
                          unsigned int *address_size) {
    const VideoPacketWiiU &video_packet = VideoPacketWiiU(packet, packet_size);
    bool is_idr = is_idr_packet(video_packet.header);

    bool seq_ok = update_seq_id(video_packet.header->seq_id);

    if (!seq_ok)
        is_streaming = false;

    if (video_packet.header->frame_begin) {
        frame_index = 0;
        if (!is_streaming) {
            if (is_idr)
                is_streaming = true;
            else {
                static const unsigned char idr_request[] = {1, 0, 0, 0}; // Undocumented
                Gamepad::sendwiiu(Gamepad::socket_msg, idr_request, sizeof(idr_request), PORT_WII_MSG);
                return;
            }
        }
    }

    memcpy(frame + frame_index, video_packet.header->payload, video_packet.header->payload_size);
    frame_index += video_packet.header->payload_size;

    if (video_packet.header->frame_end) {
        if (is_streaming) {
            size_t size = h264_nal_encapsulate(is_idr, frame, frame_index);
            size = decoder.image(nals, size, av_buf);
            if (size) {
                size = imgUtil.rgb_to_jpeg(av_buf, image_buf, sizeof(image_buf));
                if (size)
                    Server::broadcast_video(image_buf, size);
            }
        }
        else {
            Logger::debug(Logger::VIDEO, "Skipping video frame");
        }
    }
}

bool VideoHandlerWiiU::is_idr_packet(VideoPacketHeaderWiiU *header) {
    uint8_t *ptr = header->extended_header;
    while (ptr < header->extended_header + sizeof(header->extended_header)) {
        if (*ptr == 0x80)
            return true;
        ++ptr;

    }
    return false;
}

size_t VideoHandlerWiiU::h264_nal_encapsulate(bool is_idr, uint8_t *frame, size_t frame_size) {
    int slice_header = is_idr ? 0x25b804ff : (0x21e003ff | ((frame_decode_num & 0xff) << 13));
    frame_decode_num++;

    uint8_t *na = nals;
    if (is_idr) {
        static const uint8_t params[] = {
            // sps
            0x00, 0x00, 0x00, 0x01, 0x67, 0x64, 0x00, 0x20, 0xac, 0x2b, 0x40, 0x6c, 0x1e, 0xf3, 0x68,
            // pps
            0x00, 0x00, 0x00, 0x01, 0x68, 0xee, 0x06, 0x0c, 0xe8
        };
        memcpy(na, params, sizeof(params));
        na += sizeof(params);
    }

    // begin slice nalu
    static const uint8_t slice[] = {0x00, 0x00, 0x00, 0x01,
                       (uint8_t) ((slice_header >> 24) & 0xff),
                       (uint8_t) ((slice_header >> 16) & 0xff),
                       (uint8_t) ((slice_header >> 8) & 0xff),
                       (uint8_t) (slice_header & 0xff)
    };
    memcpy(na, slice, sizeof(slice));
    na += sizeof(slice);

    // Frame
    memcpy(na, frame, 2);
    na += 2;

    // Escape codes
    for (int byte = 2; byte < frame_size; ++byte) {
        if (frame[byte] <= 3 and *(na - 2) == 0 and *(na - 1) == 0)
            *(na++) = 3;
        *(na++) = frame[byte];
    }
    return na - nals;
}
