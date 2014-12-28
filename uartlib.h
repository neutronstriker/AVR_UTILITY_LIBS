/*
Author : N.Srinivas a.k.a NeutroN StrikeR
Date:19-06-2014
email:striker.dbz[at]hotmail.com
The MIT License (MIT)
Copyright (c) <year> <copyright holders>
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/


#ifndef _UART_LIB_
#define _UART_LIB_ 1

#include<avr/io.h>
#include<avr/pgmspace.h>


int uart_init(unsigned long int BAUD)
{ 

	//in this way if i define the uart_init() i can dynamicly switch baud rate
	//in the program on the fly whenever i need.

	unsigned int UBRR_VAL = ((F_CPU/(16UL * BAUD))-1);
	
	unsigned long int generated_BAUD = (F_CPU/(16UL * (UBRR_VAL+1)));
	int error = 0;
	error = generated_BAUD - BAUD;
	

	if(error < 0)//taking modulus of error
	error = error * -1;

	error = (error * 100UL)/BAUD;
	
	if(error >= 2)//acceptable reciever error is +/-2%
		return 0;//i,e, there is more than tolerable error in baud rate generation
	
	UBRR0L = UBRR_VAL;
	UBRR0H = UBRR_VAL>>8;
	UCSR0B = (1<<TXEN0) | (1<<RXEN0);
	UCSR0C = (1<<UCSZ00) | (1<<UCSZ01); //in atmega328p there is no URSEL bit
		
	return 1; //serial port initialised succesfully

}


unsigned char uart_read()
{
	unsigned char data;
	while(!(UCSR0A & (1<<RXC0)));
	data= UDR0;
	UCSR0A |= (1<<RXC0);
	return data;
}

void uart_write(unsigned char data)
{
	UDR0=data;
	while(!(UCSR0A & (1<<UDRE0)));
	UCSR0A |= (1<<UDRE0);
}

void u_print(char * string)
{
	uint8_t i=0;
	while(string[i] != '\0')
	{
		uart_write(string[i]);
		i++;
	}
}

//The below mentioned definition and function help in saving a lot of RAM by saving
//strings that are to pe printed in ROM instead of RAM
#define u_printPMEM(x) u_print_p(PSTR(x))

void u_print_p(const char *start)
{
	while(pgm_read_byte(start) != '\0')
	{
		uart_write(pgm_read_byte(start++));
	}
}


void hexbyte(unsigned char num)
{
	unsigned char a,b;
	a=num & 0x0f;
	b= ((num>>4) & 0x0f);
	if(a>=0 && a<=9)
		a+=0x30;
	else a+=(0x41-0xA);
	if(b>=0 && b<=9)
		b+=0x30;
	else b+=(0x41-0xA);
	uart_write(b);
	uart_write(a);
	uart_write(' ');
}

void uart_printnum(long int number)
{
	unsigned char temp=0;
	
	if(number == 0)
	{
		uart_write('0');
		return;
	}
	
	if(number < 0)
	{	
		number = number * -1;
		uart_write('-');
	}
		
	while(number > 0)
	{	
		temp = number%10;
		temp += 0x30;
		push(temp);
		number = number/10; 
	}
	
	for(int8_t i=top;i>=0;i--)
		uart_write(stack[i]);
	
	top = -1; // re-initialize stack pointer


}

#endif
