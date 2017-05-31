//
// Created by rolando on 5/5/17.
//

#ifndef DRC_SIM_C_ARGS_H
#define DRC_SIM_C_ARGS_H


#include <cstdint>

class Args {

public:
    static void parse_args(int argc, char **argv);

    static char *region;
    static int log_level;
    static int video_quality;
    static int32_t input_delay;

    static char *get_arg(const char *arg_name, const char *default_value);

    static bool disable_audio;
    static bool disable_video;
private:
    static int argc;
    static char **argv;

    static bool has_flag(const char *flag_name);

    static void print_help();

    static int get_int(const char *arg_name, int min, int max, int default_value);

};


#endif //DRC_SIM_C_ARGS_H
