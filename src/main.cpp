#include <iostream>
#include <thread>
#include "util/Args.h"
#include "data/Config.h"
#include "util/OSUtil.h"
#include "util/logging/Logger.h"
#include "data/Constants.h"
#include "Gamepad.h"
#include "Server.h"
#include "Input.h"

void initialize_loggers();

void log_start_info();

void log_level();

void run();

using namespace std;

int main(int argc, char **args) {
    Args::parse_args((char *) args);
    Config::init();
    initialize_loggers();
    log_start_info();
    log_level();
    OSUtil::log_sys_info();
    run();
    return 0;
}

void run() {
    thread gamepad_thread(Gamepad::run);
    thread server_thread(Server::run);
    thread input_thread(Input::run);
    gamepad_thread.join();
    server_thread.join();
    input_thread.join();
}

void log_level() {
    Logger::debug(Logger::DRC, "Debug logging enabled");
    Logger::extra(Logger::DRC, "Extra debug logging enabled");
    Logger::finer(Logger::DRC, "Finer debug logging enabled");
    Logger::verbose(Logger::DRC, "Verbose logging enabled");
}

void log_start_info() {
    Logger::info(Logger::DRC, "Initializing %s version %s", NAME.c_str(), VERSION.c_str());
    Logger::info(Logger::DRC, "Using \"%s\" as home folder.", PATH_USER_DATA.c_str());
}

void initialize_loggers() {
    Logger::set_level(Logger::FINER);
}