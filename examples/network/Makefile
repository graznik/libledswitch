CFLAGS=-Wall -DDEBUG
ARCH := $(shell uname -m)
PREFIX=/usr/local

ifeq ($(ARCH),x86_64)
ARCH=arm
CROSS_COMPILE=arm-linux-gnueabihf-
CC=$(CROSS_COMPILE)gcc
INCLUDES=-I/usr/local/include/
LIBS=-L/usr/local/lib
endif

ledswitchd:
	$(CC) $(CFLAGS) $(INCLUDES) $(LIBS) -o ledswitchd ledswitchd.c ../../libledswitch.c -l rt -l bcm2835

install: ledswitchd
	install -m 0755 ledswitchd $(PREFIX)/bin

clean:
	-@rm -rf *.o *~ ledswitchd 2>/dev/null || true
