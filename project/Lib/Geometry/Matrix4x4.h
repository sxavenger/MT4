#pragma once

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
#include <algorithm>
#include <cassert>

//* Geomtery
#include "Vector3.h"
#include "Quaternion.h"

////////////////////////////////////////////////////////////////////////////////////////////
// Matrix4x4 class
////////////////////////////////////////////////////////////////////////////////////////////
class Matrix4x4 {
public:

	//=========================================================================================
	// methods
	//=========================================================================================

	static Matrix4x4 Identity();

	// todo: DirectXMathのinverseに変更
	Matrix4x4 Inverse() const;

	Matrix4x4 Transpose() const;

	//=========================================================================================
	// compound assignment operator
	//=========================================================================================

	/* Add */
	Matrix4x4& operator+=(const Matrix4x4& mat) {
		for (int row = 0; row < 4; row++) {
			for (int column = 0; column < 4; column++) {
				m[row][column] += mat.m[row][column];
			}
		}

		return *this;
	}

	/* Subtract */
	Matrix4x4& operator-=(const Matrix4x4& mat) {
		for (int row = 0; row < 4; row++) {
			for (int column = 0; column < 4; column++) {
				m[row][column] -= mat.m[row][column];
			}
}

		return *this;
	}

	/* Multiply */
	Matrix4x4& operator*=(const Matrix4x4& mat) {
		Matrix4x4 result = { 0.0f };

		for (int row = 0; row < 4; row++) {
			for (int column = 0; column < 4; column++) {
				for (int i = 0; i < 4; i++) {
					result.m[row][column] += m[row][i] * mat.m[i][column];
				}
			}
		}

		std::memcpy(this, &result, sizeof(Matrix4x4));
		return *this;
	}

	//=========================================================================================
	// variables
	//=========================================================================================

	float m[4][4];

};

//=========================================================================================
// binary operator
//=========================================================================================

/* Add */
inline Matrix4x4 operator+(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;

	for (int row = 0; row < 4; row++) {
		for (int column = 0; column < 4; column++) {
			result.m[row][column] = m1.m[row][column] + m2.m[row][column];
		}
	}

	return result;
}

/* Subtract */
inline Matrix4x4 operator-(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;

	for (int row = 0; row < 4; row++) {
		for (int column = 0; column < 4; column++) {
			result.m[row][column] = m1.m[row][column] - m2.m[row][column];
		}
	}

	return result;
}

/* Multiply */
inline Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result = { 0.0f };

	for (int row = 0; row < 4; row++) {
		for (int column = 0; column < 4; column++) {
			for (int i = 0; i < 4; i++) {
				result.m[row][column] += m1.m[row][i] * m2.m[i][column];
			}
		}
	}

	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////
// Matrix4x4 methods
////////////////////////////////////////////////////////////////////////////////////////////
namespace Matrix {

	//* make *//

	Matrix4x4 MakeScale(const Vector3f& scale);

	Matrix4x4 MakeRotateX(float radian);
	Matrix4x4 MakeRotateY(float radian);
	Matrix4x4 MakeRotateZ(float radian);
	Matrix4x4 MakeRotate(const Vector3f& rotate);
	Matrix4x4 MakeRotate(const Quaternion& q);

	Matrix4x4 MakeTranslate(const Vector3f& translate);

	Matrix4x4 MakeAffine(const Vector3f& scale, const Vector3f& rotate, const Vector3f& translate);
	Matrix4x4 MakeAffine(const Vector3f& scale, const Quaternion& rotate, const Vector3f& translate);

	Matrix4x4 MakePerspectiveFov(float fovY, float aspectRatio, float nearClip, float farClip);

	Matrix4x4 MakeOrthographic(float left, float top, float right, float bottom, float nearZ, float farZ);

	Matrix4x4 MakeViewport(float left, float top, float width, float height, float minDepth, float maxDepth);

	//* transform *//

	Vector3f Transform(const Vector3f& v, const Matrix4x4& m);
}