GNU MCU Eclipse is a family of Eclipse CDT extensions and tools for GNU ARM & RISC-V development. RISC-V is an open instruction set architecture (ISA) based on established reduced instruction set computing (RISC) principles. It's a good direction and selection, especially for Chinese CPU development. I will study it in the future, but, now, I must use the GNU MCU Eclipse development environment for ARM, especially for STM32. If you want to learn more about GNU MCU Eclipse, you can refer the following website.

<a href="https://gnu-mcu-eclipse.github.io/">https://gnu-mcu-eclipse.github.io/</a>

In my local PC environment, I install the following components in windows7. Of cause, you will install all of these components in Linux and MAC, but in this article, I will explain the installation of windows platform as an example.

In general, for install GNU MCU Eclipse environment, you must execute the following 5 steps.

1. Install Eclipse which is a famous IDE production and CDT plugin. With CDT plugin, Eclipse can make itself to a full-featured C/C++ development tool. In my local PC environment, I use the NEON.3 version. It can be downloaded in the following website.

<a href="https://www.eclipse.org/downloads/">https://www.eclipse.org/downloads/</a>

2. Install 'GNU MCU Eclipse' plugin for Eclipse. It can be found in 'Eclipse marketplace' in Eclipse with the keyword 'MCU'. Now, its version is 4.3.3. This plugin is a bridge for Eclipse and a series of ARM development tools which includes ARM toolchains, build tools, OpenOCD, pyOCD, QEMU, and SEGGER J-Link. If you install it successfully, you will see the mark of GNU MCU Eclipse in the 'help dialog' in Eclipse.

<a href="https://github.com/wangdongshi/RTOS/blob/master/DevEnv/GNU%20MCU%20Eclipse/GNU%20MCU%20Eclipse%20Mark.JPG">GNU MCU Eclipse</a>

3. Install the following ARM development tools. It can be downloaded in the following website.

<a href="https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/">https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/</a>

<a href="https://github.com/gnu-mcu-eclipse/windows-build-tools/releases">https://github.com/gnu-mcu-eclipse/windows-build-tools/releases</a>

<a href="https://github.com/gnu-mcu-eclipse/openocd/releases">https://github.com/gnu-mcu-eclipse/openocd/releases</a>

<a href="https://www.segger.com/downloads/jlink/">https://www.segger.com/downloads/jlink/</a>

<a href="https://github.com/gnu-mcu-eclipse/qemu/releases">https://github.com/gnu-mcu-eclipse/qemu/releases</a>

The 1st tool is ARM cross-compile toolchains. In GNU MCU Eclipse community, they recommend using ARM official GNU cross compiler. It's named 'GNU Arm Embedded Toolchain', witch integrated and validated packages featuring the Arm Embedded GCC compiler, libraries and other GNU tools necessary for bare-metal software development on devices based on the Arm Cortex-M and Cortex-R processors. These toolchains are based on Free Software Foundation's (FSF) GNU Open source tools and newlib.
The download file is named like 'gcc-arm-none-eabi-7-2017-q4-major-win32.exe'. For convenience, you'd better install the software and other software that will be described bellow in a unified folder for management. In my PC, I install it in the 'D:\Work\eclipse\GNU MCU Eclipse' folder.

The 2nd tool is windows build tools. It includes the additional tools required to perform builds on Windows (make & rm).

The 3rd tool is OpenOCD. It is a free and open-source On-Chip debugger. With OpenOCD, we can realize in-system programming and boundary-scan testing for embedded target devices. It's worth mentioning that after continuous contributions from volunteers, the tool can already support most ARM CPUs.

The 4th tool is J-Link tools. It is made by SEGGER Corporation. For the emulator of ARM, there is no better than SEGGER's production. In addition, SEGGER also produced firmware that upgraded ST-Link to J-link, and it has been made public in the community, which has greatly expanded the scope of use of the SEGGER J-Link tool. Especially for the development of STM32, I believe we can not bypass the most convenient tool.

The 5th tool is QEMU. QEMU is an open source virtual machine program that is becoming popular. It is a great project, but its original focus was to emulate boards with large cores, usually application class, able to run Unix/Linux kernels. Support for bare metal Cortex-M based boards was available only for a very limited range of Cortex-M3 cores, so of little use in GNU MCU Eclipse. Now the GNU MCU Eclipse community developed the qemu-system-gnuarmeclipse. The GNU MCU Eclipse QEMU maintains a certain degree of compatibility with the original qemu-system-arm, but it does not include the large ARM cores, being focused only on Cortex-M cores. The main compatibility requirement for the emulator was to support all devices addressed during the standard CMSIS initializations, which generally refer to PLL & clock settings. The next requirement was to fully support the GPIOs, at the level of allowing one or more LEDs to blink. qemu-system-gnuarmeclipse accurately emulates Cortex-M3 and M4 thumb instructions, but specific Cortex-M4 floating point instructions are not supported. The current implementation of NVIC uses some related interrupt controller code from larger cores, and is not very accurate. Only the following limited MCUs and evaluation boards can be supported by QEMU at this stage.

[ MCU ]
- STM32F103RB
- STM32F107VC
- STM32F405RG
- STM32F407VG
- STM32F407ZG
- STM32F429ZI
- STM32L152RE

[ board ]
- Maple – LeafLab Arduino-style STM32 microcontroller board
- NUCLEO-F103RB – ST Nucleo Development Board for STM32 F1 series
- NetduinoGo – Netduino GoBus Development Board with STM32F4
- NetduinoPlus2 – Netduino Development Board with STM32F4
- STM32-E407 – Olimex Development Board for STM32F407ZGT6
- STM32-H103 – Olimex Header Board for STM32F103RBT6
- STM32-P103 – Olimex Prototype Board for STM32F103RBT6
- STM32-P107 – Olimex Prototype Board for STM32F107VCT6
- STM32F4-Discovery – ST Discovery kit for STM32F407/417 lines
- STM32F429I-Discovery – ST Discovery kit for STM32F429/439 lines

4. After installed the above tools, you also set these path and execute file name in Eclipse.

<a href="https://github.com/wangdongshi/RTOS/blob/master/DevEnv/GNU%20MCU%20Eclipse/Tools%20setting%20in%20GNU%20MCU%20Eclipse.JPG">Tools setting in GNU MCU Eclipse</a>

5. Now you can create a C/C++ project with the following type.
- Hello World ARM Cortex-M C/C++ Project
- Freescale Kinetis KLxx C/C++ Project
- Freescale Processor Expert C/C++ Project
- SiFive RISC-V C/C++ Project
- STM32F0xx C/C++ Project
- STM32F10x C/C++ Project
- STM32F2xx C/C++ Project
- STM32F3xx C/C++ Project
- STM32F4xx C/C++ Project
- STM32F7xx C/C++ Project
Next, you need to set debugger settings in you own eclipse project. Now enjoy it!

<a href="https://github.com/wangdongshi/RTOS/blob/master/DevEnv/GNU%20MCU%20Eclipse/QEMU%20debugger%20setting.JPG">QEMU debugger setting</a>