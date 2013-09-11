#pragma once
#include "cinder/gl/gl.h"
#include "cinder/Vector.h"

using namespace ci;

class Particle
{
public:

	Particle( const ci::Vec2f& position, float radius,
		float mass, float drag );
	void update();
	void draw();
	ci::Vec2f position, prevPosition;
	ci::Vec2f forces;
	float radius;
	float mass;
	float drag;
	ci::Vec2f anchor;
private:
};