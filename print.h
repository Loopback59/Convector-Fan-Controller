#ifndef PRINT_H
#define	PRINT_H

#include <xc.h>


// bitnumber = 0-7 decimal
#define setbit( var, bitnumber ) 	var |= (1 << bitnumber)
#define clrbit( var, bitnumber )	var &= ~ (1 << bitnumber)
#define cplbit( var, bitnumber )	var ^= ~ (1 << bitnumber)


int putchar(int);
void print_byte_hex(unsigned char indata);
void print_int_hex(unsigned int indata);
void print_crlf(void );
void print_byte_binary(unsigned char d);
void print_int_dec(unsigned int indata);
void print_int_dec_1(unsigned int indata);
void print_byte_dec(unsigned char indata);
void print_byte_2dec(unsigned char indata);
void print_byte_dec_1(unsigned char indata);
void _print_universal_dec(unsigned int indata, unsigned int digit_divisor, unsigned char digit_number, unsigned char decimals) ;
void putstr( char *s );

#endif
