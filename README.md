
# Ryme

A C++/Python Vulkan Game & Simulation Library

Spiritual fork of [https://github.com/WhoBrokeTheBuild/Dusk] and [https://github.com/benjinx/Toon].

## Building & Running

First install any missing dependencies:
 * A C++20 compliant compiler (VS 2019, GCC 8, Clang 10)
 * CMake >= 3.19 ([https://cmake.org/download/])
 * Vulkan SDK >= 1.2 ([https://vulkan.lunarg.com/sdk/home])
 * Python >= 3.8 ([https://www.python.org/downloads/])
   * Note: Do not install the Debug Libraries on Windows

The remaining dependencies will be configured automatically:
 * GLM
 * fmt
 * SDL2
 * pybind11
 * GTest

```
mkdir build
cd build
cmake ..
cmake --build .
```

If you are using Visual Studio Code or Visual Studio, launch targets will be generated automatically for each demo.