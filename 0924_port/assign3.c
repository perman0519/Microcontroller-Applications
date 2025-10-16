
/*
 * PCINT.c
 *
 * Created: 10/15/2025 1:43:47 PM
 *  Author: junssong
 */ 


//#include <avr/io.h>
//#define FOCS 16000000UL
//#define F_CPU 16000000UL
//#include <util/delay.h>
//#include <avr/interrupt.h>
//
//
//enum e_status {
	//PCINT0_0,
	//PCINT0_1,
	//PD22,
	//PD33
//};
//
//enum e_status status = PCINT0_0;
//
//int speed = 0;
//
//void my_delay(int _ms) {
	//for (int i = 0; i < _ms; i++) {
		//_delay_ms(1);
	//}
//}
//
//void led_alternating(int count, int _ms, int portc, int portd) {
	//for (int i = 0 ; i < count; i++) {
		//PORTC = portc;
		//PORTD = portd;
		//my_delay(_ms);
		//PORTC = ~portc;
		//PORTD = ~portd;
		//my_delay(_ms);
	//}
//}
//
//ISR(PCINT0_vect)
//{
	//cli();
	//PCIFR |= 0x01;
	//sei();
	//
	//_delay_ms(15);
	//
	//unsigned char now_b = PINB;
	//
	//if ((now_b & 0x01) == 0) {
		//status = PCINT0_0;
	//}
	//if ((now_b & 0x02) == 0) {
		//speed++;
		//speed %= 8;
	//}
//}
//
//ISR(INT0_vect)
//{
	//
	//_delay_ms(15);
	////if (PIND & 0x08) return;
	//
	//cli();
	//EIFR |= 0x01;
	//sei();
	//
	//status = PD22;
//}
//
//ISR(INT1_vect)
//{
	//_delay_ms(15);
	////if (PIND & 0x08) return;
	//
	//cli();
	//EIFR |= 0x02;
	//sei();
	//
	//status = PD33;
//}
//
//int main() {
	//DDRD = (DDRD & 0x0f) | 0xf0;
	//DDRC = (DDRC & 0xf0) | 0x0f;
	//
	////DDRB &= (unsigned char)~0x03;
	//DDRB &= (unsigned char)~0x03; //0b00000011
	//
	//PORTD = (PORTD & 0x0f) | 0xf0;
	//PORTC = (PORTC & 0xf0) | 0x0f;
	//
	//_delay_ms(20);
//
//
	//EICRA = 0x0A;
	//EIFR  = 0x03;  //
	//EIMSK = 0x03;
//
	//PCIFR  |= 0x01; //00000001
	//PCMSK0 |= 0x03; //00000011
	//PCICR  |= 0x01; //00000001	
	////PCIFR  |= (1 << PCIF0); //0000 0001
	////PCMSK0 |= (1 << PCINT0); //0000 0011
	////PCICR  |= (1 << PCIE0); //0000 0001
	//
	//sei();
	//
	//unsigned int portc = 0xf0;
	//unsigned int portd = 0x0f;
	////int real_speed = 500 - (50 * speed);
	//while(1) {
		//switch (status)
		//{
			//case PD22:
			//portc = 0xff;
			//portd = 0xff;
			//break;
			//
			//case PD33:
			//portc = 0xfc;
			//portd = 0xcf;
			//break;
			//
			//case PCINT0_0:
			//portc = 0xfa;
			//portd = 0x5f;
			//break;
		//}
		//led_alternating(1, 500 - (50 * speed), portc, portd);
	//}
//}


#define FOCS 16000000UL
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// =======================================================
// ?? ?? (ISR? ?? ?? ? ??)
// =======================================================
volatile uint8_t current_pattern_A = 0xAA; // ?? ?? A (Default: 0xAA)
volatile uint8_t current_pattern_B = 0x55; // ?? ?? B (Default: 0x55)
volatile uint16_t current_delay_ms = 500;  // ?? ??? (500ms)

// PCINT? ?? ??? ?? PORT B ?? ?? ??
volatile unsigned char last_portb_state;

// =======================================================
// ISR ?? (??? ?? ??)
// =======================================================

void my_delay(int _ms) {
	for (int i = 0; i < _ms; i++) {
		_delay_ms(1);
	}
}
// SW1 (PD2/INT0) ISR: ?? 0xF0/0x0F? ??
ISR(INT0_vect) {
	current_pattern_A = 0xF0;
	current_pattern_B = 0x0F;
}

// SW2 (PD3/INT1) ISR: ?? 0xC3/0x3C? ??
ISR(INT1_vect) {
	current_pattern_A = 0xC3;
	current_pattern_B = 0x3C;
}

