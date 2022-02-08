#include "Transform.h"
#include "Entity.h"

Transform::Transform( Point3 Position, vec3 Scale, quat Rotation ) :
	Position( Position ), Scale( Scale ), Rotation( Rotation ), ParentEntity( NULL )
{
}

void Transform::SetParent( Entity *e )
{
	ParentEntity = e;
}
mat4 Transform::GetMatrix()
{
	mat4 mPos = translate( mat4( 1 ), Position );
	mat4 mScl = scale( mat4( 1 ), Scale );
	mat4 mRot = glm::mat4_cast( Rotation );
	if ( ParentEntity )
		return mPos * mRot * mScl * ParentMatrix();

	return mPos * mRot * mScl;
}



vec3 Transform::LocalToWorldDirection( vec3 v )
{
	return GetMatrix() * vec4( v, 0 );
}
Point3 Transform::LocalToWorldPoint( Point3 p )
{
	return GetMatrix() * vec4( p, 1 );
}
Point2 Transform::LocalToWorldPoint( Point2 p )
{
	return GetMatrix() * vec4( p, 0, 1 );
}
vec3 Transform::WorldToLocalDirection( vec3 v )
{
	return inverse( GetMatrix() ) * vec4( v, 0 );
}
Point3 Transform::WorldToLocalPoint( Point3 p )
{
	return inverse( GetMatrix() ) * vec4( p, 1 );
}
Point2 Transform::WorldToLocalPoint( Point2 p )
{
	return inverse( GetMatrix() ) * vec4( p, 0, 1 );
}

mat4 Transform::ParentMatrix()
{
	return ParentEntity->transform.GetMatrix();
}