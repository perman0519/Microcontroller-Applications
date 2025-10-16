/*
 * 0924_port.c
 *
 * Created: 9/24/2025 1:36:22 PM
 * Author : junssong
 */ 

#include <avr/io.h>
#define FOCS 16000000UL
#define F_CPU 16000000UL
#include <util/delay.h>


void my_delay(int _ms) {
	for (int i = 0; i < _ms; i++) {
		_delay_ms(1);
	}
}

void led_all_onoff_alternating_shift(int count, int _ms) {
	PORTC = 0x0f;
	PORTD = 0x0f;
	while (count--) {
		unsigned x = 0x1;
		while (x < 0x10) {
			PORTD = ~x;
			my_delay(_ms);
			x = x << 1;
		}
		PORTD = 0x0f;
		x = 0x1;
		while (x < 0x10) {
			PORTC = ~x;
			my_delay(_ms);
			x = x << 1;
		}
		PORTC = 0x0f;
		x = 0x04;
		while (x > 0x00) {
			PORTC = ~x;
			my_delay(_ms);
			x = x >> 1;
		}
		PORTC = 0x0f;
		x = 0x04;
		while (x > 0x01) {
			PORTD = ~x;
			my_delay(_ms);
			x = x >> 1;
		}
		PORTD = 0x0f;
	}
}

void led_all_onoff_alternating(int count, int _ms, unsigned onbyte) {
	for (int i = 0 ; i < count; i++) {
		unsigned d = (onbyte & 0x0f);
		unsigned c = (onbyte & 0xf0) >> 4;
		PORTD = d;
		PORTC = c;
		my_delay(_ms);
		PORTD = ~d;
		PORTC = ~c;
		my_delay(_ms);
	}
}

void led_all_onoff(int count, int _ms) {
	for (int i = 0 ; i < count; i++) {
		PORTD = 0x00;
		PORTC = 0x00;
		my_delay(_ms);
		PORTD = 0x0f;
		PORTC = 0x0f;
		my_delay(_ms);
	}
}

int main(void)
{
	DDRD = 0x0f;
	DDRC = 0x0f;
	
	PORTD = 0xff;
	PORTC = 0xff;
	while(1) {
		led_all_onoff(3, 400);
		led_all_onoff_alternating(3, 400, 0x55);
		led_all_onoff_alternating(3, 400, 0xf0);
		led_all_onoff_alternating(3, 400, 0xC3);
		led_all_onoff_alternating_shift(5, 100);
	}
}

