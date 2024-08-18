# ecu-cortexm4-example
Example of using ECU on Cortex-M4 MCU. The goal of this project is to highlight how to import ECU into your CMake project and showcase some source code utilizing ECU library.


# Importing ECU
TODO


# Project Overview
NUCLEO-L432KC (https://www.st.com/en/evaluation-tools/nucleo-l432kc.html#overview) is used which has STM32L432KC. 
TODO: show pic of setup and explain project. Any number of switches/leds. When you press switch led goes on, release switch led goes off, hold down switch led toggles. goal is to easily support any number of these switch/led modules and let them have their own hold time and toggle times. Use ECU fsm and ECU timers for this.


# Project Architecture
TODO: Show layers - App, BSP, Drivers, MCU layers.

