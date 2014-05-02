
default: watchdog

%.hex: %.c
	avr-gcc -Wall -Os -DF_CPU=1000000 -mmcu=attiny85 -o $<.o $<
	avr-objcopy -j .text -j .data -O ihex $<.o $@

hello: hello.hex
	avrdude -c usbtiny -p t85 -U flash:w:hello.hex:i

watchdog: watchdog_blink.hex
	avrdude -c usbtiny -p t85 -U flash:w:watchdog_blink.hex:i

clean: /dev/null
	rm -f hello.o hello.hex
