# gamert - game runtime
Game Runtime is a cross-platform environment to empower making game programs.

## Required Dependencies and Tools
 - Vulkan SDK
 - CMake

## Build
We'd recommand using CMake to generate _make_ or _IDE_ project files.

For example, if you're using Visual Studio 2019 on Windows platform to do develop works. The commands below may help.

    mkdir code/build && cd code/build
    cmake -G 'Visual Studio 16' ..

For Linux/Unix/Mac platfroms

    mkdir code/build && cd code/build
    cmake ..

## 3rd Parties
- [tinyxml2](https://github.com/leethomason/tinyxml2)
- [lua](https://www.lua.org/)
- [antenna](https://github.com/TankleL/antenna)

