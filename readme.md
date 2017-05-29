# DRC Sim Server

A version of [DRC SIM Server](https://github.com/rolandoislas/drc-sim) written in C++.

This is a work in progress. Not all features from the Python version are implemented.


# Requirements

[DRC Sim Client](https://github.com/rolandoislas/drc-sim-client/tree/c-compat)
 and 
 [DRC Sim Server](https://github.com/rolandoislas/drc-sim/tree/c-compat)
 have `c-compat` branches. These branches offer experimental builds meant to work
 with the C++ version of DRC Sim Server.

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

Follow the Python instructions for [connecting](https://github.com/rolandoislas/drc-sim/wiki/Connecting).
 DRC Sim Server C++ is currently not launched by the Python GUI. Launch `drc_sim_c` manually.
 The client will connect normally.