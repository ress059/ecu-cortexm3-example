cmake_minimum_required(VERSION 3.21)
project(ecu_example_stm32l432 VERSION 0.1)
include(FetchContent)

TODO: Set bsp.c to compile = ${CMAKE_CURRENT_LIST_DIR}/src/bsp/${BOARD}/bsp.c 
TODO: Set include path for drivers = ${CMAKE_CURRENT_LIST_DIR}/src/drivers/${MCU}; // include path example: #include "gpio/gpio.h"
TODO: Set driver sources to compile = ${CMAKE_CURRENT_LIST_DIR}/src/drivers/${MCU}/gpio.c 


#--------------------------------------------------------------------------------------------------------#
#---------------------------------------- INITIALIZE EXECUTABLE. ----------------------------------------#
#--------------------------------------------------------------------------------------------------------#
add_executable(${CMAKE_PROJECT_NAME}
    # Application code.
    ${CMAKE_CURRENT_LIST_DIR}/src/main.c
    ${CMAKE_CURRENT_LIST_DIR}/src/led_fsm.c

    # MCU-specific startup files.
    ${TOOLCHAIN_SOURCE_FILES}
)



#--------------------------------------------------------------------------------------------------------#
#-------------------------------- SPECIFY COMPILER SETTINGS FOR OUR PROJECT. ----------------------------#
#------ DO NOT PROROGATE THESE TO 3RD PARTY LIBRARIES IN CASE THEY NEED TO BE COMPILED DIFFERENTLY. -----#
#--------------------------------------------------------------------------------------------------------#
target_compile_options(${CMAKE_PROJECT_NAME}
    PRIVATE
        # Compiler flags specific to C
        $<$<COMPILE_LANG_AND_ID:C,GNU>:-Wstrict-prototypes>

        # Compiler flags specific to C++
        # $<$<COMPILE_LANG_AND_ID:CXX,GNU>:ENTER_FLAGS_HERE>

        # Compiler flags for both C and C++
        $<$<OR:$<COMPILE_LANG_AND_ID:C,GNU>,$<COMPILE_LANG_AND_ID:CXX,GNU>>:-fdiagnostics-color=always -fstack-usage -fno-common>
        $<$<OR:$<COMPILE_LANG_AND_ID:C,GNU>,$<COMPILE_LANG_AND_ID:CXX,GNU>>:-Wall -Wextra -Wpedantic -Wconversion -Wfloat-equal -Wundef -Wshadow -Wstack-usage=500>
        $<$<OR:$<COMPILE_LANG_AND_ID:C,GNU>,$<COMPILE_LANG_AND_ID:CXX,GNU>>:-Wcast-align -Wstrict-overflow=2 -Wwrite-strings -Waggregate-return>
        $<$<OR:$<COMPILE_LANG_AND_ID:C,GNU>,$<COMPILE_LANG_AND_ID:CXX,GNU>>:-Wcast-qual -Wswitch-default -Wimplicit-fallthrough -Wnull-dereference -Wdouble-promotion -O0>

        # Compiler flags specific to Debug builds
        $<$<AND:$<CONFIG:Debug>,$<OR:$<COMPILE_LANG_AND_ID:C,GNU>,$<COMPILE_LANG_AND_ID:CXX,GNU>>>:-g3>
)


target_compile_features(${CMAKE_PROJECT_NAME}
    PRIVATE 
        cxx_std_20
        c_std_23
)



#--------------------------------------------------------------------------------------------------------#
#-------------------------------- SPECIFY LINKER SETTINGS FOR OUR PROJECT. ------------------------------#
#--------------- THIS ONLY EFFECTS THE FINAL EXECUTABLE. THE ARCHIVER IS CALLED FOR LIBRARIES. ----------#
#--------------------------------------------------------------------------------------------------------#
target_link_options(${CMAKE_PROJECT_NAME}
    PRIVATE 
        # Linker flags specific to C
        # $<$<LINK_LANG_AND_ID:C,GNU>:ENTER_FLAGS_HERE>

        # Linker flags specific to C++
        # $<$<LINK_LANG_AND_ID:CXX,GNU>:ENTER_FLAGS_HERE>

        # Linker flags for both C and C++
        # $<$<OR:$<LINK_LANG_AND_ID:C,GNU>,$<LINK_LANG_AND_ID:CXX,GNU>>:ENTER_FLAGS_HERE>

        # Linker flags specific to Debug builds
        # $<$<AND:$<CONFIG:Debug>,$<OR:$<LINK_LANG_AND_ID:C,GNU>,$<LINK_LANG_AND_ID:CXX,GNU>>>:ENTER_FLAGS_HERE>
)



#--------------------------------------------------------------------------------------------------------#
#---------------- IMPORT ECU LIBRARY AND SPECIFY COMPILER SETTINGS THAT ECU SHOULD USE. -----------------#
#------------- ECU ONLY ENABLES ALL COMPILER WARNINGS AND LINKER GARBAGE COLLECTION FLAGS. --------------# 
#----------- TO MAKE IT CUSTOMIZABLE ECU DOES NOT SPECIFY OPTIMIZATION LEVEL OR C STANDARD --------------#
#------------ TO COMPILE FOR SO WE EXPLICITLY SPECIFY WHICH ONES WE WANT ECU TO USE HERE. ---------------# 
#--------------------------------------------------------------------------------------------------------#
FetchContent_Declare(
    ecu
    GIT_REPOSITORY https://github.com/ress059/ecu.git
    GIT_TAG        f331593a09a0004aa1dea6eb1f009bf035ac1035 # Most recent commit. TODO Will update once ecu officially released.
)
FetchContent_MakeAvailable(ecu)


target_compile_options(ecu
    PRIVATE 
        # Compiler flags specific to C
        # $<$<COMPILE_LANG_AND_ID:C,GNU>:ENTER_FLAGS_HERE>

        # Compiler flags for both C and C++
        $<$<OR:$<COMPILE_LANG_AND_ID:C,GNU>,$<COMPILE_LANG_AND_ID:CXX,GNU>>:-O0>
)


target_compile_features(ecu 
    PRIVATE 
        c_std_23
)



#--------------------------------------------------------------------------------------------------------#
#------------------------------- SPECIFY INCLUDE DIRECTORIES FOR OUR PROJECT. ---------------------------#
#----------- NOTE THAT WE DO NOT HAVE TO SPECIFY ECU INCLUDE DIRECTORIES. SEE COMMENT ABOVE -------------#
#-------------------------------------- TARGET_LINK_LIBRARIES() CALL. -----------------------------------#
#--------------------------------------------------------------------------------------------------------#
target_include_directories(${CMAKE_PROJECT_NAME}
    PRIVATE 
        ${CMAKE_CURRENT_LIST_DIR}/include
)



#--------------------------------------------------------------------------------------------------------#
#------------------------------------------- LINK AGAINST ECU. ------------------------------------------#
#----------- NOTE THAT ECU INCLUDE DIRECTORIES AND DEPENDENCIES ARE AUTOMATICALLY HANDLED WHEN ----------#
#------------ WE LINK AGAINST IT SINCE IT IS A CMAKE PROJECT SO WE DO NOT HAVE TO SPECIFY ANY -----------#
#------------------------------------ ADDITIONAL SETTINGS FOR ECU. --------------------------------------#
#--------------------------------------------------------------------------------------------------------#
target_link_libraries(${CMAKE_PROJECT_NAME} 
    PRIVATE 
        ecu 
)
