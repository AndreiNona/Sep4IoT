# VIA_FreeRTOS_ATMEGA
FreeRTOS for Arduino AVR hardware, but without Arduino code.

It is based on **FreeRTOS version 10.4.3** and includes the subset that fits the AVR devices.

Based on a Fork of Phillip Stevens FreeRTOS fork optimised for the Arduino AVR devices.

### Project Setup
Be sure the project you want to add FreeRTOS to is under git control!

Open a command prompt in your projects root folder.

Add this repository in to your project as a git-submodule - by executing this command in the command prompt:

`git submodule add https://github.com/ihavn/VIA_FreeRTOS_AVRMEGA FreeRTOS`

Add `../FreeRTOS/src` as a relative include path under the projects `Properties->Toolchain->AVR/GNU C Compiler->Directories`.

Take a look at the `main.c`in the `Example` folder to get started. It is recommended that you create your own `main.c` in the projects root, and add seperate c modules and header files for each task, and give them meaningful names.

### FreeRTOS Configuration
Configuration of FreeRTOS must be done in `\FreeRTOS\src\FreeRTOSConfig.h` see details in the free RTOS documentation. **Be sure you know what you are doing before you configure anything!**


### FreeRTOS Heap Memory
This repository uses heap_4.c to handle dynamic memory/heap therefore it is **required to define how much RAM we will allocate for 
heap use**. This is done by defining
`#define configTOTAL_HEAP_SIZE 4000` in the configuration file `\FreeRTOS\src\FreeRTOSConfig.h`. In this example it is defined to 4000 bytes.


#### R2R Network for trace 
If `#define configUseR2RTrace 1` then the `FreeRTOSTraceDriver` is called when a task is switched in and out, and it will send data out to a R2R-network that must be connected to PORTK.

If `#define configUseR2RTrace 0` then the `FreeRTOSTraceDriver` will not be used, and can be removed from the project.
 
## Original Phillip Stevens README
This is a fork of Richard Barry's freeRTOS, optimised for the Arduino AVR devices.

It has been created to provide access to FreeRTOS capabilities, with full compatibility to the Arduino environment.
It does this by keeping hands off almost everything, and only touching the minimum of hardware to be successful.

[![Arduino CI](https://github.com/feilipu/Arduino_FreeRTOS_Library/workflows/Arduino%20CI/badge.svg)](https://github.com/marketplace/actions/arduino_ci)

## Usage & Further Reading

Read the short blog post on [Arduino FreeRTOS](https://feilipu.me/2015/11/24/arduino_freertos/) to get started. And there is another much older post on using [FreeRTOS with AVR](https://feilipu.me/2011/09/22/freertos-and-libraries-for-avr-atmega/), which may be useful to read too. There are some further posts I've written on [Hackster.IO](https://www.hackster.io/feilipu), but they're essentially the same content.

The canonical source for information is the [FreeRTOS Web Site](https://www.freertos.org/). Within this site, the [Getting Started](https://www.freertos.org/FreeRTOS-quick-start-guide.html) page is very useful. This is the source for FreeRTOS usage (as distinct from installing and using this Arduino Library).

My other [AVRfreeRTOS Sourceforge Repository](https://sourceforge.net/projects/avrfreertos/) or [AVRfreeRTOS Github](https://github.com/feilipu/avrfreertos) has plenty of examples, ranging from [blink](https://sourceforge.net/projects/avrfreertos/files/MegaBlink/) through to a [synthesiser](https://sourceforge.net/projects/avrfreertos/files/GA_Synth/).

This library was the genesis of [generalised support for the ATmega platform within FreeRTOS](https://github.com/FreeRTOS/FreeRTOS-Kernel/pull/48).

## General

FreeRTOS has a multitude of configuration options, which can be specified from within the FreeRTOSConfig.h file.
To keep commonality with all of the Arduino hardware options, some sensible defaults have been selected.

The AVR Watchdog Timer is used to generate 15ms time slices, but Tasks that finish before their allocated time will hand execution back to the Scheduler. This does not affect the use of any of the normal Timer functions in Arduino.

Time slices can be selected from 15ms up to 500ms. Slower time slicing can allow the Arduino MCU to sleep for longer, without the complexity of a Tickless idle.

Watchdog period options:
* `WDTO_15MS`
* `WDTO_30MS`
* `WDTO_60MS`
* `WDTO_120MS`
* `WDTO_250MS`
* `WDTO_500MS`
* `WDTO_1S`
* `WDTO_2S`

Note that Timer resolution is affected by integer math division and the time slice selected. Trying to measure 50ms, using a 120ms time slice for example, won't work.

Stack for the `loop()` function has been set at 192 bytes. This can be configured by adjusting the `configMINIMAL_STACK_SIZE` parameter. If you have stack overflow issues, just increase it.
Users should prefer to allocate larger structures, arrays, or buffers using `pvPortMalloc()`, rather than defining them locally on the stack.

Memory for the heap is allocated by the normal `malloc()` function, wrapped by `pvPortMalloc()`.
This option has been selected because it is automatically adjusted to use the capabilities of each device.
Other heap allocation schemes are supported by FreeRTOS, and they can used with additional configuration.

## Upgrading

* [Upgrading to FreeRTOS-9](https://www.freertos.org/FreeRTOS-V9.html)
* [Upgrading to FreeRTOS-10](https://www.freertos.org/FreeRTOS-V10.html)

## Errors

* Stack Overflow: If any stack (for the `loop()` or) for any Task overflows, there will be a slow LED blink, with 4 second cycle.
* Heap Overflow: If any Task tries to allocate memory and that allocation fails, there will be a fast LED blink, with 100 millisecond cycle.

## Errata

Testing with the Software Serial library shows some incompatibilities at low baud rates (9600), due to the extended time this library disables the global interrupt. Use the hardware USARTs.

## Compatibility

  * ATmega328 @ 16MHz : Arduino UNO, Arduino Duemilanove, Arduino Diecimila, etc.
  * ATmega328 @ 16MHz : Adafruit Pro Trinket 5V, Adafruit Metro 328, Adafruit Metro Mini
  * ATmega328 @ 16MHz : Seeed Studio Stalker
  * ATmega328 @ 16MHz : Freetronics Eleven
  * ATmega328 @ 12MHz : Adafruit Pro Trinket 3V
  * ATmega32u4 @ 16MHz : Arduino Leonardo, Arduino Micro, Arduino Yun, Teensy 2.0
  * ATmega32u4 @ 8MHz : Adafruit Flora, Bluefruit Micro
  * ATmega1284p @ 16MHz: Sanguino, WickedDevice WildFire
  * ATmega1284p @ 24.576MHz : Seeed Studio Goldilocks, Seeed Studio Goldilocks Analogue
  * ATmega2560 @ 16MHz : Arduino Mega, Arduino ADK
  * ATmega2560 @ 16MHz : Seeed Studio ADK
  
The new megaAVR 0-Series devices (eg. ATmega4809) are not fully compatible with this library. Their Timer configuration is substantially different from previous devices, and forms part of a new __avr8x__ architecture. It may be a while until avr-libc is updated to include support for megaAVR devices, but when that happens further work will be added here.

## Files & Configuration

* `ATMEGA_FreeRTOS.h` : Must always be `#include` first. It references other configuration files, and sets defaults where necessary.
* `FreeRTOSConfig.h` : Contains a multitude of API and environment configurations.
* `FreeRTOSVariant.h` : Contains the AVR specific configurations for this port of freeRTOS.
* `heap_3.c` : Contains the heap allocation scheme based on `malloc()`. Other schemes are available, but depend on user configuration for specific MCU choice.



