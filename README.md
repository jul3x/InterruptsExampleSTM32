# STM32InterruptsExample
Repository contains examples of interrupts usage (i.e. for `UART` communication via `DMA`).

Examples were developed using `STM32F411xE` core and several additional electronic modules.

# UART transmission using DMA
Shown example uses DMA for strings processing and sends messages about pressed and released buttons on Nucleo board. Inputs of buttons and output of DMA were both implemented using interrupts of core microprocessor.

# Dependencies
* arm-eabi-gcc compiler
* stm32.h header file for stm32 code development
* gpio.h header file for GPIO module usage
* startup_stm32.c/.h files for startup of microcontroller handling  

Necessary paths for libraries are described in `Makefile`.
