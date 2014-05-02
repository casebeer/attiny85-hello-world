
/**
 *  
 * Blinking LED ATTiny85 "hello world"
 *
 * Following the tutorial at:
 * http://www.instructables.com/id/Honey-I-Shrunk-the-Arduino-Moving-from-Arduino-t/?ALLSTEPS
 *  
 */

#include <avr/io.h>
// F_CPU frequency to be defined at command line
#include <util/delay.h>
#include <avr/wdt.h>

// LED is on pin 2, PB3
#define LED      PB3
#define DELAY_MS 500
#define HIGH (PORTB |= (1 << LED))
#define LOW  (PORTB &= ~(1 << LED))

int main () {
	uint8_t high = 1;
	uint16_t ms = 0;
	uint8_t i = 0;

	MCUSR = 0;
	wdt_disable();

	// setup LED pin for output in port B's direction register
	DDRB |= (1 << LED);

	// set LED pin LOW
	PORTB &= ~(1 << LED);

	while (1) {
		for (i = 0; i < 3; i++) {
			HIGH;
			_delay_ms(10);
			LOW;
			_delay_ms(40);
		}

		LOW;
		_delay_ms(350);
		LOW;
		_delay_ms(500);

	}

	return 0;
}

