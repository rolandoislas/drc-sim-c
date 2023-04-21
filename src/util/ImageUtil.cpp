//
// Created by rolando on 5/22/17.
//

#include <cstdint>
#include <cstring>
#include "ImageUtil.h"
#include "../data/Constants.h"
#include "Args.h"

extern "C" {
    #include <turbojpeg.h>
}

ImageUtil::ImageUtil() {
    handle = tjInitCompress();
}

ImageUtil::~ImageUtil() {
    tjDestroy(handle);
}

size_t ImageUtil::rgb_to_jpeg(uint8_t *rgb, uint8_t *out, size_t outSize) {
    tjCompressFromYUV(handle, rgb, WII_VIDEO_WIDTH, 4, WII_VIDEO_HEIGHT, TJSAMP_420, &out, &outSize, Args::video_quality, TJFLAG_FASTDCT | TJFLAG_NOREALLOC);
    return outSize;
}
