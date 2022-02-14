# Raspberry PI GPIO communication via sysfs

SysGPIO is a class which encapsulate the
reading / writing and interrupt handling to/from GPIO pins
of the raspberry pi.

## Documentation

The online docs are here: http://berndporr.github.io/gpio-sysfs/

The docs/pdf directory contains the documentation as PDF.

## Example program

The demo program gpio-int-test waits for an interrupt for 
3secs on a GPIO pin. If an interrupt has happened it prints out 
"interrupt" otherwise "timeout".

To run it type:

```
make
sudo ./gpio-int-test 22
```

which waits then for an interrupt on GPIO pin 22.

## How to use it in your program

Just copy `gpio-sysfs.cpp` and `gpio-sysfs.h` into your project 
and include the header `gpio-sysfs.h` in your program.
Alternatively, you could import this git as a subproject and 
include the header in the subdir.
