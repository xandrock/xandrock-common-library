#ifndef __VECTOR_H
#define __VECTOR_H

#include <math.h>
/*
     VECTOR.H

     CVector class

     OpenGL Game Programming
     by Kevin Hawkins and Dave Astle

     Some operators of the CVector class based on
     operators of the CVector class by Bas Kuenen.
     Copyright (c) 2000 Bas Kuenen. All Rights Reserved.
     homepage: baskuenen.cfxweb.net
*/
struct Vector4f
{
	float x;
	float y;
	float z;
	float w;
};
struct Vector2f
{
	float x;
	float y;
};
struct Vector4c
{
	char x;
	char y;
	char z;
	char w;
};

class CVector
{
public:
	float x;
	float y;
	float z;    // x,y,z coordinates

public:
	CVector(float a = 0, float b = 0, float c = 0) : x(a), y(b), z(c) {}
	CVector(const CVector &vec) : x(vec.x), y(vec.y), z(vec.z) {}

	inline void SetZero()
	{
		x = y = z = 0.0f;
	}

	// vector index
	inline float &operator[](const long idx)
	{
		return *((&x)+idx);
	}

	// vector assignment
	inline const CVector &operator=(const CVector &vec)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;

		return *this;
	}

	// vecector equality
	inline const bool operator==(const CVector &vec) const
     {
          return ((x == vec.x) && (y == vec.y) && (z == vec.z));
     }

	// vecector inequality
	inline const bool operator!=(const CVector &vec) const
     {
          return !(*this == vec);
     }

	//vector add
	inline const CVector operator+(const CVector &vec) const
	{
	  return CVector(x + vec.x, y + vec.y, z + vec.z);
	}

	// vector add (opposite of negation)
	const CVector operator+() const
     {    
          return CVector(*this);
     }

	// vector increment
	const CVector& operator+=(const CVector& vec)
	{
		x += vec.x;
		y += vec.y;
		z += vec.z;
		return *this;
	}

	// vector subtraction
	inline const CVector operator-(const CVector& vec) const
     {    
          return CVector(x - vec.x, y - vec.y, z - vec.z);
     }
	// vector negation
	inline const CVector operator-() const
     {    
          return CVector(-x, -y, -z);
     }

	// vector decrement
	const CVector &operator-=(const CVector& vec)
     {
          x -= vec.x;
          y -= vec.y;
          z -= vec.z;

          return *this;
     }

	// scalar self-multiply
	const CVector &operator*=(const float &s)
     {
          x *= s;
          y *= s;
          z *= s;
          
          return *this;
     }

	// scalar self-divecide
	const CVector &operator/=(const float &s)
     {
          const float recip = 1/s; // for speed, one divecision

          x *= recip;
          y *= recip;
          z *= recip;

          return *this;
     }

	// post multiply by scalar
	inline const CVector operator*(const float &s) const
	{
		return CVector(x*s, y*s, z*s);
	}

	// pre multiply by scalar
	friend inline const CVector operator*(const float &s, const CVector &vec)
	{
		return vec*s;
	}

	inline const CVector operator*(const CVector& vec) const
	{
		return CVector(x*vec.x, y*vec.y, z*vec.z);
	}

	// post multiply by scalar
	/*friend inline const CVector operator*(const CVector &vec, const float &s)
	{
		return CVector(vec.x*s, vec.y*s, vec.z*s);
	}*/

	// divide by scalar
	const CVector operator/(float s) const
     {
          s = 1/s;

          return CVector(s*x, s*y, s*z);
     }

	// cross product
	inline const CVector CrossProduct(const CVector &vec) const
	{
		return *this ^ vec;
	}

	// cross product
	inline const CVector operator^(const CVector &vec) const
	{
		return CVector(y*vec.z - z*vec.y, z*vec.x - x*vec.z, x*vec.y - y*vec.x);
	}

	// dot product
	inline const float DotProduct(const CVector &vec) const
	{
		return *this % vec;
	}

	// dot product
	const float operator%(const CVector &vec) const
	{
		return x*vec.x + y*vec.y + z*vec.z;
	}

	// length of vector
	inline const float Length() const
     {
          return (float)sqrt((double)(x*x + y*y + z*z));
     }

	// return the unit vector
	const CVector UnitVector() const
     {
          return (*this) / Length();
     }

	// normalize this vector
	void Normalize()
     {
          (*this) /= Length();
     }

	const float operator!() const
     {
          return sqrtf(x*x + y*y + z*z);
     }

	// return vector with specified length
	const CVector operator | (const float length) const
     {
          return *this * (length / !(*this));
     }

	// set length of vector equal to length
	const CVector& operator |= (const float length)
     {
          return *this = *this | length;
     }

	// return angle between two vectors
	const float inline Angle(const CVector& normal) const
     {
          return acosf(*this % normal);
     }

	// reflect this vector off surface with normal vector
	const CVector inline Reflection(const CVector& normal) const
     {    
          const CVector vec(*this | 1);     // normalize this vector
          return (vec - normal * 2.0 * (vec % normal)) * !*this;
     }

	// rotate angle degrees around a normal
	const CVector inline Rotate(const float angle, const CVector& normal) const
	{	
		const float cosine = cos(angle);
		const float sine = sin(angle);

		return CVector(*this * cosine + ((normal * *this) * (1.0f - cosine)) *
			          normal + (*this ^ normal) * sine);
	}

	static CVector GetLinePosInPlane(const CVector &planePoint, const CVector &planeNormal, const CVector &linePoint, const CVector &lineDir)
	{
		CVector result(0.0, 0.0, 0.0);
		float vpt = planeNormal.DotProduct(lineDir);
		if (vpt != 0.0f)
		{
			float t = ((planePoint.x - linePoint.x) * planeNormal.x + (planePoint.y - linePoint.y) * planeNormal.y + (planePoint.z - linePoint.z) * planeNormal.z) / vpt;
			result = linePoint + lineDir * t;
		}
		return result;
	}

	static bool GetShortestBridge(const CVector &lineAPoint1, const CVector &lineAPoint2, const CVector &lineBPoint1, const CVector &lineBPoint2, CVector &bridgePointA, CVector &bridgePointB)
	{
		CVector lineADir = lineAPoint2 - lineAPoint1;
		CVector lineBDir = lineBPoint2 - lineBPoint1;
		lineADir.Normalize();
		lineBDir.Normalize();
		double dotResult = lineADir.DotProduct(lineBDir);

		if(dotResult == 0.0)
		{
			return false;
		}
		CVector abNormal = lineADir.CrossProduct(lineBDir);
		abNormal.Normalize();
		CVector aPlaneNormal = abNormal.CrossProduct(lineADir);
		bridgePointB =  GetLinePosInPlane(lineAPoint1, aPlaneNormal, lineBPoint1, lineBDir);
		CVector bPlaneNormal = abNormal.CrossProduct(lineBDir);
		bridgePointA =  GetLinePosInPlane(lineBPoint1, bPlaneNormal, lineAPoint1, lineADir);
		return true;

	}
};

#endif
