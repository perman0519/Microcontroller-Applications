/*
 * assign2.c
 *
 * Created: 10/1/2025 2:28:47 PM
 *  Author: junssong
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

void led_all_onoff_alternating(int count, int _ms, int onbyte) {
	for (int i = 0 ; i < count; i++) {
		PORTD = onbyte;
		PORTC = 0x0f & onbyte;
		my_delay(_ms);
		PORTD = ~onbyte;
		PORTC = ~(0x0f & onbyte);
		my_delay(_ms);
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


int main(void)
{	
	DDRD = 0xf9;
	DDRC = 0xff;
	PORTC = 0x1E;
	PORTD = 0xFF;
 	while (1) {
		led_all_onoff_alternating(1, 200, 0xaa);
 		if (!(PIND & 0x02)) {
			 led_shift(2);
 		}
 		else if (!(PIND & 0x04)) {
			led_all_onoff(3, 300);
			PORTD = 0xff;
			PORTC = 0x1e;
		}
//  		else if (!(PINC & 0x04)) {
//  			int c = 2;
// 			 unsigned up = 0x80;
// 			 unsigned down = 0x01;
//  			while (c--) {
// 	 			for (int i = 0; i < 8; i++) {
//					 led_all_onoff_alternating2(1, 100, up | down);
//					 up = up >> 1;
//					 down = down << 1;
//	 			}
// 				up = 0x80;
// 				down = 0x01;
//  			}
//  			PORTD = 0xff;
//  		}
 	}
}

