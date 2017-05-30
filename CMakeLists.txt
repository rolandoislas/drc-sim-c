cmake_minimum_required(VERSION 2.8.12.2)
project(drc_sim_c)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
find_package(FFmpeg)
find_package(Threads REQUIRED)
find_package(JPEG REQUIRED)

include_directories(${AVCODEC_INCLUDE_DIRS} ${SWSCALE_INCLUDE_DIRS} ${FFMPEG_INCLUDE_DIR} ${JPEG_INCLUDE_DIR} include)
add_definitions(${AVCODEC_DEFINITIONS} ${SWSCALE_DEFINITIONS} ${FFMPEG_DEFINITIONS})

set(SOURCE_FILES src/main.cpp src/util/Args.cpp src/util/Args.h src/data/Config.cpp src/data/Config.h
        src/util/logging/Logger.cpp src/util/logging/Logger.h src/util/OSUtil.cpp src/util/OSUtil.h src/data/Constants.h
        src/Gamepad.cpp src/Gamepad.h src/net/wiiu/VideoHandlerWiiU.cpp src/net/wiiu/VideoHandlerWiiU.h
        src/net/wiiu/packet/VideoPacketWiiU.cpp src/net/wiiu/packet/VideoPacketWiiU.h src/util/BitUtil.cpp
        src/util/BitUtil.h src/net/wiiu/CommandHandlerWiiU.cpp src/net/wiiu/CommandHandlerWiiU.h
        src/net/PacketHandler.cpp src/net/PacketHandler.h src/data/H264Decoder.cpp src/data/H264Decoder.h src/Server.cpp
        src/Server.h src/net/wiiu/AudioHandlerWiiU.cpp src/net/wiiu/AudioHandlerWiiU.h src/net/wiiu/HidHandlerWiiU.cpp
        src/net/wiiu/HidHandlerWiiU.h src/net/wiiu/MessageHandlerWiiU.cpp src/net/wiiu/MessageHandlerWiiU.h
        src/net/server/CommandHandlerServer.cpp src/net/server/CommandHandlerServer.h
        src/net/server/packet/CommandPacketServer.cpp src/net/server/packet/CommandPacketServer.h src/net/Packet.cpp
        src/net/Packet.h src/util/ImageUtil.cpp src/util/ImageUtil.h src/net/wiiu/packet/CommandPacketWiiU.cpp
        src/net/wiiu/packet/CommandPacketWiiU.h src/util/Resource.cpp src/util/Resource.h
        src/net/wiiu/packet/AudioPacketWiiU.cpp src/net/wiiu/packet/AudioPacketWiiU.h src/Input.cpp src/Input.h
        src/net/wiiu/packet/InputPacketWiiU.cpp src/net/wiiu/packet/InputPacketWiiU.h)

list(APPEND SOURCE_FILES include/json.c include/json.h)

add_executable(drc_sim_c ${SOURCE_FILES})

target_link_libraries(drc_sim_c ${AVCODEC_LIBRARIES} ${SWSCALE_LIBRARIES} ${FFMPEG_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT}
        ${JPEG_LIBRARIES})

install(TARGETS drc_sim_c RUNTIME DESTINATION /usr/local/bin/)
install(DIRECTORY resources DESTINATION /opt/drc_sim)