#include "stdafx.h"

#include "Curl.h"
#include "Constants.h"

#include <cmath>
#include <gl\gl.h>
#include <gl\glu.h>

inline void drawVertexAt(const smr::Vector3f& vector)
{
	glVertex3f(vector.x, vector.y, 0.0f);
}

Curl::Curl(Direction direction)
	:	age_(0),
		direction_(direction),
		drawDebuggingCircle_(false),
		angle_(0.0f),
		maxLength_(pi * 2),
		duration_(13.0f),
		maxWidth_(0.25f)
{
}

smr::Vector3f Curl::getPositionAtLength(float angle) const
{
	smr::Vector3f direction=smr::Vector3f(sin(angle), cos(angle), 0.0f);
	float distanceFromCenterPoint = distanceFromCenter(angle);
	return direction*distanceFromCenterPoint;
}

float Curl::distanceFromCenter(float angle) const
{
	float revolutions = angle/radsInCircle;
	return (1.0f - revolutions);
}

void Curl::draw(float deltaTime, bool noDraw)
{
	age_ += deltaTime;

	float lengthInRadians = currentLength();

	float fractionAnimationComplete = lengthInRadians / maxLength_;

	float currentWidth = maxWidth_;
	currentWidth = fractionAnimationComplete * currentWidth;

	for(float radians=0; radians<=lengthInRadians; radians+=0.1f)
	{
		float fractionComplete = radians / lengthInRadians;
		float currentHalfWidth = (1-fractionComplete) * currentWidth; // n.b. currentHalfWidth isn't an accurate name - it has nothign to do with half.

		float distanceFromCenterPoint = distanceFromCenter(radians);

		float displayRadians = angle_ + (direction_==Clockwise ? radians : - radians);

		// Get the direction as a normalized vector:
		smr::Vector3f direction=smr::Vector3f(sin(displayRadians), cos(displayRadians), 0.0f);

		if(drawDebuggingCircle_)
		{
			if(!noDraw)
			{
				drawVertexAt(direction);
			}
		}

		if(!noDraw)
		{
			smr::Vector3f center = direction * distanceFromCenterPoint;
			drawVertexAt(center);

			smr::Vector3f inside = direction * distanceFromCenterPoint + direction * -currentHalfWidth;
			drawVertexAt(inside);
		}
	}
}

bool Curl::isComplete() const
{
	return age_ >= duration_;
}

float Curl::fractionComplete() const
{
	return (age_ > duration_) ? 1.0f : age_ / duration_;
}

float Curl::currentLength() const
{
	float lengthInRadians = ((pi*2)/duration_) * age_;
	// Limit the length 
	if(lengthInRadians > maxLength_)
	{
		lengthInRadians=maxLength_;
	}
	return lengthInRadians;
}

float Curl::baseWidth() const
{
	return fractionComplete() * maxWidth_;
}

PositionedCurl::PositionedCurl(float attachmentPoint, Direction direction) : Curl(direction), attachmentPoint_(attachmentPoint)
{
}

PositionedCurl::PositionedCurl() : attachmentPoint_(0.25f)
{
}

PositionedCurl::~PositionedCurl()
{
}

float PositionedCurl::getAngleForAttachmentPoint() const
{
	return currentLength()*attachmentPoint_;
}