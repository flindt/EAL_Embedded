This project was written by pfl. 

	It is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation. It is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
    or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details.
    
 

========================================================================================

 This is how I went about creating this magnificent blinking device :)
 
 =======================================================================================
 
 using Sourcery Codebench lite 2011/9  toolchain
eclipse indigo c/c++ with gnuarm plugin
http://sourceforge.net/projects/gnuarmeclipse/files/Eclipse/updates/


installed / setup path in [[~/.profile]]
new project, empty main.c                                 COMPILES !

add library path file:///home/pfl/sandbox/stellaris/stellaris-drivers/LuminaryMicro
add linker libraries to linker 

										compiles with gpio.h
										still cant use GPIO_PORTF_DIR_R



include theese
#include <hw_memmap.h>
#include <hw_types.h>
#include <hw_sysctl.h>
#include <gpio.h>
#include <grlib.h>
#include <sysctl.h>

change the IO to use this 


	SysCtlPeripheralEnable( SYSCTL_PERIPH_GPIOF );
	GPIODirModeSet( GPIO_PORTF_BASE, (GPIO_PIN_2 | GPIO_PIN_3), GPIO_DIR_MODE_HW );


	GPIOPadConfigSet( GPIO_PORTF_BASE, (GPIO_PIN_2 | GPIO_PIN_3 ), GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD );


										compiles - but does not link
										error : collect2: ld returned 1 exit status



Be carefull when linking libraries: 
library search path : -L[[../../../../../sandbox/stellaris/stellaris-drivers/LuminaryMicro/arm-none-eabi-gcc/]]   
AND
libraries : -ldriver

added startup.c and standalone.ld 
NOTICE : theese two files are made in pairs that work together - allways copy them both from a known good project!


-T"../src/standalone.ld"

										Compiles and links !!!!!!

										


----------------------------------------------------------------------------------------------------------------------------------

===== debugging try 3 =====

copy from [[FreeRTOS]] : startup.c
copy form [[FreeRTOS]] : standolone.ld
copy from pss test3: main.c

start the openocd server 

added gdb support to eclipse
changed GDB command to : arm-none-eabi-gdb	
remote target at [[localhost:3333											debug]] runs but not correctly

changed file image to load to .HEX										target can be stopped, but still no code upload
upload to address 0													tries to upload but fails...

																		seems it has been flashed - but there are still errors in OCD


slowed down jtag speed to 100 kHz in ek-lm3s6965.cfg

running openOCD from commandline
openocd -f ek-lm3s6965.cfg -c "init" -c "halt" -c "reset halt" 					debug works from eclipse - still does not blink, and there are errors

																		but it steps and breaks correctly
																		

												and it blinks !



----------------------------------------------------------------------------------------------------------------------------------------


 