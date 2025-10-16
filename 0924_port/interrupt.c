/*
* interrupt.c
*
* Created: 10/14/2025 2:07:14 PM
*  Author: junssong
*/

#include <avr/io.h>
#define FOCS 16000000UL
#define F_CPU 16000000UL
#include <util/delay.h>

#include <avr/interrupt.h>



void my_delay(int _ms) {
	for (int i = 0; i < _ms; i++) {
		_delay_ms(1);
	}
}

void led_all_onoff_alternating2(int count, int _ms, int onbyte) {
	for (int i = 0 ; i < count; i++) {
		PORTD = ~onbyte;
		my_delay(_ms);
		PORTD = ~onbyte;
		my_delay(_ms);
	}
}

void led_all_onoff_alternating3(int count, int _ms, int onbyte) {
	for (int i = 0 ; i < count; i++) {
		PORTC = ~onbyte;
		my_delay(_ms);
		PORTC = ~onbyte;
		my_delay(_ms);
	}
}

void led_shift(int count) {
	int c = 2;
	PORTC = 0x1e;
	PORTD = 0xff;
	while (c--) {
		for (unsigned i = 0x02; i < 0x20; ) {
			led_all_onoff_alternating3(1, 50, i);
			i = i << 1;
		}
		PORTC = 0x1e;
		int t = 4;
		unsigned i = 0x10;
		while (t--) {
			led_all_onoff_alternating2(1, 50, i);
			i = i << 1;
		}
		PORTD = 0xf0;
		t = 4;
		i = 0x80;
		while (t--) {
			led_all_onoff_alternating2(1, 50, i);
			i = i >> 1;
		}
		PORTD = 0xf0;
		t = 4;
		i = 0x10;
		while (t--) {
			led_all_onoff_alternating3(1, 50, i);
			i = i >> 1;
		}
		PORTC = 0x1e;
	}
	PORTC = 0x1e;
	PORTD = 0xff;
}

void led_all_alternating(int count, int _ms, int portc, int portd) {
	for (int i = 0 ; i < count; i++) {
		PORTC = portc;
		PORTD = portd;
		my_delay(_ms);
		PORTC = ~portc;
		PORTD = ~portd;
		my_delay(_ms);
	}
}

ISR(INT0_vect)
{
	led_shift(2);
	//_delay_ms(15);
	//if (PIND & 0x04) return;
	//
	//cli();
	//EIFR |= 0x01;
	//sei();
	//
	//for (int j = 0; j < 2; j++) {
		//for (int i = 0; i < 8; i++) {
			//PORTC = (~(0x01 << i)) & 0x0f;
			//PORTD = (~(0x01 << i)) & 0xf0;
			//_delay_ms(100);
		//}
		//
		//for (int i = 6; i >= 0; i--) {
			//PORTC = ~((0x01 << i)) & 0x0f;
			//PORTD = ~((0x01 << i)) & 0xf0;
			//_delay_ms(100);
		//}
		//
		//PORTD = (PORTD & 0x0f) | 0xf0;
		//PORTC = (PORTC & 0x0f) | 0xf0;
		//_delay_ms(100);
	//}	
}

ISR(INT0_vect)
{
	_delay_ms(15);
	if (PIND & 0x08) return;
	
	cli();
	EIFR |= 0x01;
	sei();
	
	led_all_alternating(2, 100, 0xfa, 0xaf);
	
}

int main() {
	DDRD = 0xf0;
	DDRC = 0x0f;
	PORTD = 0xf0;
	PORTC = 0x0f;
	
	// --- external interrupt setting ---
	// INTO: PD2 falling edge , INT1: PD3 falling edge
	EICRA = 0x0A;
	EIFR  = 0x03;  //
	EIMSK = 0x03;
	
	sei();
	
	_delay_ms(20);
	
	while(1) {
		PORTD = ~0xAF;
		PORTC = ~0xFA;
		_delay_ms(500);
		PORTD = ~0x5F;
		PORTC = ~0xF5;
		_delay_ms(500);
	}
}