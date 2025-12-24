cmake_minimum_required(VERSION 3.15)
project(VulkanTuto VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

# Optional: allow user to choose to compile GLSL -> SPV with glslc
option(COMPILE_SHADERS "Compile GLSL shaders to SPIR-V with glslc if available" ON)

# Collect sources (adjust/glob patterns if you prefer explicit lists)
file(GLOB_RECURSE APP_SOURCES CONFIGURE_DEPENDS
  "${CMAKE_SOURCE_DIR}/src/*.cpp")

# Exclude non-source files if needed (example)
list(FILTER APP_SOURCES EXCLUDE REGEX ".*/compile\\.bat$")

# Define executable
add_executable(${PROJECT_NAME} ${APP_SOURCES})

find_path(GLFW_INCLUDE_DIR "GLFW/glfw3.h" HINTS "D:/Documents/Visual Studio 18/Libraries/glfw-3.4.bin.WIN64/include")
find_library(GLFW_LIBRARY NAMES glfw3 glfw PATHS "D:/Documents/Visual Studio 18/Libraries/glfw-3.4.bin.WIN64/lib-vc2022")
if(GLFW_INCLUDE_DIR AND GLFW_LIBRARY)
  target_include_directories(${PROJECT_NAME} PUBLIC ${GLFW_INCLUDE_DIR})
  target_link_libraries(${PROJECT_NAME} PRIVATE ${GLFW_LIBRARY})
else()
  message(FATAL_ERROR "GLFW not found; use vcpkg or install GLFW with CMake export.")
endif()

# Find dependencies
find_package(Vulkan REQUIRED)

# Include directories (if you have headers in project root or subdirs)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Link libraries (use imported target for Vulkan)
target_link_libraries(${PROJECT_NAME} PRIVATE Vulkan::Vulkan ${GLFW_LIBRARY})

# Platform defines often required when NOT using GLFW to create surfaces;
# GLFW handles platform surface extensions, so this may be unnecessary, but keep as reference:
if(WIN32)
  target_compile_definitions(${PROJECT_NAME} PRIVATE VK_USE_PLATFORM_WIN32_KHR)
elseif(APPLE)
  target_compile_definitions(${PROJECT_NAME} PRIVATE VK_USE_PLATFORM_MACOS_MVK)
endif()

# Put runtime binary in a predictable place
set_target_properties(${PROJECT_NAME} PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Optional: copy shader files (or compile with glslc -> .spv)
file(GLOB SHADER_FILES "${CMAKE_SOURCE_DIR}/shaders/*.vert" "${CMAKE_SOURCE_DIR}/shaders/*.frag")

# If glslc is available and COMPILE_SHADERS ON, compile .vert/.frag -> .spv
if(COMPILE_SHADERS)
  find_program(GLSLC_EXECUTABLE glslc)
  if(GLSLC_EXECUTABLE)
    foreach(SHADER IN LISTS SHADER_FILES)
      get_filename_component(SHADER_NAME ${SHADER} NAME)
      set(SPV "${CMAKE_BINARY_DIR}/shaders/${SHADER_NAME}.spv")
      add_custom_command(
        OUTPUT ${SPV}
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/shaders"
        COMMAND ${GLSLC_EXECUTABLE} ${SHADER} -o ${SPV}
        DEPENDS ${SHADER}
        COMMENT "Compiling shader ${SHADER_NAME}"
        VERBATIM
      )
      list(APPEND COMPILED_SHADERS ${SPV})
    endforeach()
    add_custom_target(shaders ALL DEPENDS ${COMPILED_SHADERS})
    add_dependencies(${PROJECT_NAME} shaders)
  else()
    # Fallback: copy raw GLSL to runtime directory so app can compile/load at runtime if supported
    foreach(SHADER IN LISTS SHADER_FILES)
      add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SHADER}" "${CMAKE_BINARY_DIR}/bin/${CMAKE_CFG_INTDIR}/"
      )
    endforeach()
  endif()
else()
  # Copy shader sources to runtime dir so they'll be available when running
  foreach(SHADER IN LISTS SHADER_FILES)
    add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${SHADER}" "${CMAKE_BINARY_DIR}/bin/${CMAKE_CFG_INTDIR}/"
    )
  endforeach()
endif()

# Useful hints:
# - To use vcpkg: cmake -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake ..
# - To set build type on single-config generators: -DCMAKE_BUILD_TYPE=Release
# - Adjust shader handling to match your runtime shader loader expectations.