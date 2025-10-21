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


volatile unsigned char last_portb_state;

enum e_status {
	SW0,
	SW1
};

volatile enum e_status status = INT0;

void led_shift(int delay)
{
	for (int i = 0; i < 8; i++)
	{
		if (status != SW1) return;
		PORTC = (~(0x01 << i)) & 0x0f;
		PORTD = (~(0x01 << i)) & 0xf0;
		my_delay(delay);
	}
	for (int i = 6; i >= 1; i--)
	{
		if (status != SW1) return;
		PORTC = (~(0x01 << i)) & 0x0f;
		PORTD = (~(0x01 << i)) & 0xf0;
		my_delay(delay);
	}
	PORTC = (PORTC & 0xf0) | 0x0f;
	PORTD = (PORTD & 0x0f) | 0xf0;
}



void led_alternating(int delay) {
	if (status != SW0) return;
	PORTC = ((PORTC & 0xF0) | (current_pattern_A & 0x0F));
	PORTD = ((PORTD & 0x0F) | (current_pattern_A & 0xF0));
	my_delay(delay);
	if (status != SW0) return;
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
	
	status = SW0;
}

ISR(INT1_vect)
{
	_delay_ms(15);
	if (PIND & 0x08) return;
	
	cli();
	EIFR |= 0x01;
	sei();
	
	status = SW1;
}

// SW3(PB0/PCINT0) 및 SW4(PB1/PCINT1) 공통 ISR
ISR(PCINT0_vect) {
    unsigned char current_portb_state = PINB;
    
    // SW3 (PB0/PCINT0) 체크 - 하강 에지 감지 (Default 패턴: 0xAA/0x55)
    // 현재 Low(0)이고, 이전이 High(1)일 때 (버튼 눌림)
    if ( !(current_portb_state & (1 << PB0)) && (last_portb_state & (1 << PB0)) ) {
        status = SW0;
    }
    
    // SW4 (PB1/PCINT1) 체크 - 하강 에지 감지 (주기 조절)
    if ( !(current_portb_state & (1 << PB1)) && (last_portb_state & (1 << PB1)) ) {
        status = SW1;
    }
    // 다음 인터럽트 감지를 위해 현재 상태를 이전 상태로 저장
    last_portb_state = current_portb_state;
}

int main() {
	DDRC = 0x0f;
	DDRD = 0xf0;
	PORTC = 0x0f;
	PORTD = 0xf0;


    DDRD &= ~0x0C; // 0x0C (0b00001100): SW1(PD2), SW2(PD3) 입력
    DDRB &= ~0x03; // 0x03 (0b00000011): SW3(PB0), SW4(PB1) 입력

    PORTD |= 0x0C; // 0x0C: SW1(PD2), SW2(PD3) 풀업 활성화
    PORTB |= 0x03; // 0x03: SW3(PB0), SW4(PB1) 풀업 활성화
	
	// --- external interrupt setting ---
	// INTO: PD2 falling edge , INT1: PD3 falling edge
	// EICRA = 0x0A; // 0000 1010
	// EIFR  = 0x03; // 0000 0011
	// EIMSK = 0x03; // 0000 0011


    // 4. 핀 변화 인터럽트 (PCINT0, PCINT1) 설정 - SW3, SW4
    // 0x01 (0b00000001): PCIE0 비트 설정 -> PORT B 그룹 활성화
    PCICR = 0x01;
    // 0x03 (0b00000011): PB0, PB1 핀 변화 감지 마스크 설정
    PCMSK0 = 0x03;
    // 0x01 (0b00000001): PCIF0 플래그 Clear (초기 오동작 방지)
    PCIFR = 0x01;

	last_portb_state = PINB; // PCINT 에지 감지를 위한 초기 상태 저장
	sei();
	my_delay(20);
	
	while(1) {
		switch (status)
		{
		case SW0:
			led_alternating(100);
			break;
		case SW1:
			led_shift(100);
			break;
		default:
			break;
		}
	}
}