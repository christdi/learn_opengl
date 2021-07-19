# Learn OpenGL

This is my personal repository for following the lessons at learnopengl.com

### Prequisites

This project requires the following tools:

  * [Microsoft Visual Studio](https://visualstudio.microsoft.com)
  * [CMake](https://cmake.org)
  * [Conan](https://conan.io)

### Structure

Conan is configured to download the `glfw3` library required for the lessons.

CMake is configured to build a library named `myopengl` from the `src` directory.  This library will capture all shared functionality between individual exmaples.

Individual examples can be found in the `example` directory.

### Compilation

First, create the Visual Studio solution file.

```
# Create build directory
mkdir build

# Move to build directory
cd build

# Obtain dependencies via conan
conan install ..

# Build solution files
cmake ..
```

Secondly, open the solution file `learnopengl.sln` in `build` directory and then run desired example.