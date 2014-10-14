#ifndef MATRIX_H
#define MATRIX_H
#include "math3d.h"
class CVector;
class Quaternion;
struct Vector4f;
class Matrix
{
public:
	Matrix();
	Matrix(const Matrix &mat);
	const Matrix &operator=(const Matrix &mat);

	void LoadIdentity();

	const Matrix operator*(const Matrix &mat) const;
	const Matrix& operator*=(const Matrix &mat);
	const CVector operator*(const CVector &vec) const;
	const Vector4f operator*(const Vector4f &vec) const;



	void Scale(const CVector &scalar);
	void Rotate(const Quaternion &rot);
	void Rotate(const double degree, const CVector &axis);
	void Rotate(const CVector &eula);
	void Translate(const CVector &vec);
	bool Invert();
	void Transpost();
	void Perspective(const float fovy, const float aspectRatio, const float zNear, const float zFar);

	CVector TransformVector(const CVector &vec) const;
	Vector4f TransformVector(const Vector4f &vec) const;

	Matrix GetInvert() const;
	Matrix GetTranspose() const;

	static const Matrix RotationMatrix(const Quaternion &rot);
	static const Matrix RotationMatrix(const double degree, const CVector &axis);
	static const Matrix RotationMatrix(const CVector &eula);
	static const Matrix TranslationMatrix(const CVector &vec);
	static const Matrix ScaleMatrix(const CVector &scalar);
	static CVector ProjectPoint(const CVector &point, const Matrix &modelView, const Matrix &projection, const int viewport[4]);
	static CVector UnprojectPoint(const CVector &point, const Matrix &modelView, const Matrix &projection, const int viewport[4]);
public:
	M3DMatrix44f m_data;
};

#endif // MATRIX_H
