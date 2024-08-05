# This toolchain file is for cross compiling with ARM GNU toolchain. Used this resource as a reference:
# https://cmake.org/cmake/help/book/mastering-cmake/chapter/Cross%20Compiling%20With%20CMake.html
# Target = STM32F103 (STM32 blue pill).
# Host = Linux x86 machine. This toolchain file requires a Linux x86 host.
message(STATUS "Using toolchain file for cross compiling STM32F103 on Linux x86 host. Must be "
               "using Linux x86 host with arm-none-eabi toolchain installed in /opt directory.")


# Inform CMake we are cross compiling. CMAKE_SYSTEM_PROCESSOR is optional - others should be set.
set(CMAKE_SYSTEM_NAME Generic CACHE INTERNAL "")
set(CMAKE_CROSSCOMPILING TRUE CACHE INTERNAL "")
set(CMAKE_SYSTEM_PROCESSOR stm32f103 CACHE STRING "")


# Prevent CMake from performing internal compilation and linking tests since we are cross compiling
set(CMAKE_C_COMPILER_WORKS TRUE CACHE INTERNAL "")
set(CMAKE_CXX_COMPILER_WORKS TRUE CACHE INTERNAL "")


# Executables, libraries, and headers on the host system and target toolchain may have
# the same names which can result in CMake incorrectly using a host file when we are
# cross compiling. To avoid this scenario we set these variables.
# 1) CMAKE_FIND_ROOT_PATH_MODE_PROGRAM: Have CMake only look in the host build system to
# avoid errors if CMake needs to find an executable to configure the build system (i.e. Make).
# 2) CMAKE_FIND_ROOT_PATH_MODE_LIBRARY: Have CMake only look in the directories specified in
# CMAKE_FIND_ROOT_PATH to ensure CMake uses the libraries in arm-none-eabi. I.e. stdlib.
# 3) CMAKE_FIND_ROOT_PATH_MODE_INCLUDE: Same logic as CMAKE_FIND_ROOT_PATH_MODE_LIBRARY.
# Docker image will ensure toolchain is consistently in this path and is this version.
set(CMAKE_FIND_ROOT_PATH "/opt/arm-gnu-toolchain-13.3.rel1-x86_64-arm-none-eabi" CACHE INTERNAL "")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER CACHE INTERNAL "")
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY CACHE INTERNAL "")
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY CACHE INTERNAL "")


if(CMAKE_HOST_LINUX OR CMAKE_HOST_UNIX)
    find_program(CMAKE_C_COMPILER		NAMES arm-none-eabi-gcc			REQUIRED    ONLY_CMAKE_FIND_ROOT_PATH)
    find_program(CMAKE_CXX_COMPILER		NAMES arm-none-eabi-g++			REQUIRED    ONLY_CMAKE_FIND_ROOT_PATH)
    find_program(LINKER					NAMES arm-none-eabi-ld			REQUIRED    ONLY_CMAKE_FIND_ROOT_PATH)
    find_program(CMAKE_AR				NAMES arm-none-eabi-gcc-ar		REQUIRED    ONLY_CMAKE_FIND_ROOT_PATH) # Creates, modifies, and extracts from .a libraries
    find_program(CMAKE_RANLIB			NAMES arm-none-eabi-gcc-ranlib	REQUIRED    ONLY_CMAKE_FIND_ROOT_PATH) # Generate indices for .a libraries
    find_program(ASSEMBLER				NAMES arm-none-eabi-as			REQUIRED    ONLY_CMAKE_FIND_ROOT_PATH)
    find_program(GPROF					NAMES arm-none-eabi-gprof		REQUIRED    ONLY_CMAKE_FIND_ROOT_PATH) # Monitor program performance
    find_program(NM						NAMES arm-none-eabi-gcc-nm		REQUIRED    ONLY_CMAKE_FIND_ROOT_PATH) # List symbols exported by .o file. Measure code size in more detail (symbols).
    find_program(OBJCOPY				NAMES arm-none-eabi-objcopy		REQUIRED    ONLY_CMAKE_FIND_ROOT_PATH) # Copy .o file
    find_program(OBJDUMP				NAMES arm-none-eabi-objdump		REQUIRED    ONLY_CMAKE_FIND_ROOT_PATH) # Display info about .o files
    find_program(SIZE					NAMES arm-none-eabi-size		REQUIRED    ONLY_CMAKE_FIND_ROOT_PATH) # Measure overall code size (sections)
    find_program(STRIP					NAMES arm-none-eabi-strip		REQUIRED    ONLY_CMAKE_FIND_ROOT_PATH) # Discard symbols from .o files
    find_program(GCOV					NAMES arm-none-eabi-gcov		REQUIRED    ONLY_CMAKE_FIND_ROOT_PATH) # Code coverage
else()
    message(FATAL_ERROR "Only Linux hosts currently supported.")
endif()


# Do not let CMake automatically add some compiler flags based on the build type.
# For cross compilation and in embedded environments we want full control over these.
# Override the default flags with empty strings. Let the application decide the optimization
# levels and if they want debug symbols printed or not.
set(CMAKE_C_FLAGS_DEBUG "" CACHE STRING "")
set(CMAKE_C_FLAGS_RELEASE "-DNDEBUG" CACHE STRING "")
set(CMAKE_C_FLAGS_MINSIZEREL "-DNDEBUG" CACHE STRING "")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "-DNDEBUG" CACHE STRING "")
set(CMAKE_CXX_FLAGS_DEBUG "" CACHE STRING "")
set(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG" CACHE STRING "")
set(CMAKE_CXX_FLAGS_MINSIZEREL "-DNDEBUG" CACHE STRING "")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-DNDEBUG" CACHE STRING "")


# Only set hardware-specific compiler flags for STM32F103 here since this is a toolchain
# file. Remaining flags will be added by CMake build system depending on what is needed 
# by the application.
set(CMAKE_C_FLAGS_INIT "-mcpu=cortex-m3 -mfloat-abi=soft -mthumb" CACHE STRING "")
set(CMAKE_CXX_FLAGS_INIT "-fno-exceptions -fno-rtti -fno-use-cxa-atexit -mcpu=cortex-m3 -mfloat-abi=soft -mthumb" CACHE STRING "")


# Specify any toolchain-specific source files that must be compiled by the target.
set(TOOLCHAIN_SOURCE_FILES "${CMAKE_CURRENT_LIST_DIR}/stm32f103c8_startup.c" CACHE STRING "")


# Only set hardware-specific linker flags for STM32F103 here since this is a toolchain
# file. Remaining flags will be added by CMake build system depending on what is needed 
# by the application.
set(CMAKE_EXE_LINKER_FLAGS_INIT "-mcpu=cortex-m3 -mfloat-abi=soft -mthumb -specs=nosys.specs -Wl,-T${CMAKE_CURRENT_LIST_DIR}/stm32f103c8.ld -Wl,-Map=${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.map -Wl,--gc-sections" CACHE STRING "")


# These rule variables control how CMake actually calls the linker from the command line.
# Edit these so it creates an .elf executable. All other flags remain the same as the default.
# Verified linker invokation is exactly the same besides the .elf
set(CMAKE_C_LINK_EXECUTABLE "<CMAKE_C_COMPILER> <FLAGS> <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET>.elf <LINK_LIBRARIES>" CACHE STRING "")
set(CMAKE_CXX_LINK_EXECUTABLE "<CMAKE_CXX_COMPILER> <FLAGS> <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET>.elf <LINK_LIBRARIES>" CACHE STRING "")
