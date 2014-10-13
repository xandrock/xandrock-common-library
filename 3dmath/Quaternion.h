/////////////////////////////////////////////////////////////////////////////
//
// 3D Math Primer for Games and Graphics Development
//
// Quaternion.h - Declarations for class Quaternion
//
// Visit gamemath.com for the latest version of this file.
//
// For more details, see Quaternion.cpp
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __QUATERNION_H_INCLUDED__
#define __QUATERNION_H_INCLUDED__
#include <assert.h>
#include <math.h>

#include "MathUtil.h"
#include "vector.h"
//
//class CVector;
//class EulerAngles;

//---------------------------------------------------------------------------
// class Quaternion
//
// Implement a quaternion, for purposes of representing an angular
// displacement (orientation) in 3D.

class Quaternion {
public:
	float	 x, y, z, w;

// Public operations

	Quaternion(float a = 0, float b = 0, float c = 0, float d = 1)
		: x(a), y(b), z(c), w(d){}

	Quaternion(const Quaternion &q) : x(q.x), y(q.y), z(q.z), w(q.w) {}
	inline float &operator[](const long idx)
	{
		return *((&x)+idx);
	}

	// Set to identity
	void	identity() { w = 1.0f; x = y = z = 0.0f; }

	// Setup the quaternion to a specific rotation

	void	setToRotateAboutX(float theta);
	void	setToRotateAboutY(float theta);
	void	setToRotateAboutZ(float theta);
	void	setToRotateAboutAxis(const CVector &axis, float theta);

	// Setup to perform object<->inertial rotations,
	// given orientation in Euler angle format

	void	setToRotateObjectToInertial(const CVector &orientation);
	void	setToRotateInertialToObject(const CVector &orientation);


	inline const Quaternion &operator=(const Quaternion &vec)
     {
          x = vec.x;
          y = vec.y;
          z = vec.z;
		  w = vec.w;

          return *this;
     }


	// Cross product

	inline Quaternion operator *(const Quaternion &a) const
	{
		Quaternion result;

		result.w = w*a.w - x*a.x - y*a.y - z*a.z;
		result.x = w*a.x + x*a.w + z*a.y - y*a.z;
		result.y = w*a.y + y*a.w + x*a.z - z*a.x;
		result.z = w*a.z + z*a.w + y*a.x - x*a.y;

		return result;
	}
	// Multiplication with assignment, as per C++ convention

	Quaternion &operator *=(const Quaternion &a);

	// Normalize the quaternion.

	void	normalize();

	// Extract and return the rotation angle and axis.

	float	getRotationAngle() const;
	CVector	getRotationAxis() const;

	//rotate a vector
	inline CVector RotateVector(CVector &vec)
	{
		CVector *pV = (CVector*)this;
		return vec + 2.0 * pV->CrossProduct(pV->CrossProduct(vec) + vec * w);
	}
	//convert from a euler angle
	void ConvectEuler(CVector &angle);
};

// A global "identity" quaternion constant

extern const Quaternion kQuaternionIdentity;

// Quaternion dot product.

inline float dotProduct(const Quaternion &a, const Quaternion &b)
{
	return a.w*b.w + a.x*b.x + a.y*b.y + a.z*b.z;
}

// Spherical linear interpolation

extern Quaternion slerp(const Quaternion &p, const Quaternion &q, float t);

// Quaternion conjugation

inline Quaternion conjugate(const Quaternion &q)
{
	return Quaternion( -q.x, -q.y, -q.z, q.w);
}

inline void QuatMulti(Quaternion &qOut, Quaternion &q1, Quaternion &q2)
{	
	qOut.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
	qOut.x = q1.w * q2.x + q1.x * q2.w + q1.z * q2.y - q1.y * q2.z;
	qOut.y = q1.w * q2.y + q1.y * q2.w + q1.x * q2.z - q1.z * q2.x;
	qOut.z = q1.w * q2.z + q1.z * q2.w + q1.y * q2.x - q1.x * q2.y;
}

void QuatSlerp(Quaternion &qOut, const Quaternion &q0, const Quaternion &q1, float t);

// Quaternion exponentiation

extern Quaternion pow(const Quaternion &q, float exponent);

/////////////////////////////////////////////////////////////////////////////
#endif // #ifndef __QUATERNION_H_INCLUDED__
