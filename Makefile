CC=gcc

all: hello.c
	$(CC) `pkg-config --libs gtk+-2.0 gstreamer-0.10` `pkg-config --cflags gtk+-2.0 gstreamer-0.10` -o hello hello.c

clean:
	/bin/rm -f *.o *~
