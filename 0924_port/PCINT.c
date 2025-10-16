
/*
 * PCINT.c
 *
 * Created: 10/15/2025 1:43:47 PM
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
		for (unsigned i = 0x02; i < 0x20;) {
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

void led_all_onoff(int count, int _ms) {
	for (int i = 0 ; i < count; i++) {
		PORTC = 0xff;
		PORTD = 0xf0;
		my_delay(_ms);
		PORTC = ~0xff;
		PORTD = ~0xf0;
		my_delay(_ms);
	}
}

ISR(PCINT0_vect)
{
	
	
	cli();
	PCIFR |= 0x01;
	sei();
	
	_delay_ms(15);
	
	unsigned char now_b = PINB;
	
	if ((now_b & 0x01) == 0) {
		for (int j = 0; j < 2; j++) {
			for (int i = 0; i < 8; i++) {
				unsigned char mask = (unsigned char)(0x01 << i);
				
				PORTC = (PORTC & 0xf0) | (unsigned char)((~mask) & 0x0f);
				PORTD = (PORTD & 0x0f) | (unsigned char)((~mask) & 0xf0);
				_delay_ms(100);
			}
			
			for (int i = 6; i >= 0; i--) {
				PORTC = ~((0x01 << i)) & 0x0f;
				PORTD = ~((0x01 << i)) & 0xf0;
				_delay_ms(100);
			}
			//for (int i = 6; i >= 0; i--) {
				//unsigned char mask = (unsigned char)(0x01 << i);
				//
				//PORTC = (PORTC & 0xf0) | (unsigned char)((~mask) & 0x0f);
				//PORTD = (PORTD & 0x0f) | (unsigned char)((~mask) & 0xf0);
				//_delay_ms(100);
			//}
			PORTD = (PORTD & 0x0f) | 0xf0;
			PORTC = (PORTC & 0xf0) | 0x0f;
			_delay_ms(100);
		}
	}
	if ((now_b & 0x02) == 0) {
		for (int k = 0; k < 3; k++) {
			PORTD = (PORTD & 0x0f) | 0x00;
			PORTC = (PORTC & 0xf0) | 0x00;
			_delay_ms(500);
			PORTD = (PORTD & 0x0f) | 0xf0;
			PORTC = (PORTC & 0xf0) | 0x0f;
			_delay_ms(500);
		}
	}
}

ISR(PCINT1_vect)
{
	//_delay_ms(55);
	led_all_onoff(3, 300);
	//EIFR = 0x00;
}

int main() {
	DDRD = (DDRD & 0x0f) | 0xf0;
	DDRC = (DDRC & 0xf0) | 0x0f;
	
	//DDRB &= (unsigned char)~0x03;
	DDRB &= (unsigned char)~0x03; //0b00000011
	
	PORTD = (PORTD & 0x0f) | 0xf0;
	PORTC = (PORTC & 0xf0) | 0x0f;
	
	_delay_ms(20);

	PCIFR  |= 0x01; //00000001
	PCMSK0 |= 0x03; //00000011
	PCICR  |= 0x01; //00000001	
	//PCIFR  |= (1 << PCIF0); //0000 0001
	//PCMSK0 |= (1 << PCINT0); //0000 0011
	//PCICR  |= (1 << PCIE0); //0000 0001
	
	sei();
	
	while(1) {
		PORTD = ~0xAF;
		PORTC = ~0xFA;
		_delay_ms(500);
		PORTD = ~0x5f;
		PORTC = ~0xf5;
		_delay_ms(500);
	}
}