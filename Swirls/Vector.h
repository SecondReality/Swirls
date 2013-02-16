#pragma once

//		author: Steven Mark Rose

namespace smr {

class Vector3
{
public:

  // Constructors:
  Vector3();
  Vector3(const Vector3 & vector);
  Vector3(float X, float Y, float Z);

  // Accessor Methods:
  void assign(const Vector3& inputVector);
  void assign(const float& X, const float &Y, const float &Z);

  float magnitude() const;
  float magnitude_squared() const;

  Vector3& normalize();
  float dot(const Vector3&) const;
  Vector3 cross(const Vector3&) const;
  float distance(const Vector3&, const Vector3&);

  // Internal Operator Methods:

  const bool operator ==(const Vector3 &) const;
  const bool operator !=(const Vector3 &) const;

  const Vector3 operator +(const Vector3 &) const;
  const Vector3 operator +() const;
  const Vector3& operator +=(const Vector3 &);

  Vector3 operator -(const Vector3 &) const;
  Vector3 operator -() const;
  const Vector3& operator -=(const Vector3 &);

  const Vector3 operator *(const Vector3 &) const;
  const Vector3& operator *=(const Vector3 &);

  const Vector3 operator *(const float &) const;
  const Vector3& operator *=(const float &);

  const Vector3 operator /(const Vector3 &) const;
  const Vector3& operator /=(const Vector3 &);

  const Vector3 operator /(const float &) const;
  const Vector3& operator /=(const float &);
  
  // Cross product
  const Vector3 operator^(const Vector3 &) const;
  
  // Dot product
  const float operator%(const Vector3 &) const;

  // Data Members:
  union
  {
    struct
    {
      float x;
      float y;
      float z;
    };
    float v[3];
  };

};

template < class T >
class Vector2
{
public:
	typedef T type;

  // Constructors:
	Vector2(){}
	Vector2(const Vector2<type> & vin):
		x(vin.x), y(vin.y)
	{
	}
	Vector2(type X, type Y):
	  x(X), y(Y)
	{
	}
  
	void assign(T xIn, T yIn)
	{
		x=xIn;
		y=yIn;
	}
  // Data Members:
  union
  {
    struct
    {
      type x;
      type y;
    };
    type v[2];
  };
};

typedef Vector3	Vector3f;
typedef Vector2<float>	Vector2f;

}