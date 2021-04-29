/* Author: 
 * Lab Section:
 * Assignment: Lab 6  Exercise 3
 * Exercise Description: [optional - include for your own benefit]
 *
 * I acknowledge all content contained herein, excluding template or example
 * code is my own original work.
 *
 *  Demo Link: https://youtu.be/z0lTbndyRyk 
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

enum States { Start, Inc, Dec, Reset, IncP, DecP } State;
unsigned char button = 0x00; //button
unsigned char time = 0x00; //time for every tick
unsigned char count = 0x00;

void TickFct() {
	switch(State) {
		case Start:
			if(button == 0x03) {
				State = Reset;
			}
			else if(button == 0x02) {
				State = Dec;
			}
			else if(button == 0x01) {
				State =  Inc;
			}
			else {
				State = Start;
			}
		break;

		case Inc:
			if(button == 0x01) {
				State = IncP;
			}
			else if(button == 0x02) {
				State = Dec;
			}
			else if(button == 0x03) {
				State = Reset;
			}
			else if(!button) {
				State = Start;
			}
		break;

		case Dec:
			if(button == 0x02) {
				State = DecP;
			}
			else if(button == 0x01) {
				State = Inc;
			}
			else if(button == 0x03) {
				State = Reset;
			}
			else if(!button){
				State = Start;
			}
		break;

		case Reset:
			if(!button) {
				State = Start;
			}
			else if(button == 0x01) {
				State = Inc;
			}
			else if(button == 0x02) {
				State = Dec;
			}
			else if(button == 0x03) {
				State = Reset;
			}
		break;

		case IncP:
			if(!button) {
				State = Start;	
			}
			else if(button == 0x01) {
				State = IncP;
			}
			else if(button == 0x02) {
				State = Dec;
			}
			else if(button == 0x03) {
				State = Reset;
			}
		break;

		case DecP:
			if(!button) {
				State = Start;
			}
			else if(button == 0x02) {
				State = DecP;
			}
			else if(button == 0x01) {
				State = Inc;
			}
			else if(button == 0x03) {
				State = Reset;
			}
		break;

		default:
		State = Start;
		break;

	}

	switch(State) {
		case Start:
		break;

		case Inc:
			if(count < 9) {
				count = count + 0x01;
			}
		break;

		case Dec:
			if(count > 0) {
				count = count - 0x01;
			}
		break;

		case Reset:
			count = 0x00;
		break;

		case IncP:
			if(time < 10) {
				time = time + 0x01;
			}
			else {
				time = 0;
				if(count < 9) {
					count = count + 0x01;
				}
			}
		break;

		case DecP:
			if(time < 10) {
				time = time + 0x01;
			}
			else {
				time = 0;
				if(count > 0) {
					count = count - 0x01;
				}
			}
		break;

		default:
		break;
	}
	PORTB = count;
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	TimerSet(100);
	TimerOn();
	
	State = Start;
	count = 0x07;
    /* Insert your solution below */
    while (1) {
	
	button = ~PINA & 0x03; // but = button
	TickFct();
	while (!TimerFlag);
	TimerFlag = 0;	
    }
    return 1;
}
