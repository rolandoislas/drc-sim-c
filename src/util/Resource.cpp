//
// Created by rolando on 5/25/17.
//

#include <cstdarg>
#include <sstream>
#include "Resource.h"
#include "OSUtil.h"
#include "logging/Logger.h"
#include "../data/Constants.h"

using namespace std;

/**
 * Open a read a resource file from the local dir or global location.
 * @param ... Relative path to resource file
 */
Resource::Resource(const char* rel_path, bool throw_error) {
    init(rel_path, throw_error);
}

Resource::Resource(const char *rel_path) {
    init(rel_path, true);
}

void Resource::init(const char *rel_path, bool throw_error) {
    string path_prefix = "./resources/";
    path = path_prefix + string(rel_path);
    if (OSUtil::exists(path.c_str())) {
        Logger::debug(Logger::DRC, "Found local resource: %s", path.c_str());
    }
    else {
        path_prefix = PATH_INSTALL + "resources/";
        path = path_prefix + string(rel_path);
        if (OSUtil::exists(path.c_str())) {
            Logger::debug(Logger::DRC, "Found global resource: %s", path.c_str());
        }
        else if (throw_error)
            Logger::error(Logger::DRC, "Failed to load resource: %s", path.c_str());
        else
            Logger::info(Logger::DRC, "Failed to load resource: %s", path.c_str());
    }
}

int Resource::as_bytes(char *buffer) {
    FILE *resource = fopen(path.c_str(), "rb");
    if (resource == NULL)
        Logger::error(Logger::DRC, "Failed to open resource for reading: %s", path.c_str());

    fseek (resource , 0 , SEEK_END);
    long size = ftell(resource);
    rewind(resource);

    size_t read_size = fread(buffer, 1, (size_t) size, resource);
    if (read_size != size)
        Logger::error(Logger::DRC, "Could not read resource: %s", path.c_str());
    fclose(resource);
    return (int) size;
}

bool Resource::exists() {
    return OSUtil::exists(path.c_str());
}

