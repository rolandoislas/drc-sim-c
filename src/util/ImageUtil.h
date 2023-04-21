//
// Created by rolando on 5/22/17.
//

#ifndef DRC_SIM_C_IMAGEUTIL_H
#define DRC_SIM_C_IMAGEUTIL_H

extern "C" {
    #include <turbojpeg.h>
}

class ImageUtil {

public:
    ImageUtil();
    ~ImageUtil();
    size_t rgb_to_jpeg(uint8_t *rgb, uint8_t *out, size_t outSize);

private:
    tjhandle handle;
};


#endif //DRC_SIM_C_IMAGEUTIL_H
