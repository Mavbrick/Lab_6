/* Author: 
 * Lab Section:
 * Assignment: Lab 6  Exercise 2
 * Exercise Description: [optional - include for your own benefit]
 *
 * I acknowledge all content contained herein, excluding template or example
 * code is my own original work.
 *
 *  Demo Link: https://youtu.be/yVcYQbhQPXM 
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn () {
	
	TCCR1B = 0x0B;

	OCR1A = 125;

	TIMSK1 = 0x02;

	TCNT1 = 0;

	_avr_timer_cntcurr = _avr_timer_M;

	SREG |= 0x80;
}

void TimerOff() {
	TCCR1B = 0x00;
}

void TimerISR() {
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect) {
	_avr_timer_cntcurr--;
	if (_avr_timer_cntcurr == 0) {
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet (unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

enum States { Initial, Start, One, Two, Three, Four, Pause, Reset } State;
unsigned char but = 0x00; //button

void TickFct() {
	switch(State) {
		case Initial:
			State = Start;
		break;

		case Start:
			State = One;
		break;

		case One:
			if(but) {
				State = Pause;
			}
			else {
				State = Two;
			}
		break;

		case Two:
			if(but) {
				State = Pause;
			}
			else {
				State = Three;
			}
		break;

		case Three:
			if(but) {
				State = Pause;
			}
			else {
				State = Four;
			}
		break;

		case Four:
			if(but) {
				State = Pause;
			}
			else {
				State = One;
			}
		break;

		case Pause:
			if(but) {
				State = Pause;
			}
			else {
				State = Reset;
			}
		break;

		case Reset:
			if(but) {
				State = One;
			}
			else {
				State = Reset;
			}
		break;

		default:
		State = Start;
		break;

	}

	switch(State) {
		case Initial:
		break;

		case Start:
		break;

		case One:
		PORTB = 0x01;
		break;

		case Two:
		PORTB = 0x02;
		break;

		case Three:
		PORTB = 0x04;
		break;
		
		case Four:
		PORTB = 0x02;
		break;

		case Pause:
		break;

		case Reset:
		break;

		default:
		break;
	}
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	TimerSet(300);
	TimerOn();
	
	State = Start;
    /* Insert your solution below */
    while (1) {
	
	but = ~PINA & 0x01; // but = button
	TickFct();
	while (!TimerFlag);
	TimerFlag = 0;	
    }
    return 1;
}
