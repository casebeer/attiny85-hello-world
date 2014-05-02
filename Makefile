
default: watchdog

%.o: %.c
	avr-gcc -Wall -Os -DF_CPU=1000000 -mmcu=attiny85 -o $@ $<

%.hex: %.o
	avr-objcopy -j .text -j .data -O ihex $< $@

hello: hello.hex force
	avrdude -c usbtiny -p t85 -U flash:w:$<:i

watchdog: watchdog_blink.hex force
	avrdude -c usbtiny -p t85 -U flash:w:$<:i

triple_flash: triple_flash.hex force
	avrdude -c usbtiny -p t85 -U flash:w:$<:i

force: /dev/null

clean: /dev/null
	rm -f *.o *.hex
