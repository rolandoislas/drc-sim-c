//
// Created by rolando on 5/25/17.
//

#ifndef DRC_SIM_C_RESOURCE_H
#define DRC_SIM_C_RESOURCE_H


class Resource {

public:
    Resource(const char* rel_path);

    Resource(const char *rel_path, bool throw_error);

    int as_bytes(char *buffer);

    std::string path;

    void init(const char *rel_path, bool throw_error);

    bool exists();
};


#endif //DRC_SIM_C_RESOURCE_H
