/*
* interrupt.c
*
* Created: 10/14/2025 2:07:14 PM
*  Author: junssong
*/

#define F_CPU 16000000UL
#define FOCS 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void my_delay(int _ms) {
	for (int i = 0; i < _ms; i++) {
		_delay_ms(1);
	}
}

volatile uint8_t current_pattern_A = 0xFF; // 초기 패턴 A (0xAA)
volatile uint8_t current_pattern_B = 0x00; // 초기 패턴 B (0x55)

enum e_status {
	INT00,
	INT01
};

volatile enum e_status status = INT0;

void led_shift(int delay)
{
	for (int i = 0; i < 8; i++)
	{
		if (status != INT01) return;
		PORTC = (~(0x01 << i)) & 0x0f;
		PORTD = (~(0x01 << i)) & 0xf0;
		my_delay(delay);
	}
	for (int i = 6; i >= 1; i--)
	{
		if (status != INT01) return;
		PORTC = (~(0x01 << i)) & 0x0f;
		PORTD = (~(0x01 << i)) & 0xf0;
		my_delay(delay);
	}
	PORTC = (PORTC & 0xf0) | 0x0f;
	PORTD = (PORTD & 0x0f) | 0xf0;
}



void led_alternating(int delay) {
	if (status != INT00) return;
	PORTC = ((PORTC & 0xF0) | (current_pattern_A & 0x0F));
	PORTD = ((PORTD & 0x0F) | (current_pattern_A & 0xF0));
	my_delay(delay);
	if (status != INT00) return;
	PORTC = ((PORTC & 0xF0) | (current_pattern_B & 0x0F));
	PORTD = ((PORTD & 0x0F) | (current_pattern_B & 0xF0));
	my_delay(delay);
}


ISR(INT0_vect)
{
	_delay_ms(15);
	if (PIND & 0x04) return;
	
	cli();
	EIFR |= 0x01;
	sei();
	
	status = INT00;
}

ISR(INT1_vect)
{
	_delay_ms(15);
	if (PIND & 0x08) return;
	
	cli();
	EIFR |= 0x01;
	sei();
	
	status = INT01;
}


int main() {
	DDRD = 0xf0;
	DDRC = 0x0f;
	PORTD = 0xf0;
	PORTC = 0x0f;
	
	// --- external interrupt setting ---
	// INTO: PD2 falling edge , INT1: PD3 falling edge
	EICRA = 0x0A; // 0000 1010
	EIFR  = 0x03; // 0000 0011
	EIMSK = 0x03; // 0000 0011
	
	sei();
	_delay_ms(20);
	
	while(1) {
		switch (status)
		{
		case INT00:
			led_alternating(100);
			break;
		case INT01:
			led_shift(100);
			break;
		default:
			break;
		}
	}
}