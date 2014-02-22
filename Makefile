all: gpio-int-test


gpio-sysfs.o: gpio-sysfs.cpp gpio-sysfs.h
	gcc -c gpio-sysfs.cpp

gpio-int-test: gpio-int-test.cpp gpio-sysfs.o
	gcc -o gpio-int-test gpio-sysfs.o gpio-int-test.cpp

clean:
	rm -f gpio-int-test gpio-sysfs.o
