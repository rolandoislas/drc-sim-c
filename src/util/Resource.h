//
// Created by rolando on 5/25/17.
//

#ifndef DRC_SIM_C_RESOURCE_H
#define DRC_SIM_C_RESOURCE_H


class Resource {

public:
    Resource(const char* rel_path);

    int as_bytes(char *buffer);

    std::string path;
};


#endif //DRC_SIM_C_RESOURCE_H
