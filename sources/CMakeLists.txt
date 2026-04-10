cmake_minimum_required(VERSION 3.15)

# Set CMake policy required by UrhoCommon module
if (COMMAND cmake_policy)
    # Libraries linked via full path no longer produce linker search paths
    cmake_policy (SET CMP0003 NEW)
    # INTERFACE_LINK_LIBRARIES defines the link interface
    cmake_policy (SET CMP0022 NEW)
#    # Disallow use of the LOCATION target property - so we set to OLD as we still need it
#    cmake_policy (SET CMP0026 OLD)
    # MACOSX_RPATH is enabled by default
    cmake_policy (SET CMP0042 NEW)
#    # Honor the visibility properties for SHARED target types only
#    cmake_policy (SET CMP0063 OLD)
endif ()

# Set project name
project (Sunset)

set (TARGET_NAME Sunset)

set(CMAKE_UNITY_BUILD ON)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")         # Установка флага линковки /MT

if(WIN32)
else()
    add_definitions(
        -Wall
        -Werror
        -fmax-errors=1
        -Wno-unknown-pragmas
        -Wno-write-strings
        -pipe

#  Без оптимизации - лучше для отладки
#      -g3                       # Без этого Code Blocks не останавливается на точках останова
#      -O0

# Максимальная оптимизация - релизная версия
       -Ofast
#       -flto   #  включает Link-Time Optimization (оптимизацию на этапе линковки). Уменьшает время компиляции
       -s
       -g0
       -fno-stack-protector   
    )
endif()


if(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64")
    add_definitions(
        -march=armv8-a          # Для получения оптимального когда на Orange Pi 5 Plus (ARM64)
    )
endif()


add_subdirectory(Sunset)
