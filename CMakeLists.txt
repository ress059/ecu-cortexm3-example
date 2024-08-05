cmake_minimum_required(VERSION 3.21)
project(ecu_example_stm32f103c8 VERSION 0.1)
include(FetchContent)


################################################################################################################################
############################### Specify compiler and linker settings for this example project ##################################
################################################################################################################################
add_executable(${CMAKE_PROJECT_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/src/main.c
    ${CMAKE_CURRENT_LIST_DIR}/src/led_fsm.c
    ${TOOLCHAIN_SOURCE_FILES}
)


target_compile_options(${CMAKE_PROJECT_NAME}
    PRIVATE
        # Compiler flags specific to C
        $<$<COMPILE_LANG_AND_ID:C,GNU>:-Wstrict-prototypes>

        # Compiler flags specific to C++

        # Compiler flags for both C and C++
        $<$<OR:$<COMPILE_LANG_AND_ID:C,GNU>,$<COMPILE_LANG_AND_ID:CXX,GNU>>:-fdiagnostics-color=always -ffunction-sections -fdata-sections -fstack-usage -fno-common>
        $<$<OR:$<COMPILE_LANG_AND_ID:C,GNU>,$<COMPILE_LANG_AND_ID:CXX,GNU>>:-Wall -Wextra -Wpedantic -Wconversion -Wfloat-equal -Wundef -Wshadow -Wstack-usage=500>
        $<$<OR:$<COMPILE_LANG_AND_ID:C,GNU>,$<COMPILE_LANG_AND_ID:CXX,GNU>>:-Wcast-align -Wstrict-overflow=2 -Wwrite-strings -Waggregate-return>
        $<$<OR:$<COMPILE_LANG_AND_ID:C,GNU>,$<COMPILE_LANG_AND_ID:CXX,GNU>>:-Wcast-qual -Wswitch-default -Wimplicit-fallthrough -Wnull-dereference -Wdouble-promotion -Os>

        # Compiler flags specific to Debug builds
        $<$<CONFIG:Debug>:-g3>
)


target_compile_features(${CMAKE_PROJECT_NAME}
    PRIVATE 
        cxx_std_20
        c_std_23
)


target_compile_definitions(${CMAKE_PROJECT_NAME}
    PRIVATE 
        USE_HAL_DRIVER
        STM32F103xB
)


target_include_directories(${CMAKE_PROJECT_NAME}
    PRIVATE 
        ${CMAKE_CURRENT_LIST_DIR}/include
)


################################################################################################################################
################## Here is where we actually import ECU library, cross-compile it, and link against it. ########################
################################################################################################################################
FetchContent_Declare(
    ecu
    GIT_REPOSITORY https://github.com/ress059/ecu.git
    GIT_TAG        f331593a09a0004aa1dea6eb1f009bf035ac1035 # Most recent commit. TODO Will update once ecu officially released.
)
FetchContent_MakeAvailable(ecu)


# target_compile_options(ecu 
#     PRIVATE 
#       Specify any additional flags (i.e. optimization flags) you want here when cross-compiling ECU.
# )


target_link_libraries(${CMAKE_PROJECT_NAME} 
    PRIVATE 
        ecu 
)






# TODO: 
# 0) Obviously need startup code before anything else.
# 1) Creating normal executable and not .elf.
# 2) Need to create .map file.
# 3) Getting these linker warnings
# in function `_close_r': closer.c:(.text._close_r+0xc): warning: _close is not implemented and will always fail
# in function `_lseek_r': lseekr.c:(.text._lseek_r+0x10): warning: _lseek is not implemented and will always fail
# in function `_read_r': readr.c:(.text._read_r+0x10): warning: _read is not implemented and will always fail
# in function `_write_r': writer.c:(.text._write_r+0x10): warning: _write is not implemented and will always fail
# /opt/arm-gnu-toolchain-13.3.rel1-x86_64-arm-none-eabi/bin/../lib/gcc/arm-none-eabi/13.3.1/../../../../arm-none-eabi/bin/ld: warning: cannot find entry symbol Reset_Handler; defaulting to 08000000
# /opt/arm-gnu-toolchain-13.3.rel1-x86_64-arm-none-eabi/bin/../lib/gcc/arm-none-eabi/13.3.1/../../../../arm-none-eabi/bin/ld: warning: ecu_example_stm32f103 has a LOAD segment with RWX permissions
