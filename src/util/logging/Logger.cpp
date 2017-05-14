//
// Created by rolando on 5/5/17.
//

#include <string>
#include "Logger.h"
#include <cstdarg>
#include <fstream>
#include <iostream>
#include <string.h>

using namespace std;

int Logger::log_level = INFO;
const string Logger::DRC = "DRCSIM";

void Logger::info(const string log_name, const string message, ...) {
    va_list args;
    va_start(args, 0);
    log(log_name, INFO, message, args);
}

void Logger::log(const string log_name, const int log_level, const std::string message, va_list args) {
    if (get_level() >= log_level or log_level == ERROR) {
        time_t cur_time;
        time(&cur_time);
        char time_string[100];
        strftime(time_string, sizeof(time_string), "%F %T", localtime(&cur_time));
        printf("%s %s:%s ", time_string, get_level_str(log_level), log_name.c_str());
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

char *Logger::get_level_str(const int log_level) {
    switch (log_level) {
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
