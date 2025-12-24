# Vulkan Stuff

A cross-platform Vulkan graphics application built following the **Khronos Group** [Vulkan Tutorial](https://vulkan-tutorial.com/).

This project demonstrates core Vulkan concepts including surface creation, swap chains, graphics pipelines, framebuffers, and command buffers to render a simple triangle.

## Platform Support

- **Windows** (MSVC x64) ✅ Tested
- **Linux** (GCC/Clang) ⚠️ Not tested
- **macOS** (Clang) ⚠️ Supported (requires Metal support)

## Project Structure

```
VulkanTuto/
├── src/                    # Source files
│   ├── main.cpp           # Application entry point
│   ├── TriangleApp.cpp    # Main application logic
│   ├── Instance.cpp       # Vulkan instance creation
│   ├── Device.cpp         # Physical/logical device selection
│   ├── Surface.cpp        # Window surface creation
│   ├── SwapChain.cpp      # Swap chain management
│   ├── GraphicsPipeline.cpp # Pipeline creation
│   ├── Framebuffer.cpp    # Framebuffer setup
│   ├── Command.cpp        # Command buffer recording
│   ├── Queue.cpp          # Queue management
│   ├── Synchronisation.cpp # Synchronization primitives
│   └── ValidationLayers.cpp # Debug validation
├── include/               # Header files (corresponding to src/)
├── shaders/               # GLSL shader sources
│   ├── shader.vert        # Vertex shader
│   └── shader.frag        # Fragment shader
├── CMakeLists.txt         # Build configuration
└── README.md             # This file
```

## Dependencies

### Windows
- **Vulkan SDK** (latest)
- **GLFW3** (3.4+) - Window management
  - Download from: https://www.glfw.org/download.html
  - Or use vcpkg: `vcpkg install glfw3:x64-windows`
- **glslc** (Vulkan SDK) - Shader compilation

### Linux (Ubuntu/Debian)
```bash
sudo apt-get install vulkan-tools libvulkan-dev libglfw3-dev glslc
```

### Linux (Fedora/RHEL)
```bash
sudo dnf install vulkan-tools vulkan-devel glfw-devel glslc
```

### macOS
```bash
brew install vulkan-loader glfw glslc
```

## Building

### Windows

```powershell
mkdir build-win
cd build-win
cmake -G "Visual Studio 17 2022" ..
cmake --build . --config Release
./bin/Release/VulkanTuto.exe
```

### Linux

```bash
mkdir build-linux
cd build-linux
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
./bin/VulkanTuto
```

### macOS

```bash
mkdir build-macos
cd build-macos
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
```

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