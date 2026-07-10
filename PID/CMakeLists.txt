cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME PID)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

# ========== 方案A：手动逐个添加Hardware源文件（推荐，稳定可控） ==========
# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
    

    # ===== 新增：Hardware文件夹下的所有.c文件，有几个加几个 =====
    Hardware/key.c
    Hardware/oled.c
    Hardware/oled_data.c
    Hardware/iic.c
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
    # ===== 新增：Hardware头文件搜索路径 =====
    Hardware
)

# ========== 方案B：自动扫描Hardware下所有.c（文件多时用，二选一） ==========
# 如果你想用自动通配，把上面target_sources替换成下面这段：
# file(GLOB HARDWARE_SOURCES "Hardware/*.c")
# target_sources(${CMAKE_PROJECT_NAME} PRIVATE
#     Core/Src/oled.c
#     Core/Src/oled_data.c
#     Core/Src/key.c
#     ${HARDWARE_SOURCES}
# )
# target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
#     Hardware
# )

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx

    # Add user defined libraries
)