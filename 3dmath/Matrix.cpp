#include "Matrix.h"
#include "Vector.h"
#include "Quaternion.h"

Matrix::Matrix()
{

}

Matrix::Matrix(const Matrix &mat)
{
	*this = mat;
}

const Matrix &Matrix::operator=(const Matrix &mat)
{
	m3dCopyMatrix44(m_data, mat.m_data);
	return *this;
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

const CVector Matrix::operator*(const CVector &vec) const
{
	M3DVector3f in = {vec.x, vec.y, vec.z};
	M3DVector3f out;
	m3dTransformVector3(out, in, m_data);
	return CVector(out[0], out[1], out[2]);
}

const Vector4f Matrix::operator*(const Vector4f &vec) const
{
	M3DVector4f in = {vec.x, vec.y, vec.z, vec.w};
	M3DVector4f out;
	m3dTransformVector4(out, in, m_data);
	Vector4f result;
	result.x = out[0];
	result.y = out[1];
	result.z = out[2];
	result.w = out[3];
	return result;
}

void Matrix::Scale(const CVector &scalar)
{
	*this *= ScaleMatrix(scalar);
}

void Matrix::Rotate(const Quaternion &rot)
{
	*this *= RotationMatrix(rot);
}

void Matrix::Rotate(const double degree, const CVector &axis)
{
	*this *= RotationMatrix(degree, axis);
}

void Matrix::Rotate(const CVector &eula)
{
	*this *= RotationMatrix(eula);
}

void Matrix::Translate(const CVector &vec)
{
	*this *= TranslationMatrix(vec);
}

bool Matrix::Invert()
{
	Matrix mat;
	bool bSuc = m3dInvertMatrix44(mat.m_data, m_data);
	if(bSuc)
	{
		*this = mat;
		return true;
	}else
	{
		return false;
	}
}

void Matrix::Transpost()
{
	*this = GetTranspose();
}

void Matrix::Perspective(const float fovy, const float aspectRatio, const float zNear, const float zFar)
{
	double sine, cotangent, deltaZ;
	double radians = fovy / 2 * PI / 180;

	deltaZ = zFar - zNear;
	sine = sin(radians);
	if ((deltaZ == 0) || (sine == 0) || (aspectRatio == 0)) {
		return;
	}
	cotangent = cos(radians) / sine;

	m_data[0] = cotangent / aspectRatio;
	m_data[5] = cotangent;
	m_data[10] = -(zFar + zNear) / deltaZ;
	m_data[11] = -1;
	m_data[14] = -2 * zNear * zFar / deltaZ;
	m_data[15] = 0;
}

CVector Matrix::TransformVector(const CVector &vec) const
{
	return *this * vec;
}

Vector4f Matrix::TransformVector(const Vector4f &vec) const
{
	return *this * vec;
}

Matrix Matrix::GetInvert() const
{
	Matrix mat;
	m3dInvertMatrix44(mat.m_data, m_data);
	return mat;
}

Matrix Matrix::GetTranspose() const
{
	Matrix mat;
	m3dTransposeMatrix44(mat.m_data, m_data);
	return mat;
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
	M3DVector3f rot = {DEG2RAD(eula.x), DEG2RAD(eula.y), DEG2RAD(eula.z)};

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

CVector Matrix::ProjectPoint( const CVector &point, const Matrix &modelView, const Matrix &projection, const int viewport[4] )
{
	M3DVector3f pointIn = {point.x, point.y, point.z};
	M3DVector3f pointOut;
	m3dProjectXYZ(pointOut, modelView.m_data, projection.m_data, viewport, pointIn);
	CVector vec(pointOut[0], pointOut[1], pointOut[2]);
	return vec;
}

CVector Matrix::UnprojectPoint( const CVector &point, const Matrix &modelView, const Matrix &projection, const int viewport[4] )
{
	// screenpos = objpos * modelView * proj;
	// => ojbpos = screenpos * proj.invert * model.invert;

	//Matrix projInv = projection;
	//projInv.Invert();
	//Matrix mvInv = modelView;
	//mvInv.Invert();
	//Matrix finalMatrix = mvInv * projInv;

	//same as above
	Matrix finalMatrix = projection * modelView;
	finalMatrix.Invert();

	Vector4f in;
	Vector4f out;

	in.x = point.x;
	in.y = point.y;
	in.z = point.z;
	in.w = 1.0;

	/* Map x and y from window coordinates */
	in.x = (in.x - viewport[0]) / viewport[2];
	in.y = (in.y - viewport[1]) / viewport[3];

	/* Map to range -1 to 1 */
	in.x = in.x * 2 - 1;
	in.y = in.y * 2 - 1;
	in.z = in.z * 2 - 1;

	out = finalMatrix * in;
	if (out.w == 0.0)
	{
		return CVector(0, 0, 0);
	}

	out.x /= out.w;
	out.y /= out.w;
	out.z /= out.w;
	return CVector(out.x, out.y, out.z);
}
