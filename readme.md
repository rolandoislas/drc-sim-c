# DRC Sim Server

A version of [DRC SIM Server] written in C++.

It handles parses packets and serves clients.

This does not handle connecting to the Wii U. For that see [DRC SIM Server].

# Building

Cmake is used as the build system.

Build dependencies include `libavcodec-dev`, `libswscale-dev`, and `libjpeg-dev`.
 If the Python version is installed, these dependencies are likely to be met.

```
cmake .
make
sudo make install
```

# Running

Usage: `drc_sim_c [-h]`
 
# Credits

[drc-sim] \(original\) by [memahaxx]
- The original Python codebase

[libdrc documentation] by memahaxx
- Gamepad and Wii U software and hardware details

# Additional Software

[json-parser] JSON parser

[libjpeg] JPEG image library

[libav] Video and audio processing library




[DRC SIM Server]: https://github.com/rolandoislas/drc-sim
[connecting]: https://github.com/rolandoislas/drc-sim/wiki/Connecting
[drc-sim]: https://bitbucket.org/memahaxx/drc-sim
[memahaxx]: https://bitbucket.org/memahaxx/
[libdrc documentation]: http://libdrc.org/docs/index.html
[json-parser]: https://github.com/udp/json-parser
[libjpeg]: http://ijg.org/
[libav]: https://libav.org/