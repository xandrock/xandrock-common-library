#include "Matrix.h"
#include "Vector.h"
#include "Quaternion.h"

Matrix::Matrix()
{

}

void Matrix::LoadIdentity()
{
	m3dLoadIdentity44(m_data);
}

const Matrix Matrix::operator*(const Matrix &mat) const
{
	Matrix result;
	m3dMatrixMultiply44(result.m_data, this->m_data, mat.m_data);
	return result;
}

const Matrix& Matrix::operator *= (const Matrix &mat)
{
	return *this = *this * mat;
}

void Matrix::Scale(const CVector &scalar)
{
	*this *= ScaleMatrix(scalar);
}

void Matrix::Rotate(const Quaternion &rot)
{
	*this *= RotationMatrix(rot);
}

void Matrix::Rotate(const CVector &eula)
{
	*this *= RotationMatrix(eula);
}

void Matrix::Translate(const CVector &vec)
{
	*this *= TranslationMatrix(vec);
}

const Matrix Matrix::RotationMatrix(const Quaternion &rot)
{
	Matrix result;
	float quat[4] = {rot.x, rot.y, rot.z, rot.w};
	m3dQuaternionMatrix(quat, result.m_data);
	return result;
}

const Matrix Matrix::RotationMatrix(const double degree, const CVector &axis)
{
	Matrix result;
	m3dRotationMatrix44(result.m_data, DEG2RAD(degree), axis.x, axis.y, axis.z);
	return result;
}

const Matrix Matrix::RotationMatrix(const CVector &eula)
{
	Matrix result;
	M3DVector3d rot = {DEG2RAD(eula.x), DEG2RAD(eula.y), DEG2RAD(eula.z)};

	m3dRotationMatrix44(result.m_data, rot);
	return result;
}

const Matrix Matrix::TranslationMatrix(const CVector &vec)
{
	Matrix result;
	m3dTranslationMatrix44(result.m_data, vec.x, vec.y, vec.z);
	return result;
}

const Matrix Matrix::ScaleMatrix(const CVector &scalar)
{
	Matrix result;
	m3dScaleMatrix44(result.m_data, scalar.x, scalar.y, scalar.z);
	return result;
}
