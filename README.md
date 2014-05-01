
# ATTiny85 Hello World

Blinking an LED using an ATTiny85 programmed with `avr-gcc` and `avrdude` on Mac OS X. 

## Mac OS X AVR toolchain install

Using Homebrew, tap [larsimmisch's homebrew-avr repository](https://github.com/larsimmisch/homebrew-avr):

```
brew tap larsimmisch/avr
brew install avr-libc
```

## Build steps

1. Compile with `avr-gcc`
2. Prepare the hex file with `avr-objcopy`
3. Flash the hex with `avrdude`

```
avr-gcc -Wall -Os -DF_CPU=1000000 -mmcu=attiny85 -o hello.o hello.c
avr-objcopy -j .text -j .data -O ihex hello.o hello.hex
avrdude -c usbtiny -p t85 -U flash:w:hello.hex:i
```

`F_CPU` must be defined at the `avr-gcc` command line or above the `#include <util/delay.h>`
so the delay library knows the microcontroller's clock speed.

## Breadboard configuration

Wire the ISP-6 cable from the USBTinyISP to the breadboarded ATTiny85 using jumpers. 

### ISP-6 pinout

Looking at the ISP-6 *cable* with the connector key at the bottom, the pins are numbered 
like this:

```
+-----------+
|(6) (4) (2)|
|(5) (3) (1)|
+-----------+
     |_|
```

1. MISO
2. Vcc
3. SCK
4. MOSI
5. Reset
6. Ground

(See also [this photo](http://www.ics.uci.edu/~givargis/courses/cs145/resources/avrisp.png))

### ATTiny85 pintout

The ATTiny85's pins are:

```
 (8) (7) (6) (5)
+---------------+
|               |
| o             |
+---------------+
 (1) (2) (3) (4)
```

1. PB5 (Reset)
2. PB3
3. PB4
4. Ground
5. PB0 (MOSI)
6. PB1 (MISO)
7. PB2 (SCK)
8. Vcc

### Programming jumpers

Connect the ISP and the ATTiny85 together on the Ground, MISO (Master In, Slave Out), 
MOSI (Master Out, Slave In), SCK (Serial ClocK), Reset, and Vcc pins:

- ISP-6 pin 1–ATTiny85 pin 6 (MISO)
- ISP-6 pin 2–ATTiny85 pin 8 (Vcc)
- ISP-6 pin 3–ATTiny85 pin 7 (SCK)
- ISP-6 pin 4–ATTiny85 pin 5 (MOSI)
- ISP-6 pin 5–ATTiny85 pin 1 (Reset)
- ISP-6 pin 6–ATTiny85 pin 4 (Ground)

N.B. The ATTiny's pins 2 and 3 are unconnected.

### LED circuit

Power the LED with a transistor and a series resistor, switching the transistor using a resistor 
from the ATTiny's pin 2 to the transistor's base.

## References

- [Honey, I Shrunk the Arduino](http://www.instructables.com/id/Honey-I-Shrunk-the-Arduino-Moving-from-Arduino-t/?ALLSTEPS)
- [ATTiny85 and ISP-6 pinouts](http://fabienroyer.wordpress.com/2010/12/12/build-a-cheap-flexible-avr-microcontroller-programming-target-board/)
- [ATTiny85 pinout](http://teslaui.files.wordpress.com/2012/11/attiny85_pins.png)
- [Atmel's in-system programming documentation](http://www.atmel.com/Images/doc0943.pdf)

