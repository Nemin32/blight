CC ?= gcc

build:
	${CC} backlight_setter.c -o blight -O2 -Wall -Wextra

clean:
	rm -f blight
