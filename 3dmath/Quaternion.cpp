////////////////////////////////////////////////////////////////////////////
//
// 3D Math Primer for Games and Graphics Development
//
// Quaternion.cpp - Quaternion implementation
//
// Visit gamemath.com for the latest version of this file.
//
// For more details see section 11.3.
//
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
//
// global data
//
/////////////////////////////////////////////////////////////////////////////

// The global identity quaternion.  Notice that there are no constructors
// to the Quaternion class, since we really don't need any.
//
//const Quaternion kQuaternionIdentity = {
//	1.0f, 0.0f, 0.0f, 0.0f
//};

/////////////////////////////////////////////////////////////////////////////
//
// class Quaternion members
//
/////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------
// Quaternion::setToRotateAboutX
// Quaternion::setToRotateAboutY
// Quaternion::setToRotateAboutZ
// Quaternion::setToRotateAboutAxis
//
// Setup the quaternion to rotate about the specified axis

#include "Quaternion.h"
void	Quaternion::SetToRotateAboutX(float theta) {

	// Compute the half angle

	float	thetaOver2 = theta * .5f;

	// Set the values

	w = cos(thetaOver2);
	x = sin(thetaOver2);
	y = 0.0f;
	z = 0.0f;
}

void	Quaternion::SetToRotateAboutY(float theta) {

	// Compute the half angle

	float	thetaOver2 = theta * .5f;

	// Set the values

	w = cos(thetaOver2);
	x = 0.0f;
	y = sin(thetaOver2);
	z = 0.0f;
}

void	Quaternion::SetToRotateAboutZ(float theta) {

	// Compute the half angle

	float	thetaOver2 = theta * .5f;

	// Set the values

	w = cos(thetaOver2);
	x = 0.0f;
	y = 0.0f;
	z = sin(thetaOver2);
}

void	Quaternion::SetToRotateAboutAxis(const CVector &axis, float theta) {

	// The axis of rotation must be normalized

	assert(fabs(axis.Length() - 1.0f) < .01f);

	// Compute the half angle and its sin

	float	thetaOver2 = theta * .5f;
	float	sinThetaOver2 = sin(thetaOver2);

	// Set the values

	w = cos(thetaOver2);
	x = axis.x * sinThetaOver2;
	y = axis.y * sinThetaOver2;
	z = axis.z * sinThetaOver2;
}

//---------------------------------------------------------------------------
// EulerAngles::setToRotateObjectToInertial
//
// Setup the quaternion to perform an object->inertial rotation, given the
// orientation in Euler angle format
//
// See 10.6.5 for more information.

void	Quaternion::SetToRotateObjectToInertial(const CVector &orientation) {

	// Compute sine and cosine of the half angles

	float	sp, sb, sh;
	float	cp, cb, ch;
	sinCos(&sp, &cp, orientation.x * 0.5f);
	sinCos(&sb, &cb, orientation.y * 0.5f);
	sinCos(&sh, &ch, orientation.z * 0.5f);

	// Compute values

	w =  ch*cp*cb + sh*sp*sb;
	x =  ch*sp*cb + sh*cp*sb;
	y = -ch*sp*sb + sh*cp*cb;
	z = -sh*sp*cb + ch*cp*sb;
}

//---------------------------------------------------------------------------
// EulerAngles::setToRotateInertialToObject
//
// Setup the quaternion to perform an object->inertial rotation, given the
// orientation in Euler angle format
//
// See 10.6.5 for more information.

void	Quaternion::SetToRotateInertialToObject(const CVector &orientation) {

	// Compute sine and cosine of the half angles

	float	sp, sb, sh;
	float	cp, cb, ch;
	sinCos(&sp, &cp, orientation.x * 0.5f);
	sinCos(&sb, &cb, orientation.y * 0.5f);
	sinCos(&sh, &ch, orientation.z * 0.5f);

	// Compute values

	w =  ch*cp*cb + sh*sp*sb;
	x = -ch*sp*cb - sh*cp*sb;
	y =  ch*sp*sb - sh*cb*cp;
	z =  sh*sp*cb - ch*cp*sb;
}

//---------------------------------------------------------------------------
// Quaternion::operator *
//
// Quaternion cross product, which concatonates multiple angular
// displacements.  The order of multiplication, from left to right,
// corresponds to the order that the angular displacements are
// applied.  This is backwards from the *standard* definition of
// quaternion multiplication.  See section 10.4.8 for the rationale
// behind this deviation from the standard.

