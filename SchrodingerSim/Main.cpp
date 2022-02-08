#include <CPPRenderInterface.h>
#include "Line.h"
#include "Complex.h"

const double h = 1.054571817e-34;
const double n = 5.0;
const double m = 1e-31;


double dt;
bool ProbDensity = false;
float CameraRadius = 5.0;
Point3 CameraDirection = Point3( 0, 0, 1 );

void KeyResponse( GLFWwindow *window, int key, int scancode, int action, int mods );
int64 Factorial( int64 n );

double DisplayXValue = 0.0;
Transform *Camera = new Transform( CameraDirection * CameraRadius, one<vec3>(), identity<quat>() );

int main()
{
	float t0;

	Window *w = new Window( 720, 500, "window" );
	Line *l1 = new Line( -5, 5, [] ( double x, double t ) 
	{
		double f = 1.0;
		return
			pow( 
				x * exp( -m * f * x * x / 2 / h / n ) 
				- ( h / m / f ) * ( -m * f * x * x / 2 / h / n ) * exp( -m * f * x * x / 2 / h / n ), n )
			* pow( m * f / pi<double>() / h, .25 )
			* pow( m * f / 2 / h, n / 2 )
			/ sqrt( Factorial( n ) )
			* exp( -1i * t * ( n + .5 ) * f );
	});
	Line *l2 = new Line( -5, 5, [] ( double x, double t )
	{
		double f = 0.9;
		return
			pow(
				x * exp( -m * f * x * x / 2 / h / n )
				- ( h / m / f ) * ( -m * f * x * x / 2 / h / n ) * exp( -m * f * x * x / 2 / h / n ), n )
			* pow( m * f / pi<double>() / h, .25 )
			* pow( m * f / 2 / h, n / 2 )
			/ sqrt( Factorial( n ) )
			* exp( -1i * t * ( n + .5 ) * f );
	} );
	Line *l3 = new Line( -5, 5, [&l1, &l2] ( double x, double t )
	{
		return ( *l1 )[ x ] + ( *l2 )[ x ];
	} );

	//l1->t -= .3;

	Entity *e = new Entity( vec2( -.03f, -.03f ), vec2( .03f, .03f ), 
							Texture::Find( "white.png" ), Texture::Find( "white.png" ), 
							Transform( zero<Point3>(), one<vec3>(), identity<quat>() ) );

	Entity *bounds[ 6 ] =
	{
		new Entity( vec2( -5, -5 ), vec2( 5, 5 ), Texture::Find( "side.png" ), Texture::Find( "side.png" ),
			Transform( Point3( 0, 0, -5 ), one<vec3>(), identity<quat>() ) ),
		new Entity( vec2( -5, -5 ), vec2( 5, 5 ), Texture::Find( "side.png" ), Texture::Find( "side.png" ),
			Transform( Point3( 0, 0, +5 ), one<vec3>(), identity<quat>() ) ),
		new Entity( vec2( -5, -5 ), vec2( 5, 5 ), Texture::Find( "side.png" ), Texture::Find( "side.png" ),
			Transform( Point3( -5, 0, 0 ), one<vec3>(), rotate( identity<quat>(), radians( +90.0f ), vec3( 0, 1, 0 ) ) ) ),
		new Entity( vec2( -5, -5 ), vec2( 5, 5 ), Texture::Find( "side.png" ), Texture::Find( "side.png" ),
			Transform( Point3( +5, 0, 0 ), one<vec3>(), rotate( identity<quat>(), radians( -90.0f ), vec3( 0, 1, 0 ) ) ) ),
		new Entity( vec2( -5, -5 ), vec2( 5, 5 ), Texture::Find( "bottom.png" ), Texture::Find( "bottom.png" ),
			Transform( Point3( 0, -5, 0 ), one<vec3>(), rotate( identity<quat>(), radians( +90.0f ), vec3( 1, 0, 0 ) ) ) ),
		new Entity( vec2( -5, -5 ), vec2( 5, 5 ), Texture::Find( "top.png" ), Texture::Find( "top.png" ),
			Transform( Point3( 0, +5, 0 ), one<vec3>(), rotate( identity<quat>(), radians( -90.0f ), vec3( 1, 0, 0 ) ) ) ),
	};

	w->SetKeyboardCallback( &KeyResponse );

	t0 = GetTime();
	while ( w->IsOpen() )
	{
		dt = GetTime() - t0;
		t0 += dt;

		Camera->Position = CameraDirection * CameraRadius;
		Camera->Rotation = quatLookAt( normalize( -Camera->Position ), vec3( 0, 1, 0 ) );
		w->StartFrame( Camera );

		l1->Update( ProbDensity, dt );
		//l1->Render( w ); 
		l2->Update( ProbDensity, dt );
		//l2->Render( w );
		l3->Update( ProbDensity, dt );
		l3->Render( w );

		vec3 DisplayPoint;
		if ( !ProbDensity )
		{
			DisplayPoint = vec3( DisplayXValue, ( *l1 )[ DisplayXValue ].a, ( *l1 )[ DisplayXValue ].b );
		}
		else
		{
			Complex c1 = ( *l1 )[ DisplayXValue ];
			Complex c2 = Complex( c1.a, -c1.b );
			DisplayPoint = vec3( DisplayXValue, ( c1 * c2 ).a, ( c1 * c2 ).b );
		}
		e->transform.Position = DisplayPoint;
		printf( "x: %f, y: %f %c %fi\n", DisplayXValue, DisplayPoint.y, DisplayPoint.z < 0 ? '-' : '+', abs( DisplayPoint.z ) );
		w->RenderObject( e );

		for ( int i = 6; --i >= 0; )
			w->RenderObject( bounds[ i ] );

		w->EndFrame();

	}

	for ( int i = 6; --i >= 0; )
		delete bounds[ i ];
	delete l1;
	delete l2;
	delete w;
	delete Camera;
}

void KeyResponse( GLFWwindow *window, int key, int scancode, int action, int mods )
{
	if ( action == 2 || action == 1 )
	{
		switch ( key )
		{
			case 263: //left
				DisplayXValue -= .01;
				break;
			case 262: //right
				DisplayXValue += .01;
				break;

			case 65: //A
				CameraDirection = rotate( identity<mat4>(), -(float)dt, vec3( 0, 1, 0 ) ) * vec4( CameraDirection, 0 );
				break;
			case 68: //D
				CameraDirection = rotate( identity<mat4>(), +(float)dt, vec3( 0, 1, 0 ) ) * vec4( CameraDirection, 0 );
				break;
			case 87: //W
				CameraRadius -= 5 * dt;
				break;
			case 83: //S
				CameraRadius += 5 * dt;
				break;
			case 81: //Q
				CameraDirection = rotate( identity<mat4>(), +(float)dt, Camera->LocalToWorldDirection( vec3( 1, 0, 0 ) ) ) * vec4( CameraDirection, 0 );
				break;
			case 69: //E
				CameraDirection = rotate( identity<mat4>(), -(float)dt, Camera->LocalToWorldDirection( vec3( 1, 0, 0 ) ) ) * vec4( CameraDirection, 0 );
				break;

			case 32: //Space
				break;
		}
	}

	if ( action == 1 && key == 32 ) //pressed space
	{
		ProbDensity = !ProbDensity;
	}
}

int64 Factorial( int64 n )
{
	if ( n <= 1 )
		return 1;
	return Factorial( n - 1 ) * n;
}