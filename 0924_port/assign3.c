
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


#define FOCS 16000000UL     // CPU 클럭 주파수 정의 (AVR 설정에 사용)
#define F_CPU 16000000UL    // _delay_ms() 함수 사용을 위한 CPU 클럭 정의
#include <avr/io.h>         // AVR 입출력 레지스터 정의 포함
#include <avr/interrupt.h>  // 인터럽트 관련 함수 (ISR, sei 등) 포함
#include <util/delay.h>     // 지연 함수 (_delay_ms) 사용을 위해 포함

// =======================================================
// 전역 변수 (ISR과 메인 루프 간 공유)
// =======================================================
// volatile: 컴파일러 최적화 방지 (ISR과 공유 시 필수)
volatile uint8_t current_pattern_A = 0xAA; // 초기 패턴 A (0xAA)
volatile uint8_t current_pattern_B = 0x55; // 초기 패턴 B (0x55)
volatile uint16_t current_delay_ms = 500;  // 초기 딜레이 (500ms)

// PCINT의 에지 감지를 위한 PORT B 이전 상태 저장
volatile unsigned char last_portb_state;

// =======================================================
// ISR 정의 (스위치 입력 처리)
// (기존 로직 유지)
// =======================================================

void my_delay(int _ms) {
    for (int i = 0; i < _ms; i++) {
        _delay_ms(1);
    }
}

// SW1 (PD2/INT0) ISR: 패턴 0xF0/0x0F로 변경
ISR(INT0_vect) {
    cli();
    EIFR |= 0x01;
    sei();
    current_pattern_A = 0xF0;
    current_pattern_B = 0x0F;
}

// SW2 (PD3/INT1) ISR: 패턴 0xC3/0x3C로 변경
ISR(INT1_vect) {

    cli();
    EIFR |= 0x02;
    sei();
    current_pattern_A = 0xC3;
    current_pattern_B = 0x3C;
}

// SW3(PB0/PCINT0) 및 SW4(PB1/PCINT1) 공통 ISR
ISR(PCINT0_vect) {
    unsigned char current_portb_state = PINB;
    
    // SW3 (PB0/PCINT0) 체크 - 하강 에지 감지 (Default 패턴: 0xAA/0x55)
    // 현재 Low(0)이고, 이전이 High(1)일 때 (버튼 눌림)
    if ( !(current_portb_state & (1 << PB0)) && (last_portb_state & (1 << PB0)) ) {
        current_pattern_A = 0xAA;
        current_pattern_B = 0x55;
    }
    
    // SW4 (PB1/PCINT1) 체크 - 하강 에지 감지 (주기 조절)
    if ( !(current_portb_state & (1 << PB1)) && (last_portb_state & (1 << PB1)) ) {
        // SW4 Pressed: 50ms씩 감소 (최소 100ms)
        if (current_delay_ms > 100) {
            current_delay_ms -= 50;
        } else {
            current_delay_ms = 500; // 100ms 이하면 500ms로 순환
        }
    }

    // 다음 인터럽트 감지를 위해 현재 상태를 이전 상태로 저장
    last_portb_state = current_portb_state;
}


// =======================================================
// 메인 함수
// =======================================================
int main() {
    // 1. DDR Setup: 출력(LED) 및 입력(스위치) 설정

    // LED 출력 설정 (PC0-PC3 Output)
    DDRC = 0x0F; // 0x0F (0b00001111): PC0-PC3 출력
    // LED 출력 설정 (PD4-PD7 Output)
    DDRD = 0xF0; // 0xF0 (0b11110000): PD4-PD7 출력
        
    // 스위치 입력 설정 (DDR 비트를 0으로 만듦)
    DDRD &= ~0x0C; // 0x0C (0b00001100): SW1(PD2), SW2(PD3) 입력
    DDRB &= ~0x03; // 0x03 (0b00000011): SW3(PB0), SW4(PB1) 입력

    // 2. PORT Setup: 초기 LED 패턴 및 풀업 저항 활성화

    // LED 초기 패턴 (current_pattern_A) 적용
    PORTD = (current_pattern_A & 0xF0); // 패턴 A의 상위 4비트 출력
    PORTC = (current_pattern_A & 0x0F); // 패턴 A의 하위 4비트 출력
        
    // 모든 스위치 핀에 풀업 저항 활성화 (PORT 비트를 1로 설정)
    PORTD |= 0x0C; // 0x0C: SW1(PD2), SW2(PD3) 풀업 활성화
    PORTB |= 0x03; // 0x03: SW3(PB0), SW4(PB1) 풀업 활성화
        
        
    // 3. 외부 인터럽트 (INT0, INT1) 설정 - SW1, SW2
    // 0x03 (0b00000011): INT1, INT0 활성화
    EIMSK = 0x03; 
        
    // 하강 에지(Falling Edge) 트리거 설정 (ISCxx = 10)
    // 0x0A (0b00001010): ISC11과 ISC01 비트를 1로 설정
    EICRA = 0x0A; 
        
        
    // 4. 핀 변화 인터럽트 (PCINT0, PCINT1) 설정 - SW3, SW4
    // 0x01 (0b00000001): PCIE0 비트 설정 -> PORT B 그룹 활성화
    PCICR = 0x01;
    // 0x03 (0b00000011): PB0, PB1 핀 변화 감지 마스크 설정
    PCMSK0 = 0x03;
    // 0x01 (0b00000001): PCIF0 플래그 Clear (초기 오동작 방지)
    PCIFR = 0x01;
        
        
    // 5. 전역 인터럽트 활성화 및 초기 상태 저장
    last_portb_state = PINB; // PCINT 에지 감지를 위한 초기 상태 저장
    sei();                   // 전역 인터럽트 활성화
        
        
    // 6. 메인 루프: LED 깜빡임 (Alternative) 동작
    while(1) {
        // Pattern A 출력
        PORTD = (PORTD & 0x0F) | (current_pattern_A & 0xF0);
        PORTC = (PORTC & 0xF0) | (current_pattern_A & 0x0F);
        my_delay(current_delay_ms);
        
        // Pattern B 출력
        PORTD = (PORTD & 0x0F) | (current_pattern_B & 0xF0);
        PORTC = (PORTC & 0xF0) | (current_pattern_B & 0x0F);
        my_delay(current_delay_ms);
    }
}