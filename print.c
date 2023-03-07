/*
 * File:   print.c
 * Author: martin
 *
 * Created on March 1, 2023, 1:35 PM
 */


#include <xc.h>
#include "print.h"

#define false 0
#define true 1

const unsigned char nibble2hex[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};


void print_byte_hex(unsigned char indata) {
   putchar( nibble2hex[0x0F & (indata>>4)] );
   putchar( nibble2hex[0x0F & (indata)] );
}

void print_int_hex(unsigned int indata) {
   print_byte_hex((unsigned char) indata>>8);
   print_byte_hex((unsigned char) indata&0x00FF);
}

void print_crlf(void ) {
   putchar(13);
   putchar(10);
}



void print_byte_binary(unsigned char d) {
	unsigned char i, indata;
    
    indata = d;
	
	putchar('b');
    for (i = 0; i < 8; i++) {
        if (i==4) putchar('_');
        if ((indata & 0x80) == 0) 
            putchar('0');
        else 
            putchar('1');
        indata <<= 1;
    }
}



// Print an INT with 5 digits, no decimals
void print_int_dec(unsigned int indata) {
   _print_universal_dec( indata, 10000, 5, 0);
}

// Print an INT with 5 digits, one decimal
void print_int_dec_1(unsigned int indata) {
   _print_universal_dec( indata, 10000, 5, 1);
}

// Print an uchar with 3 digits, no decimals
void print_byte_dec(unsigned char indata) {
   _print_universal_dec( indata, 100, 3, 0);
}

// Print an uchar with 2 digits, no decimals
void print_byte_2dec(unsigned char indata) {
   _print_universal_dec( indata, 100, 2, 0);
}

// Print an uchar with 3 digits, one decimal
void print_byte_dec_1(unsigned char indata) {
   _print_universal_dec( indata, 100, 3, 1);
}

void _print_universal_dec(unsigned int indata, unsigned int digit_divisor, unsigned char digit_number, unsigned char decimals)  {
   unsigned char digit;
   unsigned char no_digits_printed_yet;
   unsigned char number_of_digits = 0;
   
	if ( indata == 0 )
	{
		if (digit_number > 2) 
			putchar(' ');
		putchar(' ');
		putchar('0');
		if (decimals != 0) 
		{
			putchar('.'); 
			putchar('0');
		}
		return;
	}

	if (( indata < 10 ) && (decimals != 0) )
	{
		putchar('0');
	}

	if (( indata < 10 ) && ( indata > 0 ) )
	{
		putchar(' ');
//		putchar('0');
	}

	no_digits_printed_yet = true;

	while (digit_divisor > 0)
	{
//		if ((decimals > 0) && (indata < 10))
		if ((decimals > 0) && (digit_divisor == 1))
			putchar('.');
		digit = (unsigned char)  ( indata / digit_divisor );
		indata = indata % digit_divisor;
		digit_divisor = digit_divisor / 10;
      
		if ((digit != 0) | (no_digits_printed_yet == false))
		{
			putchar( digit | '0');
			no_digits_printed_yet = false;
		}
		digit_number--;
	}
}


void putstr( char *s ) {
   	char c;

   	while ( (c = *s++) ) // wait for the \0 terminator
   	{
	   	if (c == 10) 
			putchar('\r'); // add carrige return if line feed is found
	   	putchar(c);
   	}
}





