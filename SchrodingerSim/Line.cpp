#include "Line.h"

Line::Line( double b, double a, std::function<Complex( double, double )> Function ) :
	segments{ NULL }, Function( Function ), a( a ), b( b ), t( 0.0 )
{
	double Size = ( a - b ) / SEGMENTS;
	double ix;
	for ( int i = 0; i != SEGMENTS; ++i )
	{
		ix = (double)i / SEGMENTS * ( a - b ) + b;
		segments[ i ] = new TwoPointLine( 
			dvec3( ix, Function( ix, t ).a, Function( ix, t ).b ), 
			dvec3( ix + Size, Function( ix + Size, t ).a, Function( ix + Size, t ).b ) );
	}
}
Line::~Line()
{
	for ( int i = SEGMENTS; --i >= 0; )
		delete segments[ i ];
}

void Line::Render( Window *w )
{
	for ( int i = SEGMENTS; --i >= 0; )
		w->DrawLine( segments[ i ], 1.0f );
}
void Line::Update( bool ProbDensity, float dt )
{
	t += dt;

	double Size = ( a - b ) / SEGMENTS;
	double ix;
	for ( int i = 0; i != SEGMENTS; ++i )
	{
		ix = (float)i / SEGMENTS * ( a - b ) + b;
		if ( !ProbDensity )
		{
			segments[ i ]->Update(
				dvec3( ix, Function( ix, t ).a, Function( ix, t ).b ),
				dvec3( ix + Size, Function( ix + Size, t ).a, Function( ix + Size, t ).b ) );
		}
		else
		{
			Complex c1 = Function( ix, t );
			Complex c2 = Complex( c1.a, -c1.b );
			Complex c3 = Function( ix + Size, t );
			Complex c4 = Complex( c3.a, -c3.b );

			segments[ i ]->Update(
				dvec3( ix, ( c1 * c2 ).a, ( c1 * c2 ).b ),
				dvec3( ix + Size, ( c3 * c4 ).a, ( c3 * c4 ).b ) );
		}
	}
}