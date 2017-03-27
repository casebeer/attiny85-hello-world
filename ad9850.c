
/**
 *  
 *  Control an AD9850 DDS chip
 *
 *  See http://nr8o.dhlpilotcentral.com/?p=83
 *  
 */

#include <avr/io.h>
// F_CPU frequency to be defined at command line
#include <util/delay.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <stdlib.h>

//#define pulse(PIN) do { PORTB ^= (1 << (PIN)); __asm__("nop\r\nnop\r\nnop"); PORTB ^= (1 << (PIN)); } while (0)
//#define pulse(PIN) do { PORTB |= (1 << (PIN)); __asm__("nop\r\nnop\r\nnop"); PORTB &= ~(1 << (PIN)); } while (0)
#define pulse(PIN) do { PORTB |= (1 << (PIN)); PORTB &= ~(1 << (PIN)); } while (0)
//#define write(PIN, BIT) do { PORTB |= (((BIT) & 1) << (PIN)); PORTB &= ~(((BIT) & 1) << (PIN)); } while (0)
#define write(PIN, BIT) do { if (BIT) { PORTB |= (1 << (PIN)); } else { PORTB &= ~(1 << (PIN)); } } while (0)

// AD9850 pin D7 for serial data input
#define DATA PB0
#define RESET PB1

#define FQ_UD PB3
#define W_CLK PB4

#define AD9850_CLOCK_FREQUENCY 125000000.0
//#define ad9850_frequency_to_delta_phase(FREQ) (((uint32_t)((FREQ) * AD9850_CLOCK_FREQUENCY)) / (1UL << 31))
//#define ad9850_frequency_to_delta_phase(FREQ) ((uint32_t)((FREQ) * (1UL << 32)) / AD9850_CLOCK_FREQUENCY)
//#define ad9850_frequency_to_delta_phase(FREQ) ((uint32_t)((FREQ) * (1UL << 32)) / AD9850_CLOCK_FREQUENCY)
//#define ad9850_frequency_to_delta_phase(FREQ) ((double)(FREQ) * ((1UL << 31) / AD9850_CLOCK_FREQUENCY))
#define ad9850_frequency_to_delta_phase(FREQ) (uint32_t)(((double)(FREQ) * ad9850_phase_increment))

typedef struct ad9850_config {
	uint32_t frequency_tuning_word;
	uint8_t  shutdown;
	uint8_t  phase;
} *ad9850_config_t;

const double ad9850_phase_increment = (1UL << 31) / AD9850_CLOCK_FREQUENCY;

ad9850_config_t ad9850_init_config(ad9850_config_t);
ad9850_config_t ad9850_write_config(ad9850_config_t);

ad9850_config_t ad9850_init_config(ad9850_config_t config) {
	// default frequency = 0 MHz
	config->frequency_tuning_word = 0x0UL;
	// default to not shutdown after frequency update
	config->shutdown = 0x0;
	// default phase = 0º
	config->phase = 0x0;

	return config;
}

ad9850_config_t ad9850_write_config(ad9850_config_t config) {
	/*
	 * Send 40 bit programming via serial
	 * 
	 * 32 bits of frequency, LS bit to MS bit
	 * 3 control bits
	 * 5 phase bits
	 */
	uint8_t bit;

	// enter serial mode
	pulse(FQ_UD);

	for (bit = 0; bit < 32; bit++) {
		write(DATA, (config->frequency_tuning_word >> bit) & 0x01);
		pulse(W_CLK);
	}

	// must write bits 32, 33 low – factory testing control bits
	write(DATA, 0);
	pulse(W_CLK);
	write(DATA, 0);
	pulse(W_CLK);

	// write the shutdown control bit
	write(DATA, config->shutdown & 0x1);
	pulse(W_CLK);

	// write five bits of phase
	for (bit = 0; bit < 5; bit++) {
		write(DATA, (config->phase >> bit) & 0x1);
		pulse(W_CLK);
	}

	// trigger the frequency update
	pulse(FQ_UD);

	return config;
}

void setup(void);
void setup() {
	// suspend interrupts, clear any watchdog timers, then resume interrupts 
	cli();
	wdt_reset();
	// clear reset state flag
	MCUSR = 0;
	// set the watchdog enable bit writable
	WDTCR |= (1 << WDCE) | (1 << WDE);
	// clear the watchdog enable bit
	WDTCR &= ~(1 << WDE);
	// reenable interrutps
	sei();

	// setup pins for output
	DDRB |= (1 << DATA) | (1 << RESET) | (1 << FQ_UD) | (1 << W_CLK);

	// set pins low
	PORTB &= ~((1 << DATA) | (1 << RESET) | (1 << FQ_UD) | (1 << W_CLK));

	// reset module
	pulse(RESET);
	pulse(W_CLK);
}

int main () {
	struct ad9850_config config_struct;
	uint32_t f = 570000UL; //11000000UL; //15600000UL + 100; //15000000UL + 50; //1UL; //710000UL; //1560000UL; //5817900UL;
	uint32_t i;

	ad9850_config_t config = &config_struct;

	setup();

	ad9850_init_config(config);
	config->shutdown = 0x0;
		
	config->frequency_tuning_word = ad9850_frequency_to_delta_phase(f);
	ad9850_write_config(config);

	//while (1) {};

	while (1) {	
		config->frequency_tuning_word = ad9850_frequency_to_delta_phase(f);
		config->frequency_tuning_word = 200000000UL;
		_delay_ms(250);
		/*
		for (i = 0; i < 100; i++) {
			config->frequency_tuning_word = ad9850_frequency_to_delta_phase(f + i * 1000);
			// simulate ALE's 8FSK between 750–2500 Hz
			//config->frequency_tuning_word = ad9850_frequency_to_delta_phase(f + 750 + 250 * (random() & 0x07));
			//config->frequency_tuning_word = ad9850_frequency_to_delta_phase(f + i * 1000);
			ad9850_write_config(config);
			//_delay_ms(1000 / 4);
			//_delay_ms(1000 / 125);
			//_delay_ms(1000 / 5);
			_delay_ms(1);
		}
		*/
		//continue;
		pulse(RESET);
		//pulse(W_CLK);
		//pulse(FQ_UD);
		_delay_ms(250);
	}

	return 0;
}

