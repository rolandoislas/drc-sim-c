//
// Created by rolando on 5/5/17.
//

#include <string>
#include "Logger.h"
#include <cstdarg>
#include <fstream>
#include <string.h>
#include <sstream>
#include <assert.h>

using namespace std;

int Logger::log_level = INFO;
const string Logger::DRC = "DRCSIM";
const string Logger::SERVER = "SERVER";
const string Logger::VIDEO = "VIDEO";
const string Logger::AUDIO = "AUDIO";
const string Logger::CONFIG = "CONFIG";

void Logger::info(const string log_name, const string message, ...) {
    va_list args;
    va_start(args, 0);
    log(log_name, INFO, message, args);
}

void Logger::log(const string log_name, const int level, const std::string message, va_list args) {
    if (is_level_enabled(level) or level == ERROR) {
        time_t cur_time;
        time(&cur_time);
        char time_string[100];
        strftime(time_string, sizeof(time_string), "%F %T", localtime(&cur_time));
        printf("%s %s:%s ", time_string, get_level_str(level), log_name.c_str());
        vprintf((message + "\n").c_str(), args);
    }
}

void Logger::set_level(int log_level) {
    Logger::log_level = log_level;
}

void Logger::debug(const string log_name, const string message, ...) {
    va_list args;
    va_start(args, 0);
    log(log_name, DEBUG, message, args);
}

void Logger::extra(const string log_name, const string message, ...) {
    va_list args;
    va_start(args, 0);
    log(log_name, EXTRA, message, args);
}

void Logger::finer(const string log_name, const string message, ...) {
    va_list args;
    va_start(args, 0);
    log(log_name, FINER, message, args);
}

void Logger::verbose(const string log_name, const string message, ...) {
    va_list args;
    va_start(args, 0);
    log(log_name, VERBOSE, message, args);
}

int Logger::get_level() {
    return log_level;
}

char *Logger::get_level_str(const int level) {
    switch (level) {
        case INFO:
            return (char *) "INFO";
        case DEBUG:
            return (char *) "DEBUG";
        case EXTRA:
            return (char *) "EXTRA";
        case FINER:
            return (char *) "FINER";
        case VERBOSE:
            return (char *) "VERBOSE";
        case ERROR:
            return (char *) "ERROR";
        default:
            return (char *) "INFO";
    }
}

void Logger::error(const string log_name, const string message, ...) {
    va_list args;
    va_start(args, 0);
    log(log_name, ERROR, message, args);
    char buf[256];
    char *error_message = strerror_r(errno, buf, 256);
    debug(log_name, error_message);
    exit(errno);
}

bool Logger::is_level_enabled(const int level) {
    return get_level() >= level;
}

const char * Logger::to_hex(unsigned char *data, size_t size) {
    stringstream hex_string;
    hex_string << hex;
    for (int byte = 0; byte < size; ++byte) {
        hex_string << (int)data[byte] << " ";
    }
    assert(strlen(hex_string.str().c_str()) >= size);
    return hex_string.str().c_str();
}