//Quaternion Quaternion::operator *(const Quaternion &a) const {
//	Quaternion result;
//
//	result.w = w*a.w - x*a.x - y*a.y - z*a.z;
//	result.x = w*a.x + x*a.w + z*a.y - y*a.z;
//	result.y = w*a.y + y*a.w + x*a.z - z*a.x;
//	result.z = w*a.z + z*a.w + y*a.x - x*a.y;
//
//	return result;
//}

//---------------------------------------------------------------------------
// Quaternion::operator *=
//
// Combined cross product and assignment, as per C++ convention

Quaternion &Quaternion::operator *=(const Quaternion &a) {

	// Multuiply and assign

	*this = *this * a;

	// Return reference to l-value

	return *this;
}

//---------------------------------------------------------------------------
// Quaternion::normalize
//
// "Normalize" a quaternion.  Note that normally, quaternions
// are always normalized (within limits of numerical precision).
// See section 10.4.6 for more information.
//
// This function is provided primarily to combat floating point "error
// creep," which can occur when many successive quaternion operations
// are applied.

void	Quaternion::Normalize() {

	// Compute magnitude of the quaternion

	float	mag = (float)sqrt(w*w + x*x + y*y + z*z);

	// Check for bogus length, to protect against divide by zero

	if (mag > 0.0f) {

		// Normalize it

		float	oneOverMag = 1.0f / mag;
		w *= oneOverMag;
		x *= oneOverMag;
		y *= oneOverMag;
		z *= oneOverMag;

	} else {

		// Houston, we have a problem

		assert(false);

		// In a release build, just slam it to something

		Identity();
	}
}

//---------------------------------------------------------------------------
// Quaternion::getRotationAngle
//
// Return the rotation angle theta

float	Quaternion::GetRotationAngle() const {

	// Compute the half angle.  Remember that w = cos(theta / 2)

	float thetaOver2 = safeAcos(w);

	// Return the rotation angle

	return thetaOver2 * 2.0f;
}

//---------------------------------------------------------------------------
// Quaternion::getRotationAxis
//
// Return the rotation axis

CVector	Quaternion::GetRotationAxis() const {

	// Compute sin^2(theta/2).  Remember that w = cos(theta/2),
	// and sin^2(x) + cos^2(x) = 1

	float sinThetaOver2Sq = 1.0f - w*w;

	// Protect against numerical imprecision

	if (sinThetaOver2Sq <= 0.0f) {

		// Identity quaternion, or numerical imprecision.  Just
		// return any valid vector, since it doesn't matter

		return CVector(1.0f, 0.0f, 0.0f);
	}

	// Compute 1 / sin(theta/2)

	float	oneOverSinThetaOver2 = 1.0f / sqrt(sinThetaOver2Sq);

	// Return axis of rotation

	return CVector(
		x * oneOverSinThetaOver2,
		y * oneOverSinThetaOver2,
		z * oneOverSinThetaOver2
	);
}


void Quaternion::FromEuler(const CVector &euler)
{
	float		angle;
	float		sr, sp, sy, cr, cp, cy;

	// FIXME: rescale the inputs to 1/2 angle
	angle = euler.z * 0.5f;
	sy = sin(angle);
	cy = cos(angle);
	angle = euler.y * 0.5f;
	sp = sin(angle);
	cp = cos(angle);
	angle = euler.x * 0.5f;
	sr = sin(angle);
	cr = cos(angle);

	x = sr*cp*cy-cr*sp*sy; // X
	y = cr*sp*cy+sr*cp*sy; // Y
	z = cr*cp*sy-sr*sp*cy; // Z
	w = cr*cp*cy+sr*sp*sy; // W
}
/////////////////////////////////////////////////////////////////////////////
//
// Nonmember functions
//
/////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------
// dotProduct
//
// Quaternion dot product.  We use a nonmember function so we can
// pass quaternion expressions as operands without having "funky syntax"
//
// See 10.4.10

//---------------------------------------------------------------------------
// slerp
//
// Spherical linear interpolation.
//
// See 10.4.13

