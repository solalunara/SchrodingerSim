#include "Complex.h"
#include <CPPRenderInterface.h>

Complex::Complex( float64 a, float64 b /*= 0*/ ) : a( a ), b( b )
{}

std::string Complex::ToString()
{
	return std::to_string( a ) + " + " + std::to_string( b ) + "i";
}

Complex operator +( Complex a )
{
	return a;
}
Complex operator -( Complex a )
{
	return 0 - a;
}
Complex operator +( Complex a, Complex b )
{
	return Complex( a.a + b.a, a.b + b.b );
}
Complex operator -( Complex a, Complex b )
{
	return Complex( a.a - b.a, a.b - b.b );
}
Complex operator *( Complex a, Complex b )
{
	//quaternion multiplication
	//return Complex( a.a * b.a - a.b * b.b - a.c * b.c - a.d * b.d, 
	//				a.a * b.b + a.b * b.a + a.c * b.d - a.d * b.c,
	//				a.a * b.c - a.b * b.d + a.c * b.a + a.d * b.b,
	//				a.a * b.d + a.b * b.c - a.c * b.b + a.d * b.a );
	return Complex( a.a * b.a - a.b * b.b, a.b * b.a + a.a * b.b );
}
Complex operator /( Complex a, Complex b )
{
	Complex Inverse = Complex( b.a, -b.b ) * ( 1 / ( b.a * b.a + b.b * b.b ) );
	return a * Inverse;
}

void operator +=( Complex &a, Complex b )
{
	a = a + b;
}
void operator -=( Complex &a, Complex b )
{
	a = a - b;
}
void operator *=( Complex &a, Complex b )
{
	a = a * b;
}
void operator /=( Complex &a, Complex b )
{
	a = a / b;
}

Complex operator "" i( long double d )
{
	return Complex( 0, d );
}
Complex operator "" i( unsigned long long i )
{
	return Complex( 0, i );
}

Complex exp( Complex z )
{
	return pow( e<float64>(), z.a ) * ( cos( z.b ) + sin( z.b ) * 1i );
}

Complex pow( float64 a, Complex b )
{
	return exp( log( a ) * b );
}

Complex pow( Complex a, Complex b )
{
	float64 r = sqrt( a.a * a.a + a.b * a.b );
	float64 t = atan( a.b / a.a );
	return exp( log( r ) * b + b * t * 1i );
}