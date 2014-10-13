#ifndef MATRIX_H
#define MATRIX_H
#include "math3d.h"
class CVector;
class Quaternion;
class Matrix
{
public:
	Matrix();

	void LoadIdentity();

	const Matrix operator*(const Matrix &mat) const;
	const Matrix& operator*=(const Matrix &mat);

	void Scale(const CVector &scalar);
	void Rotate(const Quaternion &rot);
	void Rotate(const double degree, const CVector &axis);
	void Rotate(const CVector &eula);
	void Translate(const CVector &vec);


	static const Matrix RotationMatrix(const Quaternion &rot);
	static const Matrix RotationMatrix(const double degree, const CVector &axis);
	static const Matrix RotationMatrix(const CVector &eula);
	static const Matrix TranslationMatrix(const CVector &vec);
	static const Matrix ScaleMatrix(const CVector &scalar);

private:
	M3DMatrix44d m_data;
};

#endif // MATRIX_H
