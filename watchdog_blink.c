
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
#include <avr/sleep.h>
#include <avr/interrupt.h>

// LED is on pin 2, PB3
#define LED      PB3
#define DELAY_MS 500
#define HIGH (PORTB |= (1 << LED))
#define LOW  (PORTB &= ~(1 << LED))

void setup(void);
void wdt_clear();

void setup(void) {
	MCUSR = 0;
	wdt_disable();

	// Disable ADC for power savings
	ADCSRA &= ~(1<<ADEN);

	// setup LED pin for output in port B's direction register
	DDRB |= (1 << LED);

	// set LED pin LOW
	PORTB &= ~(1 << LED);

	// set LED pin HIGH
	PORTB |= (1 << LED);

	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	//set_sleep_mode(SLEEP_MODE_IDLE);
		/*
		set_sleep_mode(SLEEP_MODE_PWR_DOWN);
		sleep_enable();
		sleep_mode();
		sleep_disable();
		*/
}

void wdt_clear() {
	// suspend interrupts
	cli();
	wdt_reset();
	MCUSR = 0;
	WDTCR |= (1 << WDCE) | (1 << WDE);
	WDTCR &= ~(1 << WDE);
	// reenable interrutps
	sei();
}

/**
 * Set the watchdog timeout to interrupt us after the specified timeout, then enter power down sleep mode.
**/
void sleep_for(timeout) {
	// suspend interrupts
	cli();

	// n.b. wdt_enable() sets WDE!
	wdt_enable(timeout);

	// clear WDE
	WDTCR |= (1 << WDCE) | (1 << WDE);
	WDTCR |= (1 << WDIE);

	// set to power down 
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);

	// enable interrutps
	sei();

	sleep_enable();
	// enter sleep mode
	sleep_mode();

	// woken up
	
	// clear the watchdog reset flag, and the watchdog enable flag that it's set
	wdt_clear();

	sleep_disable();
}

int main () {
	setup();

	wdt_clear();

	while (1) {
		HIGH;
		sleep_for(WDTO_1S);

		LOW;
		sleep_for(WDTO_500MS);
	}

	return 0;
}

uint8_t foo;

// Interrupt service routine for the watchdog timeout interrupt
ISR(WDT_vect) {};

//ISR(WDT_vect) {
//	// clear watchdog enable and watchdog reset flag after wakeup
//	wdt_clear();
//	//PORTB ^= (1 << LED);
//}