Quaternion Quaternion::Slerp(const Quaternion &q0, const Quaternion &q1, float t)
{

	// Check for out-of range parameter and return edge points if so

	if (t <= 0.0f) return q0;
	if (t >= 1.0f) return q1;

	// Compute "cosine of angle between quaternions" using dot product

	float cosOmega = q0.DotProduct(q1);

	// If negative dot, use -q1.  Two quaternions q and -q
	// represent the same rotation, but may produce
	// different slerp.  We chose q or -q to rotate using
	// the acute angle.

	float q1w = q1.w;
	float q1x = q1.x;
	float q1y = q1.y;
	float q1z = q1.z;
	if (cosOmega < 0.0f) {
		q1w = -q1w;
		q1x = -q1x;
		q1y = -q1y;
		q1z = -q1z;
		cosOmega = -cosOmega;
	}

	// We should have two unit quaternions, so dot should be <= 1.0

	assert(cosOmega < 1.1f);

	// Compute interpolation fraction, checking for quaternions
	// almost exactly the same

	float k0, k1;
	if (cosOmega > 0.9999f) {

		// Very close - just use linear interpolation,
		// which will protect againt a divide by zero

		k0 = 1.0f-t;
		k1 = t;

	} else {

		// Compute the sin of the angle using the
		// trig identity sin^2(omega) + cos^2(omega) = 1

		float sinOmega = sqrt(1.0f - cosOmega * cosOmega);

		// Compute the angle from its sin and cosine

		float omega = atan2(sinOmega, cosOmega);

		// Compute inverse of denominator, so we only have
		// to divide once

		float oneOverSinOmega = 1.0f / sinOmega;

		// Compute interpolation parameters

		k0 = sin((1.0f - t) * omega) * oneOverSinOmega;
		k1 = sin(t * omega) * oneOverSinOmega;
	}

	// Interpolate

	Quaternion result;
	result.x = k0 * q0.x + k1 * q1x;
	result.y = k0 * q0.y + k1 * q1y;
	result.z = k0 * q0.z + k1 * q1z;
	result.w = k0 * q0.w + k1 * q1w;

	// Return it

	return result;
}

void Quaternion::Slerp(Quaternion &qOut, const Quaternion &q0, const Quaternion &q1, float t)
{

	// Check for out-of range parameter and return edge points if so

	if(t <= 0.0f)
	{
		qOut = q0;
		return;
	}
	if(t >= 1.0f)
	{
		qOut = q1;
		return;
	}

	// Compute "cosine of angle between quaternions" using dot product

	float cosOmega = q0.DotProduct(q1);

	// If negative dot, use -q1.  Two quaternions q and -q
	// represent the same rotation, but may produce
	// different slerp.  We chose q or -q to rotate using
	// the acute angle.

	float q1w = q1.w;
	float q1x = q1.x;
	float q1y = q1.y;
	float q1z = q1.z;
	if (cosOmega < 0.0f)
	{
		q1w = -q1w;
		q1x = -q1x;
		q1y = -q1y;
		q1z = -q1z;
		cosOmega = -cosOmega;
	}

	// We should have two unit quaternions, so dot should be <= 1.0

	//assert(cosOmega < 1.1f);

	// Compute interpolation fraction, checking for quaternions
	// almost exactly the same

	float k0, k1;
	if (cosOmega > 0.9999f)
	{

		// Very close - just use linear interpolation,
		// which will protect againt a divide by zero

		k0 = 1.0f-t;
		k1 = t;

	}else
	{
		// Compute the sin of the angle using the
		// trig identity sin^2(omega) + cos^2(omega) = 1

		float sinOmega = sqrt(1.0f - cosOmega * cosOmega);

		// Compute the angle from its sin and cosine

		float omega = atan2(sinOmega, cosOmega);

		// Compute inverse of denominator, so we only have
		// to divide once

		float oneOverSinOmega = 1.0f / sinOmega;

		// Compute interpolation parameters

		k0 = sin((1.0f - t) * omega) * oneOverSinOmega;
		k1 = sin(t * omega) * oneOverSinOmega;
	}

	// Interpolate
	qOut.x = k0 * q0.x + k1 * q1x;
	qOut.y = k0 * q0.y + k1 * q1y;
	qOut.z = k0 * q0.z + k1 * q1z;
	qOut.w = k0 * q0.w + k1 * q1w;
}


//---------------------------------------------------------------------------
// conjugate
//
// Compute the quaternion conjugate.  This is the quaternian
// with the opposite rotation as the original quaternian.  See 10.4.7

//---------------------------------------------------------------------------
// pow
//
// Quaternion exponentiation.
//
// See 10.4.12

void Quaternion::Pow(float exponent)
{

	// Check for the case of an identity quaternion.
	// This will protect against divide by zero

	if (fabs(w) > .9999f) {
		return;
	}

	// Extract the half angle alpha (alpha = theta/2)

	float	alpha = acos(w);

	// Compute new alpha value

	float	newAlpha = alpha * exponent;

	// Compute new w value

	w = cos(newAlpha);

	// Compute new xyz values

	float	mult = sin(newAlpha) / sin(alpha);
	x *= mult;
	y *= mult;
	z *= mult;
}

void Quaternion::FromMatrix( const Matrix &mat )
{
	float quat[4];
	m3dMatToQuat(quat, mat.m_data);
	x = quat[0];
	y = quat[1];
	z = quat[2];
	w = quat[3];
}
