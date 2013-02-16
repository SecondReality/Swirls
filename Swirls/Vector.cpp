#include "stdafx.h"
#include "Vector.h"
#include <cmath>

using smr::Vector3;

Vector3::Vector3()
{
};

Vector3::Vector3(const Vector3 & inputVector)
{
  x=inputVector.x;
  y=inputVector.y;
  z=inputVector.z;
}

Vector3::Vector3(float X, float Y, float Z): x(X), y(Y), z(Z)
{
}

inline void Vector3::assign(const Vector3& inputVector)
{
  x=inputVector.x;
  y=inputVector.y;
  z=inputVector.z;
}

void Vector3::assign(const float &X, const float &Y, const float &Z)
{
  x=X;
  y=Y;
  z=Z;
}

inline float Vector3::magnitude() const
{
  return (float)sqrt(x*x + y*y + z*z);
}

inline float Vector3::magnitude_squared() const
{
  return (x*x + y*y + z*z);
}

Vector3& Vector3::normalize()
{
  float m=1.0f/magnitude();
  x *= m;
  y *= m;
  z *= m;
  return *this;
}

float Vector3::dot( const Vector3 & v ) const
{
	return x*v.x + y*v.y + z*v.z;
}

Vector3 Vector3::cross( const Vector3 & v) const
{
  return Vector3
    (
    y*v.z - z*v.y,
    z*v.x - x*v.z,
    x*v.y - y*v.x
    );
}

inline float Vector3::distance( const Vector3& vA, const Vector3& vB)
{
	Vector3 distance_vector(vB.x - vA.x, vB.y - vA.y, vB.z - vA.z );
	return distance_vector.magnitude();
}

const bool Vector3::operator ==(const Vector3 & vIn) const
{
  return (x==vIn.x && y==vIn.y && z==vIn.z);
}

const bool Vector3::operator !=(const Vector3 & vIn) const
{
  return (x!=vIn.x && y!=vIn.y && z!=vIn.z);
}

const Vector3 Vector3::operator +(const Vector3 & vIn) const
{
	return smr::Vector3(x+vIn.x, y+vIn.y, z+vIn.z);
}

inline const Vector3 Vector3::operator +() const
{
  return *this;
}

const Vector3& Vector3::operator +=(const Vector3 & vIn)
{
  x+=vIn.x;
  y+=vIn.y;
  z+=vIn.z;
  return *this;
}

Vector3 Vector3::operator -(const Vector3 & vIn) const
{
	return smr::Vector3(x-vIn.x, y-vIn.y, z-vIn.z);
}

Vector3 Vector3::operator -() const
{
  return Vector3(-x,-y,-z);
}

const Vector3& Vector3::operator -=(const Vector3 & vIn)
{
  x-=vIn.x;
  y-=vIn.y;
  z-=vIn.z;
  return *this;
}

inline const Vector3 Vector3::operator *(const Vector3 & vIn) const
{
  return Vector3(x*vIn.x, y*vIn.y, z*vIn.z);
}

const Vector3 Vector3::operator *(float const & fIn) const
{
  return Vector3(x*fIn, y*fIn, z*fIn);
}

const Vector3& Vector3::operator *=(const Vector3 & vIn)
{
  x*=vIn.x;
  y*=vIn.y;
  z*=vIn.z;
  return *this;
}

const Vector3& Vector3::operator *=(const float & fIn)
{
  x*=fIn;
  y*=fIn;
  z*=fIn;
  return *this;
}

inline const Vector3 Vector3::operator /(const Vector3 & vIn) const
{
  return Vector3(x*vIn.x, y*vIn.y, z*vIn.z);
}

inline const Vector3& Vector3::operator /=(const Vector3 & vIn)
{
  x/=vIn.x;
  y/=vIn.y;
  z/=vIn.z;
  return *this;
}

const Vector3 Vector3::operator /(const float & fIn) const
{
  float Inverse=1.0f/fIn;
  return Vector3(x*Inverse, y*Inverse, z*Inverse);
}

const Vector3& Vector3::operator /=(const float & fIn)
{
  x/=fIn;
  y/=fIn;
  z/=fIn;
  return *this;
}

inline const Vector3 Vector3::operator^(const Vector3 & vIn) const
{
  return Vector3
    (
    y*vIn.z - z*vIn.y,
    z*vIn.x - x*vIn.z,
    x*vIn.y - y*vIn.x
    );
}

inline const float Vector3::operator%(const Vector3 & vIn) const
{
  return x*vIn.x + y*vIn.y + z*vIn.z;
}