#ifndef TRANSFORM_H
#define TRANSFORM_H

#pragma once

#include "Definitions.h"

class Entity;

struct CPPRENDER_API Transform
{
	Transform( Point3 Position, vec3 Scale, quat Rotation );

	Point3	Position;
	vec3	Scale;
	quat	Rotation;

	void	SetParent( Entity *e );
	mat4	GetMatrix();

	vec3	LocalToWorldDirection( vec3 v );
	Point3	LocalToWorldPoint( Point3 p );
	Point2	LocalToWorldPoint( Point2 p );
	vec3	WorldToLocalDirection( vec3 v );
	Point3	WorldToLocalPoint( Point3 p );
	Point2	WorldToLocalPoint( Point2 p );

private:
	Entity *ParentEntity;
	mat4 ParentMatrix();

};

#endif