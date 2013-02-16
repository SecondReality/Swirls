#pragma once

#include "Vector.h"

class Curl
{
public:
	enum Direction { Clockwise, AntiClockwise };
	Curl(Direction=Clockwise);
	
	void draw(float deltaTime, bool noDraw=false);
	
	// Given an angle this function returns the vector position of a point on the curl - considering the curl fits in a unit vector.
	// Note that that angle specified may be greater than 2*pi.
	smr::Vector3f getPositionAtLength(float angle) const;

	// Returns true if the Curl has completed its revolution
	bool isComplete() const;

	// How complete the animation is (from 0 to 1)
	float fractionComplete() const;
		
	float age_; // In seconds
	
	bool drawDebuggingCircle_;
	Direction direction_;
	float angle_; // The angle the base of the curl is at.
	float maxLength_;
	float duration_;
	float maxWidth_;

	// Returns the current length in radians
	float currentLength() const;

	// Returns the width of the base
	float baseWidth() const;

private:
	// Given an angle return the distance from the center of the curl.
	// Note that that angle specified may be greater than 2*pi.
	float distanceFromCenter(float angle) const;

};

class PositionedCurl : public Curl
{
public:
	PositionedCurl();
	PositionedCurl(float attachmentPoint, Direction=Clockwise);
	~PositionedCurl();

	//smr::Vector3f position_;
	float attachmentPoint_; // Attachment point is a float in the range 0 to 1, which represents the position on the parent curl which this curl is growing out of. As the parent curl grows it will move its children nodes. 
	float getAngleForAttachmentPoint() const;
};