//// SW3(PB0/PCINT0) ? SW4(PB1/PCINT1) ?? ISR
//ISR(PCINT0_vect) {
	//unsigned char current_portb_state = PINB;
	//
	//// 1. SW3 (PB0/PCINT0) Check - Default ?? (0xAA/0x55)
	//// ?? ?? (?? ??) ??: ?? Low(0)??, ??? High(1)? ?
	//if ( !(current_portb_state & (1 << PB0)) && (last_portb_state & (1 << PB0)) ) {
		//// SW3 Pressed
		//current_pattern_A = 0xAA;
		//current_pattern_B = 0x55;
	//}
	//
	//// 2. SW4 (PB1/PCINT1) Check - ?? ??
	//// ?? ?? (?? ??) ??
	//if ( !(current_portb_state & (1 << PB1)) && (last_portb_state & (1 << PB1)) ) {
		//// SW4 Pressed: 50ms? ?? (?? 100ms)
		//if (current_delay_ms > 100) {
			//current_delay_ms -= 50;
			//} else {
			//current_delay_ms = 500; // 100ms ??? 500ms? ??
		//}
	//}
//
	//// ?? ???? ??? ?? ?? ??? ?? ??? ??
	//last_portb_state = current_portb_state;
//}

ISR(PCINT0_vect)
{
	cli();
	PCIFR |= 0x01;
	sei();
	
	_delay_ms(15);
	
	unsigned char now_b = PINB;
	
	if ((now_b & 0x01) == 0) {
		current_pattern_A = 0xAA;
		current_pattern_B = 0x55;
	}
	if ((now_b & 0x02) == 0) {
		// SW4 Pressed: 50ms? ?? (?? 100ms)
		if (current_delay_ms > 100) {
			current_delay_ms -= 50;
			} else {
			current_delay_ms = 500; // 100ms ??? 500ms? ??
		}
	}
}


// =======================================================
// ?? ??
// =======================================================
int main() {// 1. DDR Setup: ??(LED) ? ??(???) ??

	// LED ?? ??
	// PC0-PC3? ??(1)?? ??, ???(PC4-PC7)? AVR ???? ??(0)?? ????
	DDRC = 0x0F;
	// PD4-PD7? ??(1)?? ??, ???(PD0-PD3)? AVR ???? ??(0)?? ????
	DDRD = 0xF0;
	
	// ??? ?? ?? (0?? ??)
	// PD2, PD3? ??(0)?? ?? (~0x0C = 0b11110011)
	DDRD &= ~0x0C;
	// PB0, PB1? ??(0)?? ?? (~0x03 = 0b11111100)
	DDRB &= ~0x03;

	// 2. PORT Setup: ?? LED ?? ? ?? ?? ???

	// LED ?? ?? (current_pattern_A) ??
	// PORTD? PD4-PD7? ?? A? ?? 4??? ??. ???? 0?? ????
	PORTD = (current_pattern_A & 0xF0);
	// PORTC? PC0-PC3? ?? A? ?? 4??? ??. ???? 0?? ????
	PORTC = (current_pattern_A & 0x0F);
	
	// ?? ??? ?? ?? ?? ??? (1? ??)
	// ?? PORTD ?? PD2, PD3 ?? ??? 1? ?? ??
	PORTD |= 0x0C;
	// ?? PORTB ?? PB0, PB1 ?? ??? 1? ?? ??
	PORTB |= 0x03;
	
	
	// 3. ?? ???? (INT0, INT1) ?? - SW1, SW2
	// EIMSK? INT0, INT1 ??? 1? ?? (0x03)
	EIMSK = 0x03;
	
	// ?? ??(Falling Edge) ??? ?? (ISCxx = 10)
	// EICRA? 0x0A (0b00001010)? ???? '10' ??? ? ?? ??? ??
	EICRA = 0x0A;
	// EICRA &= ~0x05? EICRA = 0x0A;? ????? ?? ??
	
	// EIFR flag clear
	EIFR = 0x03;
	
	// 4. ? ?? ???? (PCINT0, PCINT1) ?? - SW3, SW4
	// PCICR? PCIE0 ??? 1? ?? (0x01)
	PCICR = 0x01;
	// PCMSK0? PCINT0, PCINT1 ??? 1? ?? (0x03)
	PCMSK0 = 0x03;
	// PCIFR? PCIF0 ???? 1? ?? ???
	PCIFR = 0x01;
	
	
	// 5. ?? ???? ??? ? ?? ?? ??
	last_portb_state = PINB;
	sei();
	
	
	// 6. ?? ??: LED ??? (Alternative) ??
	while(1) {
		// Pattern A ??
		PORTD = (PORTD & 0x0F) | (current_pattern_A & 0xF0);
		PORTC = (PORTC & 0xF0) | (current_pattern_A & 0x0F);
		my_delay(current_delay_ms);
		
		// Pattern B ??
		PORTD = (PORTD & 0x0F) | (current_pattern_B & 0xF0);
		PORTC = (PORTC & 0xF0) | (current_pattern_B & 0x0F);
		my_delay(current_delay_ms);
	}
}