# Vulkan Stuff

A cross-platform Vulkan graphics application built following the **Khronos Group** [Vulkan Tutorial](https://vulkan-tutorial.com/).

This project demonstrates core Vulkan concepts including surface creation, swap chains, graphics pipelines, framebuffers, and command buffers to render a simple triangle.

## Platform Support

- **Windows** (MSVC x64) ✅ Tested
- **Linux** (GCC/Clang) ✅ Tested
- **macOS** (Clang) ⚠️ Supported (requires Metal support)

## Project Structure

```
VulkanStuff/
├── src/                           # Source implementation files
│   ├── main.cpp                   # Application entry point
│   ├── TriangleApp.cpp            # Main application logic & render loop
│   ├── Instance.cpp               # Vulkan instance creation
│   ├── Device.cpp                 # Physical/logical device selection
│   ├── Surface.cpp                # Window surface creation
│   ├── SwapChain.cpp              # Swap chain management
│   ├── ImageViews.cpp             # Image view creation
│   ├── GraphicsPipeline.cpp       # Graphics pipeline creation
│   ├── Framebuffer.cpp            # Framebuffer setup
│   ├── Command.cpp                # Command buffer recording
│   ├── Queue.cpp                  # Queue management
│   ├── Buffer.cpp                 # Vertex/index buffer management
│   ├── Synchronisation.cpp        # Synchronization (semaphores, fences)
│   └── ValidationLayers.cpp       # Debug validation layer setup
├── include/                       # Header files
│   ├── TriangleApp.hpp            # Main application header
│   ├── Instance.hpp               # Instance management
│   ├── Device.hpp                 # Device management
│   ├── Surface.hpp                # Surface management
│   ├── SwapChain.hpp              # Swap chain management
│   ├── ImageViews.hpp             # Image view management
│   ├── GraphicsPipeline.hpp       # Graphics pipeline
│   ├── Framebuffer.hpp            # Framebuffer management
│   ├── Command.hpp                # Command buffer management
│   ├── Queue.hpp                  # Queue management
│   ├── Buffer.hpp                 # Buffer management
│   ├── Synchronisation.hpp        # Synchronization primitives
│   ├── ValidationLayers.hpp       # Validation layer handling
│   ├── FrameSize.hpp              # Frame size constants
│   └── helper.hpp                 # Utility functions
├── shaders/                       # GLSL shader sources
│   ├── shader.vert                # Vertex shader
│   └── shader.frag                # Fragment shader
├── build/                         # Build directory (generated)
│   ├── bin/                       # Compiled executable
│   │   └── VulkanTuto             # Main application binary
│   ├── shaders/                   # Compiled SPIR-V shaders
│   │   ├── shader.vert.spv        # Compiled vertex shader
│   │   └── shader.frag.spv        # Compiled fragment shader
│   ├── CMakeFiles/                # CMake generated files
│   └── compile_commands.json      # Compilation database
├── CMakeLists.txt                 # Build configuration
└── README.md                      # This file
```

## Dependencies

### Required
- **Vulkan SDK** (latest) - Graphics API
- **CMake** (3.10+) - Build system
- **C++ Compiler** (C++17 or later)
  - Windows: MSVC, Clang, or MinGW
  - Linux: GCC or Clang
  - macOS: Clang

### External Libraries
- **GLFW3** (3.4+) - Window management
- **glslc** (Vulkan SDK) - Shader compilation
- **GLM** (0.9.9+) - Math library (automatically fetched by CMake)

### Windows
- **Vulkan SDK** (latest)
- **GLFW3** (3.4+)
  - Download from: https://www.glfw.org/download.html
  - Or use vcpkg: `vcpkg install glfw3:x64-windows`
- **glslc** (Vulkan SDK) - Shader compilation

### Linux (Ubuntu/Debian)
```bash
sudo apt-get install vulkan-tools libvulkan-dev libglfw3-dev glslc cmake build-essential
```

### Linux (Fedora/RHEL)
```bash
sudo dnf install vulkan-tools vulkan-devel glfw-devel glslc cmake gcc-c++
```

### macOS
```bash
brew install vulkan-loader glfw glslc cmake
```

## Building

### Quick Start

```bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
./bin/VulkanTuto
```

### Windows

```powershell
mkdir build
cd build
cmake -G "Visual Studio 17 2022" ..
cmake --build . --config Release
.\bin\Release\VulkanTuto.exe
```

### Linux

```bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
./bin/VulkanTuto
```

### macOS

```bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
./bin/VulkanTuto
```

## Build Options

- **COMPILE_SHADERS** (ON by default)
  - Automatically compiles GLSL shaders to SPIR-V using `glslc`
  - If disabled, raw GLSL files are copied to the output directory

```bash
cmake -DCOMPILE_SHADERS=OFF ..
cmake --build .
```

- **CMAKE_BUILD_TYPE** 
  - `Release` - Optimized production build
  - `Debug` - Development build with debug symbols

## Configuration

### GLFW Library Paths (Windows)

If CMake cannot find GLFW, edit the paths in `CMakeLists.txt`:

```cmake
find_path(GLFW_INCLUDE_DIR "GLFW/glfw3.h" HINTS "C:/path/to/glfw/include")
find_library(GLFW_LIBRARY NAMES glfw3 glfw PATHS "C:/path/to/glfw/lib")
```

Alternatively, use vcpkg for automatic dependency management.

## Core Components

### TriangleApp
Main application class managing the rendering loop and Vulkan initialization.

### Instance & Device
Creates Vulkan instance and selects appropriate GPU with queue families.

### Surface & SwapChain
Manages window surface and presents rendered frames to screen.

### GraphicsPipeline
Defines the graphics rendering pipeline with shaders and fixed-function stages.

### Framebuffer & ImageViews
Manages framebuffer attachments for rendering targets.

### Command & Queue
Records and submits rendering commands to the GPU.

### Synchronisation
Handles semaphores and fences for frame synchronization.

### ValidationLayers
Enables Vulkan validation for debugging (requires Vulkan SDK).

## Shaders

Shaders are written in GLSL and automatically compiled to SPIR-V:

- **shader.vert** - Vertex shader (triangle vertices)
- **shader.frag** - Fragment shader (color output)

Compiled shaders are stored in `build/shaders/` directory.

## Troubleshooting

### GLFW not found (Windows)
- Ensure GLFW is installed and paths in CMakeLists.txt are correct
- Or use vcpkg: `vcpkg install glfw3:x64-windows`

### Missing Vulkan headers (Linux)
```bash
sudo apt-get install vulkan-headers
```

### Validation layer warnings
These are normal and help debug Vulkan issues. Disable with `ENABLE_VALIDATION_LAYERS = false` in code if needed.

### Shader compilation fails
Ensure `glslc` is in your PATH:
- Windows: Install from Vulkan SDK
- Linux: `sudo apt-get install glslc`
- macOS: `brew install glslc`

## References

- [Vulkan Tutorial](https://vulkan-tutorial.com/) - Official learning resource
- [Vulkan Specification](https://www.khronos.org/vulkan/)
- [GLFW Documentation](https://www.glfw.org/documentation.html)
- [Vulkan SDK](https://vulkan.lunarg.com/)

## License

Educational project based on Khronos Group tutorials.