#ifndef LINE_H
#define LINE_H

#pragma once

#include <functional>
#include <CPPRenderInterface.h>
#include "Complex.h"

#define SEGMENTS 2048


class Line
{
public:
	Line( double b, double a, std::function<Complex( double, double )> Function );
	~Line();

	std::function<Complex( double, double )>	Function;
	double										b;
	double										a;
	double										t;

	void										Update( bool ProbDensity, float dt );
	void										Render( Window *w );

	TwoPointLine								*segments[ SEGMENTS ];

	Complex operator[] ( double x )				{ return Function( x, t ); }

};

#endif