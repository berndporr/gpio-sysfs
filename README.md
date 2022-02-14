# Raspberry PI GPIO communication via sysfs

SysGPIO is a class which encapsulate the
reading / writing and interrupt handling to/from GPIO pins
of the raspberry pi.

## Example program

The demo program gpio-int-test waits for an interrupt for 
3secs on a GPIO pin. If an interrupt has happened it prints out 
"interrupt" otherwise "timeout".

To run it type:

make
sudo ./gpio-int-test 22

which waits then for an interrupt on GPIO pin 22.

## How to use it in your program

Just copy `gpio-sysfs.cpp` and `gpio-sysfs.h` and include them in your program
or declare this git a s subproject under git and include them.
