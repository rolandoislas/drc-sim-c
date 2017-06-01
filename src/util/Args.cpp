//
// Created by rolando on 5/5/17.
//

#include <cstring>
#include "Args.h"
#include "logging/Logger.h"
#include "../data/Constants.h"

char* Args::region;
int Args::log_level;
int Args::video_quality;
int32_t Args::input_delay; // Nanoseconds
int Args::argc;
char **Args::argv;
bool Args::disable_audio;
bool Args::disable_video;

void Args::parse_args(int argc, char **argv) {
    Args::argc = argc;
    Args::argv = argv;
    // Help
    if (has_flag("-h") or has_flag("--help"))
        print_help();
    // Log Level
    log_level = Logger::INFO;
    if (has_flag("-d") or has_flag("--debug"))
        log_level = Logger::DEBUG;
    if (has_flag("-e") or has_flag("--extra"))
        log_level = Logger::EXTRA;
    if (has_flag("-f") or has_flag("--finer"))
        log_level = Logger::FINER;
    if (has_flag("-v") or has_flag("--verbose"))
        log_level = Logger::VERBOSE;
    // Region
    region = get_arg("-region", "none");
    region = strcmp(region, "none") != 0 ? region : get_arg("-r", "none");
    for (int cchar = 0; cchar < strlen(region); ++cchar)
        if (!islower(region[cchar]))
            region[cchar] = (char) tolower(region[cchar]);
    // Video
    video_quality = get_int("-video-quality", 0, 100, 75);
    disable_video = has_flag("--disable-video") or has_flag("--no-video");
    // Audio
    disable_audio = has_flag("--disable-audio") or has_flag("--no-audio");
    // Input
    input_delay = get_int("-input-delay", 0, 1000, 100); // milliseconds
    input_delay *= 1000000; // To nanoseconds

    // Log args
    Logger::info(Logger::CONFIG, "Region: %s", region);
    Logger::info(Logger::CONFIG, "Input delay: %dms", input_delay / 1000000);
    Logger::info(Logger::CONFIG, "Video quality: %d%%", video_quality);
    Logger::info(Logger::CONFIG, "Video enabled: %d", !disable_video);
    Logger::info(Logger::CONFIG, "Audio enabled: %d", !disable_audio);
}

char *Args::get_arg(const char *arg_name, const char *default_value) {
    for (int arg = 0; arg < argc; ++arg) {
        if (strcmp(argv[arg], arg_name) == 0)
            return (char *) (arg + 1 < argc ? argv[arg + 1] : default_value);
    }
    return (char *) default_value;
}

bool Args::has_flag(const char *flag_name) {
    for (int arg = 0; arg < argc; ++arg)
        if (strcmp(argv[arg], flag_name) == 0)
            return true;
    return false;
}

void Args::print_help() {
    printf("%s v%s\n", NAME.c_str(), VERSION.c_str());
    printf("Usage: drc_sim_c [args/flags]\n");

    printf("\nFlags:\n");
    printf("\t-h, --help: show this help message\n");
    printf("\t--disable-video, --no-video: disables video handling and streaming\n");
    printf("\t--disable-audio, --no-audio: disables audio handling and streaming\n");

    printf("\nOptional Args:\n");
    printf("\t-region [string]: default \"none\"\n");
    printf("\t-video-quality [0-100]: default 75\n");
    printf("\t-input-delay [0-1000]: number of milliseconds to send input from a client - default 100\n");

    printf("\nLog level:\n");
    printf("\t-d, --debug: general info\n");
    printf("\t-e, --extra: extra values\n");
    printf("\t-f, --finer: most details\n");
    printf("\t-v, --verbose: console spam\n");
    exit(0);
}

int Args::get_int(const char *arg_name, int min, int max, int default_value) {
    char *arg_str = get_arg(arg_name, "none");
    if (strcmp(arg_str, "none") == 0)
        return default_value;
    int arg = atoi(arg_str);
    return arg < min or arg > max ? default_value : arg;
}
