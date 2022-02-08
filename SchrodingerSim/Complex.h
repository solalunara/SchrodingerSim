#ifndef COMPLEX_H
#define COMPLEX_H

#pragma once

#include <string>



typedef double float64;
class Complex
{
public:
	Complex( float64 a, float64 b = 0 );

	float64 a;
	float64 b;

	std::string ToString();
};

Complex operator +( Complex a );
Complex operator -( Complex a );
Complex operator +( Complex a, Complex b );
Complex operator -( Complex a, Complex b );
Complex operator *( Complex a, Complex b );
Complex operator /( Complex a, Complex b );

void operator +=( Complex &a, Complex b );
void operator -=( Complex &a, Complex b );
void operator *=( Complex &a, Complex b );
void operator /=( Complex &a, Complex b );

#pragma warning( disable : 4455 )
Complex operator "" i( long double d );
Complex operator "" i( unsigned long long i );

Complex exp( Complex z );
Complex pow( float64 a, Complex b );
Complex pow( Complex a, Complex b );

#endif