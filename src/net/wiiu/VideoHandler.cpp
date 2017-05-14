//
// Created by rolando on 5/8/17.
//

#include <fstream>
#include "packet/VideoPacket.h"
#include "VideoHandler.h"

using namespace std;

void VideoHandler::update(unsigned char *packet, size_t size) {
    const VideoPacket &video_packet = VideoPacket(packet, size);
}
