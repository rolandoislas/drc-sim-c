//
// Created by rolando on 5/5/17.
//

#include <sys/stat.h>
#include "OSUtil.h"

void OSUtil::log_sys_info() {

}

bool OSUtil::exists(const char *path) {
    struct stat file_info;
    if (stat(path, &file_info) != -1)
        return S_ISDIR(file_info.st_mode) or S_ISREG(file_info.st_mode) or S_ISLNK(file_info.st_mode);
}
