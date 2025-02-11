#include"MathFanctions.h"
#include<cmath>
#include"assert.h"
#include <algorithm>
#pragma region Math

Vector3 Add(const Vector3& v1, const Vector3& v2) {
	Vector3 result{};

	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;


	return result;
};
Vector2 Add(const Vector2& v1, const Vector2& v2) {
	Vector2 result{};

	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;

	return result;
};
Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2)
{
	Matrix4x4 result{};


	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			result.m[y][x] = m1.m[y][x] + m2.m[y][x];
		}
	}


	return result;
}


Vector3 Subtract(const Vector3& v1, const Vector3& v2) {
	Vector3 result{};

	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;


	return result;
}
Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2)
{
	Matrix4x4 result{};


	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			result.m[y][x] = m1.m[y][x] - m2.m[y][x];
		}
	}


	return result;
}



float Length(const Vector3& v) {
	float result;

	result = sqrtf((v.x * v.x) + (v.y * v.y) + (v.z * v.z));

	return result;
};
float LengthSquared(const Vector3& v) {
	return v.x * v.x + v.y * v.y + v.z * v.z;
}
float Length(const float& v) {
	float result;

	result = sqrtf((v * v));

	return result;
};

float Clamp(float t) {

	if (1.0f <= t) {
		t = 1.0f;
	}
	if (0.0f >= t) {
		t = 0.0f;
	}

	return t;
}
float Clamp(float t, float min, float max) {

	if (max <= t) {
		t = max;
	}
	if (min >= t) {
		t = min;
	}

	return t;
}
float Clamp3(float value, float min, float max) {
	return (std::max)(min, (std::min)(value, max));
}

float Distance(const Vector3& point1, const Vector3& point2)
{
	float dx = point1.x - point2.x;
	float dy = point1.y - point2.y;
	float dz = point1.z - point2.z;
	return std::sqrt(dx * dx + dy * dy +dz * dz);
}
// 2つの点間の距離を計算する関数
float DistanceXZ(const Vector3& point1, const Vector3& point2) {
	float dx = point1.x - point2.x;
	float dz = point1.z - point2.z;
	return std::sqrt(dx * dx + dz * dz);
}





Vector3 Bezier(const Vector3& p0, const Vector3& p1, const Vector3& p2, float t) {

	Vector3 p0p1 = Lerp(p0, p1, t);

	Vector3 p1p2 = Lerp(p1, p2, t);

	Vector3 p = Lerp(p0p1, p1p2, t);

	return Lerp(p0p1, p1p2, t);

}
//単位行列
Matrix4x4 MakeIdentity4x4() {
	Matrix4x4 result{};


	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			if (x == y) {
				result.m[y][x] = 1.0f;
			}
			else {
				result.m[y][x] = 0.0f;
			}
		}
	}
	return result;
}
//行列の積
Matrix4x4 Multiply(const Matrix4x4& v1, const Matrix4x4& v2) {
	Matrix4x4 result{};

	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			for (int z = 0; z < 4; z++) {
				float value1 = v1.m[y][z];
				float value2 = v2.m[z][x];

				if (value1 == 0.0f || value2 == 0.0f) {
					result.m[y][x] += 0.0f; // ゼロを掛け算した結果はゼロ
				}
				else {
					result.m[y][x] += value1 * value2;
				}
			}
		}
	}
	return result;
};
// Vector3同士
Vector3 Multiply(const Vector3& v1, const Vector3& v2) {
	Vector3 result{};

	result.x = v1.x * v2.x;
	result.y = v1.y * v2.y;
	result.z = v1.z * v2.z;

	return result;
}
// floatとVector3
Vector3 Multiply(const float& v, const Vector3& v1) {
	Vector3 result{};

	result.x = v1.x * v;
	result.y = v1.y * v;
	result.z = v1.z * v;

	return result;
}
Vector3 Multiply(const Vector3& v1, const float& v2)
{
	Vector3 result{};

	result.x = v1.x * v2;
	result.y = v1.y * v2;
	result.z = v1.z * v2;

	return result;
}
//移動行列
Matrix4x4 MakeTranslateMatrix(const  Vector3& translate) {
	Matrix4x4 result{};

	result.m[0][0] = 1;
	result.m[0][1] = 0;
	result.m[0][2] = 0;
	result.m[0][3] = 0;

	result.m[1][0] = 0;
	result.m[1][1] = 1;
	result.m[1][2] = 0;
	result.m[1][3] = 0;

	result.m[2][0] = 0;
	result.m[2][1] = 0;
	result.m[2][2] = 1;
	result.m[2][3] = 0;

	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;
	result.m[3][3] = 1;

	return result;
}
//拡大縮小行列
Matrix4x4 MakeScaleMatrix(const  Vector3& scale) {
	Matrix4x4 result{};

	result.m[0][0] = scale.x;
	result.m[0][1] = 0;
	result.m[0][2] = 0;
	result.m[0][3] = 0;

	result.m[1][0] = 0;
	result.m[1][1] = scale.y;
	result.m[1][2] = 0;
	result.m[1][3] = 0;

	result.m[2][0] = 0;
	result.m[2][1] = 0;
	result.m[2][2] = scale.z;
	result.m[2][3] = 0;

	result.m[3][0] = 0;
	result.m[3][1] = 0;
	result.m[3][2] = 0;
	result.m[3][3] = 1;

	return result;
}
//回転行列X
Matrix4x4 MakeRotateXMatrix(float rotate) {
	Matrix4x4 result{};

	result.m[0][0] = 1;
	result.m[0][1] = 0;
	result.m[0][2] = 0;
	result.m[0][3] = 0;
	result.m[1][0] = 0;
	result.m[1][1] = std::cos(rotate);
	result.m[1][2] = std::sin(rotate);
	result.m[1][3] = 0;
	result.m[2][0] = 0;
	result.m[2][1] = -std::sin(rotate);
	result.m[2][2] = std::cos(rotate);
	result.m[2][3] = 0;
	result.m[3][0] = 0;
	result.m[3][1] = 0;
	result.m[3][2] = 0;
	result.m[3][3] = 1;


	return result;
}
//回転行列Y
Matrix4x4 MakeRotateYMatrix(float rotate) {
	Matrix4x4 result{};

	result.m[0][0] = std::cos(rotate);
	result.m[0][1] = 0;
	result.m[0][2] = -std::sin(rotate);;
	result.m[0][3] = 0;
	result.m[1][0] = 0;
	result.m[1][1] = 1;
	result.m[1][2] = 0;
	result.m[1][3] = 0;
	result.m[2][0] = std::sin(rotate);
	result.m[2][1] = 0;
	result.m[2][2] = std::cos(rotate);
	result.m[2][3] = 0;
	result.m[3][0] = 0;
	result.m[3][1] = 0;
	result.m[3][2] = 0;
	result.m[3][3] = 1;


	return result;
}
//回転行列Z
Matrix4x4 MakeRotateZMatrix(float rotate) {
	Matrix4x4 result{};

	result.m[0][0] = std::cos(rotate);
	result.m[0][1] = std::sin(rotate);
	result.m[0][2] = 0;
	result.m[0][3] = 0;
	result.m[1][0] = -std::sin(rotate);
	result.m[1][1] = std::cos(rotate);
	result.m[1][2] = 0;
	result.m[1][3] = 0;
	result.m[2][0] = 0;
	result.m[2][1] = 0;
	result.m[2][2] = 1;
	result.m[2][3] = 0;
	result.m[3][0] = 0;
	result.m[3][1] = 0;
	result.m[3][2] = 0;
	result.m[3][3] = 1;


	return result;
}
//逆行列
Matrix4x4 Inverse(const Matrix4x4& m) {
	Matrix4x4 result{};
	float result2;

	result2 =
		1 / ((m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3])
			+ (m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1])
			+ (m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2])

			- (m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1])
			- (m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3])
			- (m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2])

			- (m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3])
			- (m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1])
			- (m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2])

			+ (m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1])
			+ (m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3])
			+ (m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2])

			+ (m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3])
			+ (m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1])
			+ (m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2])

			- (m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1])
			- (m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3])
			- (m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2])

			- (m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0])
			- (m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0])
			- (m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0])


			+ (m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0])
			+ (m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0])
			+ (m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0]));


	//一行目一列目
	result.m[0][0] = (m.m[1][1] * m.m[2][2] * m.m[3][3]) + (m.m[1][2] * m.m[2][3] * m.m[3][1]) + (m.m[1][3] * m.m[2][1] * m.m[3][2])
		- (m.m[1][3] * m.m[2][2] * m.m[3][1]) - (m.m[1][2] * m.m[2][1] * m.m[3][3]) - (m.m[1][1] * m.m[2][3] * m.m[3][2]);
	//一行目二列目
	result.m[0][1] = -(m.m[0][1] * m.m[2][2] * m.m[3][3]) - (m.m[0][2] * m.m[2][3] * m.m[3][1]) - (m.m[0][3] * m.m[2][1] * m.m[3][2])
		+ (m.m[0][3] * m.m[2][2] * m.m[3][1]) + (m.m[0][2] * m.m[2][1] * m.m[3][3]) + (m.m[0][1] * m.m[2][3] * m.m[3][2]);
	//一行目三列目
	result.m[0][2] = (m.m[0][1] * m.m[1][2] * m.m[3][3]) + (m.m[0][2] * m.m[1][3] * m.m[3][1]) + (m.m[0][3] * m.m[1][1] * m.m[3][2])
		- (m.m[0][3] * m.m[1][2] * m.m[3][1]) - (m.m[0][2] * m.m[1][1] * m.m[3][3]) - (m.m[0][1] * m.m[1][3] * m.m[3][2]);
	//一行目四列目
	result.m[0][3] = -(m.m[0][1] * m.m[1][2] * m.m[2][3]) - (m.m[0][2] * m.m[1][3] * m.m[2][1]) - (m.m[0][3] * m.m[1][1] * m.m[2][2])
		+ (m.m[0][3] * m.m[1][2] * m.m[2][1]) + (m.m[0][2] * m.m[1][1] * m.m[2][3]) + (m.m[0][1] * m.m[1][3] * m.m[2][2]);


	//二行目一列目
	result.m[1][0] = -(m.m[1][0] * m.m[2][2] * m.m[3][3]) - (m.m[1][2] * m.m[2][3] * m.m[3][0]) - (m.m[1][3] * m.m[2][0] * m.m[3][2])
		+ (m.m[1][3] * m.m[2][2] * m.m[3][0]) + (m.m[1][2] * m.m[2][0] * m.m[3][3]) + (m.m[1][0] * m.m[2][3] * m.m[3][2]);
	//二行目二列目
	result.m[1][1] = (m.m[0][0] * m.m[2][2] * m.m[3][3]) + (m.m[0][2] * m.m[2][3] * m.m[3][0]) + (m.m[0][3] * m.m[2][0] * m.m[3][2])
		- (m.m[0][3] * m.m[2][2] * m.m[3][0]) - (m.m[0][2] * m.m[2][0] * m.m[3][3]) - (m.m[0][0] * m.m[2][3] * m.m[3][2]);
	//二行目三列目
	result.m[1][2] = -(m.m[0][0] * m.m[1][2] * m.m[3][3]) - (m.m[0][2] * m.m[1][3] * m.m[3][0]) - (m.m[0][3] * m.m[1][0] * m.m[3][2])
		+ (m.m[0][3] * m.m[1][2] * m.m[3][0]) + (m.m[0][2] * m.m[1][0] * m.m[3][3]) + (m.m[0][0] * m.m[1][3] * m.m[3][2]);

	//二行目四列目
	result.m[1][3] = (m.m[0][0] * m.m[1][2] * m.m[2][3]) + (m.m[0][2] * m.m[1][3] * m.m[2][0]) + (m.m[0][3] * m.m[1][0] * m.m[2][2])
		- (m.m[0][3] * m.m[1][2] * m.m[2][0]) - (m.m[0][2] * m.m[1][0] * m.m[2][3]) - (m.m[0][0] * m.m[1][3] * m.m[2][2]);


	//三行目一列目
	result.m[2][0] = (m.m[1][0] * m.m[2][1] * m.m[3][3]) + (m.m[1][1] * m.m[2][3] * m.m[3][0]) + (m.m[1][3] * m.m[2][0] * m.m[3][1])
		- (m.m[1][3] * m.m[2][1] * m.m[3][0]) - (m.m[1][1] * m.m[2][0] * m.m[3][3]) - (m.m[1][0] * m.m[2][3] * m.m[3][1]);
	//三行目二列目
	result.m[2][1] = -(m.m[0][0] * m.m[2][1] * m.m[3][3]) - (m.m[0][1] * m.m[2][3] * m.m[3][0]) - (m.m[0][3] * m.m[2][0] * m.m[3][1])
		+ (m.m[0][3] * m.m[2][1] * m.m[3][0]) + (m.m[0][1] * m.m[2][0] * m.m[3][3]) + (m.m[0][0] * m.m[2][3] * m.m[3][1]);
	//三行目三列目
	result.m[2][2] = (m.m[0][0] * m.m[1][1] * m.m[3][3]) + (m.m[0][1] * m.m[1][3] * m.m[3][0]) + (m.m[0][3] * m.m[1][0] * m.m[3][1])
		- (m.m[0][3] * m.m[1][1] * m.m[3][0]) - (m.m[0][1] * m.m[1][0] * m.m[3][3]) - (m.m[0][0] * m.m[1][3] * m.m[3][1]);
	//三行目四列目
	result.m[2][3] = -(m.m[0][0] * m.m[1][1] * m.m[2][3]) - (m.m[0][1] * m.m[1][3] * m.m[2][0]) - (m.m[0][3] * m.m[1][0] * m.m[2][1])
		+ (m.m[0][3] * m.m[1][1] * m.m[2][0]) + (m.m[0][1] * m.m[1][0] * m.m[2][3]) + (m.m[0][0] * m.m[1][3] * m.m[2][1]);


	//四行目一列目
	result.m[3][0] = -(m.m[1][0] * m.m[2][1] * m.m[3][2]) - (m.m[1][1] * m.m[2][2] * m.m[3][0]) - (m.m[1][2] * m.m[2][0] * m.m[3][1])
		+ (m.m[1][2] * m.m[2][1] * m.m[3][0]) + (m.m[1][1] * m.m[2][0] * m.m[3][2]) + (m.m[1][0] * m.m[2][2] * m.m[3][1]);
	//四行目二列目
	result.m[3][1] = (m.m[0][0] * m.m[2][1] * m.m[3][2]) + (m.m[0][1] * m.m[2][2] * m.m[3][0]) + (m.m[0][2] * m.m[2][0] * m.m[3][1])
		- (m.m[0][2] * m.m[2][1] * m.m[3][0]) - (m.m[0][1] * m.m[2][0] * m.m[3][2]) - (m.m[0][0] * m.m[2][2] * m.m[3][1]);
	//四行目三列目
	result.m[3][2] = -(m.m[0][0] * m.m[1][1] * m.m[3][2]) - (m.m[0][1] * m.m[1][2] * m.m[3][0]) - (m.m[0][2] * m.m[1][0] * m.m[3][1])
		+ (m.m[0][2] * m.m[1][1] * m.m[3][0]) + (m.m[0][1] * m.m[1][0] * m.m[3][2]) + (m.m[0][0] * m.m[1][2] * m.m[3][1]);
	//四行目四列目
	result.m[3][3] = (m.m[0][0] * m.m[1][1] * m.m[2][2]) + (m.m[0][1] * m.m[1][2] * m.m[2][0]) + (m.m[0][2] * m.m[1][0] * m.m[2][1])
		- (m.m[0][2] * m.m[1][1] * m.m[2][0]) - (m.m[0][1] * m.m[1][0] * m.m[2][2]) - (m.m[0][0] * m.m[1][2] * m.m[2][1]);

	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {

			result.m[y][x] = result.m[y][x] * result2;

		}
	}


	return result;
}
//転置行列
Matrix4x4 Transpose(const Matrix4x4& m) {
	Matrix4x4 result{};


	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			result.m[y][x] = m.m[x][y];
		}
	}
	return result;
}
//アフィン変換
Matrix4x4 MakeAffineMatrix(const  Vector3& scale, const  Vector3& rotate, const  Vector3& translate) {
	Matrix4x4 result{};

	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);
	Matrix4x4 rotateXYZMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));


	result.m[0][0] = scale.x * rotateXYZMatrix.m[0][0];
	result.m[0][1] = scale.x * rotateXYZMatrix.m[0][1];
	result.m[0][2] = scale.x * rotateXYZMatrix.m[0][2];
	result.m[0][3] = 0;
	result.m[1][0] = scale.y * rotateXYZMatrix.m[1][0];
	result.m[1][1] = scale.y * rotateXYZMatrix.m[1][1];
	result.m[1][2] = scale.y * rotateXYZMatrix.m[1][2];
	result.m[1][3] = 0;
	result.m[2][0] = scale.z * rotateXYZMatrix.m[2][0];
	result.m[2][1] = scale.z * rotateXYZMatrix.m[2][1];
	result.m[2][2] = scale.z * rotateXYZMatrix.m[2][2];
	result.m[2][3] = 0;
	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;
	result.m[3][3] = 1;

	//SafeMatrix(result);

	return result;
};

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Quaternion& rotate, const Vector3& translate) {
	// 結果行列の初期化
	Matrix4x4 result{};
	result.Identity();
	// 回転行列を生成（回転が正規化済みであることを想定）
	Matrix4x4 rotationMatrix = rotate.MakeRotateMatrix();

	// スケールを適用
	result.m[0][0] = scale.x * rotationMatrix.m[0][0];
	result.m[0][1] = scale.x * rotationMatrix.m[0][1];
	result.m[0][2] = scale.x * rotationMatrix.m[0][2];
	result.m[0][3] = 0.0f;

	result.m[1][0] = scale.y * rotationMatrix.m[1][0];
	result.m[1][1] = scale.y * rotationMatrix.m[1][1];
	result.m[1][2] = scale.y * rotationMatrix.m[1][2];
	result.m[1][3] = 0.0f;

	result.m[2][0] = scale.z * rotationMatrix.m[2][0];
	result.m[2][1] = scale.z * rotationMatrix.m[2][1];
	result.m[2][2] = scale.z * rotationMatrix.m[2][2];
	result.m[2][3] = 0.0f;

	// 平行移動を適用
	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;
	result.m[3][3] = 1.0f;

	return result;
}




//正射影行列
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip) {
	Matrix4x4 result{};

	result.m[0][0] = 2 / (right - left);
	result.m[0][1] = 0;
	result.m[0][2] = 0;
	result.m[0][3] = 0;

	result.m[1][0] = 0;
	result.m[1][1] = 2 / (top - bottom);
	result.m[1][2] = 0;
	result.m[1][3] = 0;

	result.m[2][0] = 0;
	result.m[2][1] = 0;
	result.m[2][2] = 1 / (farClip - nearClip);
	result.m[2][3] = 0;

	result.m[3][0] = (left + right) / (left - right);
	result.m[3][1] = (top + bottom) / (bottom - top);
	result.m[3][2] = nearClip / (nearClip - farClip);
	result.m[3][3] = 1;





	return result;
}
//透視射影行列
Matrix4x4 MakePerspectiveFovMatrix(float forY, float aspectRatio, float nearClip, float farClip) {
	Matrix4x4 result{};

	result.m[0][0] = (1 / aspectRatio) * 1 / std::tan(forY / 2);
	result.m[0][1] = 0;
	result.m[0][2] = 0;
	result.m[0][3] = 0;

	result.m[1][0] = 0;
	result.m[1][1] = 1 / std::tan(forY / 2);
	result.m[1][2] = 0;
	result.m[1][3] = 0;

	result.m[2][0] = 0;
	result.m[2][1] = 0;
	result.m[2][2] = farClip / (farClip - nearClip);
	result.m[2][3] = 1;

	result.m[3][0] = 0;
	result.m[3][1] = 0;
	result.m[3][2] = (-nearClip * farClip) / (farClip - nearClip);
	result.m[3][3] = 0;

	return result;
}
//ビューポート変換行列
Matrix4x4 MakeViewportMatrix(float leht, float top, float width, float height, float minDepth, float maxDepth) {
	Matrix4x4 result{};

	result.m[0][0] = width / 2;
	result.m[0][1] = 0;
	result.m[0][2] = 0;
	result.m[0][3] = 0;

	result.m[1][0] = 0;
	result.m[1][1] = (-height) / 2;
	result.m[1][2] = 0;
	result.m[1][3] = 0;

	result.m[2][0] = 0;
	result.m[2][1] = 0;
	result.m[2][2] = maxDepth - minDepth;
	result.m[2][3] = 0;

	result.m[3][0] = leht + (width / 2);
	result.m[3][1] = top + (height / 2);
	result.m[3][2] = minDepth;
	result.m[3][3] = 1;

	return result;
};
//座標変換
Vector3 Transforms(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 result{};

	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];

	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;
	return result;
}
Vector4 Transforms(const Vector4& vec, const Matrix4x4& mat)
{
	return Vector4{
		vec.x * mat.m[0][0] + vec.y * mat.m[1][0] + vec.z * mat.m[2][0] + vec.w * mat.m[3][0],
		vec.x * mat.m[0][1] + vec.y * mat.m[1][1] + vec.z * mat.m[2][1] + vec.w * mat.m[3][1],
		vec.x * mat.m[0][2] + vec.y * mat.m[1][2] + vec.z * mat.m[2][2] + vec.w * mat.m[3][2],
		vec.x * mat.m[0][3] + vec.y * mat.m[1][3] + vec.z * mat.m[2][3] + vec.w * mat.m[3][3]
	};
}
bool IsCollision(const AABB& aabb, const Vector3& point) {
	return (point.x >= aabb.min.x && point.x <= aabb.max.x) &&
		(point.y >= aabb.min.y && point.y <= aabb.max.y) &&
		(point.z >= aabb.min.z && point.z <= aabb.max.z);
}

Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m) {
	Vector3 result{

		v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0],
		v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1],
		v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2],
	};

	return result;
}



Vector3 Project(const Vector3& v1, const Vector3& v2)
{
	Vector3 result{};
	float v2length = Dot(v2, v2);

	float dot = Dot(v1, v2);


	result.x = dot / v2length * v2.x;
	result.y = dot / v2length * v2.y;
	result.z = dot / v2length * v2.z;


	return result;
}


Vector3 ClosestPoint(const Vector3& point, const Segment& segment)
{
	Vector3 result{};
	Vector3 d = segment.diff();
	Vector3 v = {
		point.x - segment.origin.x,
		point.y - segment.origin.y,
		point.z - segment.origin.z 
	};


	float dot = Dot(d, d);
	if (dot == 0.0f) {
		return segment.origin;
	}

	float t = Dot(v, d) / dot;

	t = Clamp(t);

	result.x = segment.origin.x + d.x * t;
	result.y = segment.origin.y + d.y * t;
	result.z = segment.origin.z + d.z * t;

	return result;
}
//
Vector3 ClosestPointAABBSphere(const Sphere& sphere, const AABB& aabb)
{
	Vector3 closestPoint;
	closestPoint.x = Clamp(sphere.center.x, aabb.min.x, aabb.max.x);
	closestPoint.y = Clamp(sphere.center.y, aabb.min.y, aabb.max.y);
	closestPoint.z = Clamp(sphere.center.z, aabb.min.z, aabb.max.z);

	return closestPoint;
}
//衝突判定(球と球)
bool IsCollision(const Sphere& s1, const Sphere& s2)
{
	Vector3 result{};
	float isDistance = false;

	result.x = s2.center.x - s1.center.x;
	result.y = s2.center.y - s1.center.y;
	result.z = s2.center.z - s1.center.z;

	float distance = Length(result);

	if (distance <= s2.radius + s1.radius) {

		isDistance = true;

	}
	else {

		isDistance = false;

	}

	return isDistance;
}
//衝突判定(球と平面)
bool IsCollision(const Sphere& sphere, const Plane& plane)
{
	Vector3 dis = Normalize(plane.normal);

	float distance = Dot(sphere.center, dis) - plane.distance;
	return std::abs(distance) < sphere.radius;
}
//衝突判定(線と平面)
bool IsCollision(const Segment& segment, const Plane& plane)
{
	Vector3 diff = segment.diff();


	//まず垂直判定を行うために、法線と線の内積を求める
	float dot = Dot(plane.normal, diff);

	// 垂直=平行であるので、衝突しているはずがない
	if (dot == 0.0f) {
		return false;
	}

	float t = (plane.distance - Dot(segment.origin, plane.normal)) / dot;

	if (t >= 0.0f && t <= 1.0f) {
		return true;
	}
	return false;
}

//三角形と線の衝突判定
bool IsCollision(const Triangle& triangle, const Segment& segment) {
	Vector3 diff = segment.diff();

	// 三角形の平面を取得
	Plane plane = PlaneFromPoints(triangle.vertices[0], triangle.vertices[1], triangle.vertices[2]);

	float dot = Dot(plane.normal, diff);

	// 平行チェック（浮動小数点誤差を考慮）
	if (fabs(dot) < 1e-6f) {
		return false;
	}

	float t = -(Dot(segment.origin, plane.normal) + plane.distance) / dot;

	// tが線分の範囲 [0, 1] にない場合、衝突なし
	if (t < 0.0f || t > 1.0f) {
		return false;
	}

	// 衝突点を求める
	Vector3 p = segment.origin + diff * t;

	Vector3 v01 = triangle.vertices[1] - triangle.vertices[0];
	Vector3 v12 = triangle.vertices[2] - triangle.vertices[1];
	Vector3 v20 = triangle.vertices[0] - triangle.vertices[2];

	Vector3 v0p = p - triangle.vertices[0];
	Vector3 v1p = p - triangle.vertices[1];
	Vector3 v2p = p - triangle.vertices[2];

	Vector3 cross01 = Cross(v01, v0p);
	Vector3 cross12 = Cross(v12, v1p);
	Vector3 cross20 = Cross(v20, v2p);

	float dot01 = Dot(cross01, plane.normal);
	float dot12 = Dot(cross12, plane.normal);
	float dot20 = Dot(cross20, plane.normal);

	// 浮動小数点誤差に対する閾値を適用して判定
	const float epsilon = 1e-6f;
	if (dot01 >= -epsilon && dot12 >= -epsilon && dot20 >= -epsilon) {
		return true;
	}

	return false;
}

//bool IsCollision(const Triangle& triangle, const Capsule& capsule) {
//	// カプセルの軸線を取得
//	Segment capsuleSegment = capsule.segment;
//	float radius = capsule.radius;
//
//	// 三角形の最近接点を求める
//	Vector3 closestPoint = ClosestPointSegmentTriangle(capsuleSegment, triangle);
//
//	// カプセルの軸線上の最近接点を求める
//	Vector3 capsuleClosestPoint = ClosestPointSegment(capsuleSegment, closestPoint);
//
//	// カプセル表面との最短距離を計算
//	float distanceSquared = LengthSquared(Subtract(closestPoint, capsuleClosestPoint));
//
//	// 衝突判定：距離がカプセル半径以内であれば衝突
//	return distanceSquared <= (radius * radius);
//}

// 点と線分間の距離の二乗を求める関数
float PointLineDistanceSquared(const Vector3& point, const Vector3& a, const Vector3& b)
{
	Vector3 ab = b - a;
	Vector3 ap = point - a;
	float abLengthSquared = ab.LengthSq();

	if (abLengthSquared == 0.0f) {
		return ap.LengthSq(); // 線分が点の場合
	}

	float t = ap.Dot(ab) / abLengthSquared;
	t = std::max(0.0f, std::min(1.0f, t));

	Vector3 closestPoint = a + ab * t;
	return (point - closestPoint).LengthSq();
}

// 線分と線分間の距離の二乗を求める関数
float SegmentSegmentDistanceSquared(const Segment& seg1, const Segment& seg2)
{
	Vector3 u = seg1.diff();
	Vector3 v = seg2.diff();
	Vector3 w = seg1.origin - seg2.origin;

	float a = u.Dot(u);  // |u|^2
	float b = u.Dot(v);
	float c = v.Dot(v);  // |v|^2
	float d = u.Dot(w);
	float e = v.Dot(w);

	float denom = a * c - b * b;
	float s, t;

	if (denom != 0.0f) {
		s = (b * e - c * d) / denom;
		t = (a * e - b * d) / denom;
		s = std::max(0.0f, std::min(1.0f, s));
		t = std::max(0.0f, std::min(1.0f, t));
	}
	else {
		s = 0.0f;
		t = d / b;
		t = std::max(0.0f, std::min(1.0f, t));
	}

	Vector3 closestPoint1 = seg1.origin + u * s;
	Vector3 closestPoint2 = seg2.origin + v * t;
	return (closestPoint1 - closestPoint2).LengthSq();
}

// Triangle と Capsule の衝突判定関数
bool IsCollision(const Triangle& triangle, const Capsule& capsule)
{
	// カプセルの半径の二乗
	float radiusSquared = capsule.radius * capsule.radius;

	// 三角形の各頂点とカプセルの線分との距離を判定
	for (const Vector3& vertex : triangle.vertices)
	{
		if (PointLineDistanceSquared(vertex, capsule.segment.origin, capsule.segment.end) <= radiusSquared)
		{
			return true;
		}
	}

	// 三角形のエッジとカプセルの線分の距離を判定
	Segment edge1 = { triangle.vertices[0], triangle.vertices[1] };
	Segment edge2 = { triangle.vertices[1], triangle.vertices[2] };
	Segment edge3 = { triangle.vertices[2], triangle.vertices[0] };

	if (SegmentSegmentDistanceSquared(capsule.segment, edge1) <= radiusSquared ||
		SegmentSegmentDistanceSquared(capsule.segment, edge2) <= radiusSquared ||
		SegmentSegmentDistanceSquared(capsule.segment, edge3) <= radiusSquared)
	{
		return true;
	}

	return false;
}


//
bool IsCollision(const AABB& aabb1, const AABB& aabb2)
{
	if ((aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) &&
		(aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y) &&
		(aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z)) {

		return true;
	}

	return false;
}
//
bool IsCollision(const AABB& aabb, const Sphere& sphere)
{
	Vector3 closestPoint = ClosestPointAABBSphere(sphere, aabb);

	float dist = Distance(sphere.center, closestPoint);

	if (dist <= sphere.radius) {
		return true;
	}
	else {
		return false;
	}
}
//四角形と線の衝突判定
bool IsCollision(const AABB& aabb, const Segment& segment)
{
	//点がAABB内にあるなら
	if (IsPointInsideAABB(segment.origin, aabb) || IsPointInsideAABB(segment.end, aabb)) {

		return true;
	}

	Plane planeX1, planeY1, planeZ1;
	planeX1.normal = { 1,0,0 };
	planeY1.normal = { 0,1,0 };
	planeZ1.normal = { 0,0,1 };

	Vector3 diff = segment.diff();

	float dotX = Dot(planeX1.normal, diff);
	float dotY = Dot(planeY1.normal, diff);
	float dotZ = Dot(planeZ1.normal, diff);


	// 特異点チェック: 線分が軸に平行である場合
	if (std::abs(dotX) < 1e-6) {
		if (segment.origin.x < aabb.min.x || segment.origin.x > aabb.max.x) return false;
	}
	if (std::abs(dotY) < 1e-6) {
		if (segment.origin.y < aabb.min.y || segment.origin.y > aabb.max.y) return false;
	}
	if (std::abs(dotZ) < 1e-6) {
		if (segment.origin.z < aabb.min.z || segment.origin.z > aabb.max.z) return false;
	}


	Vector3 tMin, tMax;

	tMin.x = (aabb.min.x - Dot(segment.origin, planeX1.normal)) / dotX;
	tMin.y = (aabb.min.y - Dot(segment.origin, planeY1.normal)) / dotY;
	tMin.z = (aabb.min.z - Dot(segment.origin, planeZ1.normal)) / dotZ;

	tMax.x = (aabb.max.x - Dot(segment.origin, planeX1.normal)) / dotX;
	tMax.y = (aabb.max.y - Dot(segment.origin, planeY1.normal)) / dotY;
	tMax.z = (aabb.max.z - Dot(segment.origin, planeZ1.normal)) / dotZ;


	Vector3 tNear, tFar;

	tNear.x = std::min(tMin.x, tMax.x);
	tNear.y = std::min(tMin.y, tMax.y);
	tNear.z = std::min(tMin.z, tMax.z);
	tFar.x = std::max(tMin.x, tMax.x);
	tFar.y = std::max(tMin.y, tMax.y);
	tFar.z = std::max(tMin.z, tMax.z);


	// AABBとの衝突点(貫通点)のtが小さい方
	float tmin = std::max(std::max(tNear.x, tNear.y), tNear.z);

	// AABBとの衝突点(貫通点)のtが大きい方
	float tmax = std::min(std::min(tFar.x, tFar.y), tFar.z);

	if (tmin <= tmax) {
		return true;
	}


	return false;
}
//
bool IsCollision(const Sphere& sphere, const Segment& segment) {
	Vector3 m = Subtract(segment.origin, sphere.center);
	//Vector3 segEnd = Add(segment.origin, segment.diff);

	// 線分の方向ベクトル
	Vector3 d = segment.diff();
	float a = Dot(d, d);

	// 線分の長さがゼロの場合（無効な線分）
	if (a == 0.0f) return false;

	float b = Dot(m, d);
	float c = Dot(m, m) - sphere.radius * sphere.radius;

	// 判別式の計算
	float discriminant = b * b - a * c;

	// 判別式が負の場合、衝突していない
	if (discriminant < 0.0f) return false;

	// 解の計算
	float sqrtDiscriminant = std::sqrt(discriminant);
	float t1 = (-b - sqrtDiscriminant) / a;
	float t2 = (-b + sqrtDiscriminant) / a;

	// 衝突が線分内（0 <= t <= 1）の場合にのみ true を返す
	if ((t1 >= 0.0f && t1 <= 1.0f) || (t2 >= 0.0f && t2 <= 1.0f)) {
		return true;
	}

	return false;
}
bool IsCollision(const OBB& obb, const Sphere& sphere)
{
	// OBBのローカル座標系に球の中心を変換
	Vector3 d = sphere.center - obb.center;
	Vector3 closestPoint = obb.center;

	for (int i = 0; i < 3; ++i) {
		float distance = Dot(d, obb.orientations[i]);
		float clampedDistance;
		if (i == 0) {
			clampedDistance = std::max(-obb.size.x, std::min(distance, obb.size.x));
		}
		else if (i == 1) {
			clampedDistance = std::max(-obb.size.y, std::min(distance, obb.size.y));
		}
		else {
			clampedDistance = std::max(-obb.size.z, std::min(distance, obb.size.z));
		}
		closestPoint += obb.orientations[i] * clampedDistance;
	}


	// 球の中心と最近接点の距離を比較
	float distSquared = LengthSquared(sphere.center - closestPoint);
	return distSquared <= (sphere.radius * sphere.radius);
}

// OBB Segment
// OBBとSegmentの衝突判定
// OBBとSegmentの衝突判定
bool IsCollision(const OBB& obb, const Segment& segment) {
	// OBBの各軸に対して投影を行い、交差を確認
	for (int i = 0; i < 3; ++i) {
		// OBBの軸ベクトル
		Vector3 axis = obb.orientations[i].Normalize();

		// セグメントの始点・終点の投影
		float projStart = axis.Dot(segment.origin);
		float projEnd = axis.Dot(segment.end);

		// 線分の投影範囲
		float minSegment = std::min(projStart, projEnd);
		float maxSegment = std::max(projStart, projEnd);

		// OBBのサイズに基づく投影範囲
		float obbCenterProjection = axis.Dot(obb.center);
		float obbHalfSize = std::abs(obb.size.x * axis.x) +
			std::abs(obb.size.y * axis.y) +
			std::abs(obb.size.z * axis.z);
		float obbMin = obbCenterProjection - obbHalfSize;
		float obbMax = obbCenterProjection + obbHalfSize;

		// 交差判定 (分離軸定理)
		if (maxSegment < obbMin || minSegment > obbMax) {
			return false; // 交差しない
		}
	}
	return true; // 交差する
}

//
Vector3 Perpendicular(const Vector3& vector) {
	if (vector.x != 0.0f || vector.y != 0.0f) {
		return { -vector.y,vector.x,0.0f };
	}
	return { 0.0f,-vector.z,vector.y };
}
//
Plane PlaneFromPoints(const Vector3& p1, const Vector3& p2, const Vector3& p3) {
	Plane result{};

	// 2つのベクトルを求める
	Vector3 v1 = Subtract(p2, p1);
	Vector3 v2 = Subtract(p3, p1);

	// 法線を計算 (外積)
	result.normal = Cross(v1, v2);

	// ゼロベクトルチェック (3点が同一直線上の場合)
	if (Length(result.normal) == 0.0f) {
		// 法線が求まらない場合のエラーハンドリング
		result.normal = { 0.0f, 0.0f, 0.0f };
		result.distance = 0.0f;
		return result;
	}

	// 正規化
	result.normal = Normalize(result.normal);

	// 平面の距離 D の計算 (符号の修正)
	result.distance = -Dot(result.normal, p1);

	return result;
}

Vector3 Reflect(const Vector3& input, const Vector3& normal)
{
	Vector3 result;
	float dotProduct = Dot(input, normal);
	result.x = input.x - normal.x * (2 * dotProduct);
	result.y = input.y - normal.y * (2 * dotProduct);
	result.z = input.z - normal.z * (2 * dotProduct);
	return result;
}


Vector3 Reflect(const Vector3& input, const Vector3& normal, float restitution)
{
	// まず normal が正規化されている前提だが、明示的に normalize する場合:
	Vector3 norm = Normalize(normal);

	// 計算用変数
	float scale = 2.0f * Dot(input, norm) * restitution;

	// 反射ベクトルの計算
	return input - norm * scale;
}


// 分離軸定理による OBB vs OBB の衝突判定
bool IsCollision(const OBB& obb0, const OBB& obb1) {
	const Vector3* A = obb0.orientations; // OBB0の軸ベクトル
	const Vector3* B = obb1.orientations; // OBB1の軸ベクトル

	Vector3 T = obb1.center - obb0.center; // OBB0からOBB1の中心位置ベクトル
	T = Vector3(T.Dot(A[0]), T.Dot(A[1]), T.Dot(A[2])); // OBB0の座標系に変換

	constexpr float EPSILON = std::numeric_limits<float>::epsilon();

	float ra, rb;
	float R[3][3], AbsR[3][3];

	// 回転行列の計算と絶対値の取得
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			R[i][j] = A[i].Dot(B[j]);
			AbsR[i][j] = std::abs(R[i][j]) + EPSILON;  // 数値誤差対策
		}
	}

	// 1. Aの各軸を分離軸としてチェック
	for (int i = 0; i < 3; ++i) {
		ra = obb0.size.x * std::abs(A[i].x) + obb0.size.y * std::abs(A[i].y) + obb0.size.z * std::abs(A[i].z);
		rb = obb1.size.x * AbsR[i][0] + obb1.size.y * AbsR[i][1] + obb1.size.z * AbsR[i][2];

		if (std::abs(T.Dot(A[i])) > ra + rb) return false;
	}

	// 2. Bの各軸を分離軸としてチェック
	for (int i = 0; i < 3; ++i) {
		ra = obb0.size.x * AbsR[0][i] + obb0.size.y * AbsR[1][i] + obb0.size.z * AbsR[2][i];
		rb = obb1.size.x * std::abs(B[i].x) + obb1.size.y * std::abs(B[i].y) + obb1.size.z * std::abs(B[i].z);

		if (std::abs(T.Dot(B[i])) > ra + rb) return false;
	}

	// 3. Aの各軸とBの各軸のクロス積（9軸）を分離軸としてチェック
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			ra = obb0.size.y * AbsR[(i + 1) % 3][j] + obb0.size.z * AbsR[(i + 2) % 3][j];
			rb = obb1.size.y * AbsR[i][(j + 1) % 3] + obb1.size.z * AbsR[i][(j + 2) % 3];

			float t;
			if (i == 0) {
				t = std::abs(T.y * R[2][j] - T.z * R[1][j]);
			}
			else if (i == 1) {
				t = std::abs(T.z * R[0][j] - T.x * R[2][j]);
			}
			else { // i == 2
				t = std::abs(T.x * R[1][j] - T.y * R[0][j]);
			}

			if (t > ra + rb) return false;
		}
	}

	// すべての軸で分離がない場合、衝突あり
	return true;
}
// カプセルたち
bool IsCollision(const Capsule& cap0, const Capsule& cap1)
{
	float distanceSq = SegmentClosestDistanceSq(cap0.segment, cap1.segment);
	float radiusSum = cap0.radius + cap1.radius;
	return distanceSq <= radiusSum * radiusSum;
}



Vector3 ClosestPointSegmentTriangle(const Segment& segment, const Triangle& triangle) {
	// 三角形の3辺を定義
	Segment edge1 = { triangle.vertices[0], triangle.vertices[1] };
	Segment edge2 = { triangle.vertices[1], triangle.vertices[2] };
	Segment edge3 = { triangle.vertices[2], triangle.vertices[0] };

	// 最初に線分の始点を初期最近接点とする
	Vector3 closest = ClosestPointSegment(segment, triangle.vertices[0]);

	// 三角形のエッジに対して最近接点を更新
	closest = ClosestPointSegmentSegment(segment, edge1, closest);
	closest = ClosestPointSegmentSegment(segment, edge2, closest);
	closest = ClosestPointSegmentSegment(segment, edge3, closest);

	// 三角形の面上の最近接点も考慮（重要）
	Plane plane = PlaneFromPoints(triangle.vertices[0], triangle.vertices[1], triangle.vertices[2]);
	Vector3 pointOnPlane = ClosestPointOnPlane(plane, segment.origin);
	closest = LengthSquared(Subtract(closest, segment.origin)) < LengthSquared(Subtract(pointOnPlane, segment.origin))
		? closest
		: pointOnPlane;

	return closest;
}

Vector3 ClosestPointSegmentSegment(const Segment& seg1, const Segment& seg2, Vector3 currentClosest) {
	Vector3 u = seg1.diff();
	Vector3 v = seg2.diff();
	Vector3 w = Subtract(seg1.origin, seg2.origin);

	float a = Dot(u, u);
	float b = Dot(u, v);
	float c = Dot(v, v);
	float d = Dot(u, w);
	float e = Dot(v, w);

	float denom = a * c - b * b;

	// ゼロ除算の回避
	float s = 0.0f, t = 0.0f;
	if (denom != 0.0f) {
		s = Clamp3((b * e - c * d) / denom, 0.0f, 1.0f);
		t = Clamp3((a * e - b * d) / denom, 0.0f, 1.0f);
	}

	Vector3 closestOnSeg1 = Add(seg1.origin, Multiply(u, s));
	Vector3 closestOnSeg2 = Add(seg2.origin, Multiply(v, t));

	return LengthSquared(Subtract(closestOnSeg1, closestOnSeg2)) < LengthSquared(Subtract(currentClosest, seg1.origin))
		? closestOnSeg1
		: currentClosest;
}

Vector3 ClosestPointSegment(const Segment& segment, const Vector3& point) {
	Vector3 diff = segment.diff();
	float lenSq = Dot(diff, diff);

	if (lenSq == 0.0f) {
		return segment.origin;  // 線分が点の場合は始点を返す
	}

	float t = Dot(Subtract(point, segment.origin), diff) / lenSq;
	t = Clamp3(t, 0.0f, 1.0f);

	return Add(segment.origin, Multiply(diff, t));
}

Vector3 ClosestPointOnPlane(const Plane& plane, const Vector3& point) {
	// 平面の法線ベクトル
	Vector3 normal = plane.normal.Normalize();

	// 点から平面までの距離 d = dot(N, P) - D
	float distance = Dot(normal, point) - plane.distance;

	// 最近接点を計算: P - d * N
	Vector3 closestPoint = Subtract(point, Multiply(normal, distance));

	return closestPoint;
}



float SegmentClosestDistanceSq(const Segment& seg0, const Segment& seg1) {
	Vector3 u = seg0.diff();
	Vector3 v = seg1.diff();
	Vector3 w = seg0.origin - seg1.origin;

	float a = u.Dot(u); // seg0の長さの2乗
	float b = u.Dot(v);
	float c = v.Dot(v); // seg1の長さの2乗
	float d = u.Dot(w);
	float e = v.Dot(w);

	float denom = a * c - b * b;
	float sN, sD = denom;
	float tN, tD = denom;

	// デノミネーターがゼロの場合（平行な場合）、近似値をとる
	if (denom < 1e-6f) {
		sN = 0.0f;
		sD = 1.0f;
		tN = e;
		tD = c;
	}
	else {
		sN = (b * e - c * d);
		tN = (a * e - b * d);

		if (sN < 0.0f) {
			sN = 0.0f;
			tN = e;
			tD = c;
		}
		else if (sN > sD) {
			sN = sD;
			tN = e + b;
			tD = c;
		}
	}

	if (tN < 0.0f) {
		tN = 0.0f;
		if (-d < 0.0f) {
			sN = 0.0f;
		}
		else if (-d > a) {
			sN = sD;
		}
		else {
			sN = -d;
			sD = a;
		}
	}
	else if (tN > tD) {
		tN = tD;
		if ((-d + b) < 0.0f) {
			sN = 0.0f;
		}
		else if ((-d + b) > a) {
			sN = sD;
		}
		else {
			sN = (-d + b);
			sD = a;
		}
	}

	float sc = (std::abs(sN) < 1e-6f ? 0.0f : sN / sD);
	float tc = (std::abs(tN) < 1e-6f ? 0.0f : tN / tD);

	Vector3 dP = w + (u * sc) - (v * tc);
	return dP.LengthSq();
}

bool IsPointInsideAABB(const Vector3& point, const AABB& aabb) {
	return (point.x >= aabb.min.x && point.x <= aabb.max.x) &&
		(point.y >= aabb.min.y && point.y <= aabb.max.y) &&
		(point.z >= aabb.min.z && point.z <= aabb.max.z);
}



Vector3 CatmullRom(const Vector3& p0, const Vector3 p1, const Vector3 p2, const Vector3 p3, float t) {

	const float s = 0.5f;

	float t2 = t * t;  // tの2乗
	float t3 = t2 * t; // tの3乗

	Vector3 pt{};
	Vector3 e3{};
	Vector3 e2{};
	Vector3 e1{};
	Vector3 e0{};
	//
	e3.x = (((-p0.x) + (3 * p1.x) + (-3 * p2.x) + (p3.x)));
	e3.y = (((-p0.y) + (3 * p1.y) + (-3 * p2.y) + (p3.y)));
	e3.z = (((-p0.z) + (3 * p1.z) + (-3 * p2.z) + (p3.z)));
	//
	e2.x = (((2 * p0.x) + (-5 * p1.x) + (4 * p2.x) + (-p3.x)));
	e2.y = (((2 * p0.y) + (-5 * p1.y) + (4 * p2.y) + (-p3.y)));
	e2.z = (((2 * p0.z) + (-5 * p1.z) + (4 * p2.z) + (-p3.z)));
	//
	e1 = Subtract(p2, p0);
	//
	e0 = Multiply(p1, 2);

	pt.x = (e3.x * t3 + e2.x * t2 + e1.x * t + e0.x) * s;
	pt.y = (e3.y * t3 + e2.y * t2 + e1.y * t + e0.y) * s;
	pt.z = (e3.z * t3 + e2.z * t2 + e1.z * t + e0.z) * s;

	return pt;
}

Vector3 CatmullRom(std::vector<Vector3> points, float t) {
	assert(points.size() >= 4 && "制御点は4点以上必要です");

	// 区間数は制御点の数-1
	size_t division = points.size() - 1;
	// 1区間分の長さ　(全体を1.0とした場合)
	float areaWhidth = 1.0f / division;

	// 区間分の始点を0.0f,終点を1.0fとしたときにの現在位置
	float t_2 = std::fmod(t, areaWhidth) * division;
	// 下限(0,0f)と上限(1.0f)の範囲に収める
	t_2 = Clamp(t_2, 0.0f, 1.0f);

	// 区間番号
	size_t index = static_cast<size_t>(t / areaWhidth);
	// 区間番号が上限を超えないように収める
	if (index >= division) {
		index = division - 1;
	}
	// 4頂点分のインデックス
	size_t index0 = index - 1;
	size_t index1 = index;
	size_t index2 = index + 1;
	size_t index3 = index + 2;

	// 最初の区間のp0はp1を重複使用する
	if (index == 0) {
		index0 = index1;
	}

	// 最後の区間のp3はp2を重複使用する
	if (index3 >= points.size()) {
		index3 = index2;
	}

	// 4点の座標
	const Vector3& p0 = points[index0];
	const Vector3& p1 = points[index1];
	const Vector3& p2 = points[index2];
	const Vector3& p3 = points[index3];


	// 4点を指定してCatmull-Rom補間
	return CatmullRom(p0, p1, p2, p3, t_2);
}

// カーブ上の点を取得 (Catmull-Rom)
Vector3 CatmullRom2(const std::vector<Vector3>& controlPoints, float t) {
	int p0, p1, p2, p3;
	p1 = static_cast<int>(t);
	p2 = (p1 + 1) % (int)controlPoints.size();
	p3 = (p2 + 1) % (int)controlPoints.size();
	p0 = (p1 - 1 + (int)controlPoints.size()) % (int)controlPoints.size();

	t = t - static_cast<int>(t); // 小数部分のみを取り出す

	float t2 = t * t;
	float t3 = t2 * t;

	Vector3 result = Add(
		Add(
			Multiply(controlPoints[p0], -0.5f * t3 + t2 - 0.5f * t),
			Multiply(controlPoints[p1], 1.5f * t3 - 2.5f * t2 + 1.0f)),
		Add(
			Multiply(controlPoints[p2], -1.5f * t3 + 2.0f * t2 + 0.5f * t),
			Multiply(controlPoints[p3], 0.5f * t3 - 0.5f * t2)
		)
	);
	return result;
}
// 2点間の距離を計算する関数
float Distance2(const Vector3& a, const Vector3& b) {
	return std::sqrtf(std::powf(b.x - a.x, 2) + std::powf(b.y - a.y, 2) + std::powf(b.z - a.z, 2));
}

// アーク長を計算する関数
float CalculateArcLength(const std::vector<Vector3>& controlPoints, int numSamples) {
	float totalLength = 0.0f;
	Vector3 prevPos = CatmullRom(controlPoints, 0.0f);

	for (int i = 1; i <= numSamples; ++i) {
		float t = static_cast<float>(i) / numSamples;
		Vector3 currPos = CatmullRom(controlPoints, t);
		totalLength += Distance2(prevPos, currPos);
		prevPos = currPos;
	}

	return totalLength;
}

// アーク長に基づく位置を取得する関数
float FindTByArcLength(const std::vector<Vector3>& controlPoints, float targetLength, int numSamples) {
	float currentLength = 0.0f;
	Vector3 prevPos = CatmullRom(controlPoints, 0.0f);

	for (int i = 1; i <= numSamples; ++i) {
		float t = static_cast<float>(i) / numSamples;
		Vector3 currPos = CatmullRom(controlPoints, t);
		currentLength += Distance2(prevPos, currPos);

		if (currentLength >= targetLength) {
			return t;
		}
		prevPos = currPos;
	}

	return 1.0f; // 最後まで到達した場合
}


// 曲線を細かくサンプリングし、累積アーク長を計算
std::vector<std::pair<float, float>> CalculateArcLengths(const std::vector<Vector3>& controlPoints, int numSamples) {
	std::vector<std::pair<float, float>> arcLengths;
	arcLengths.push_back({ 0.0f, 0.0f });

	float totalLength = 0.0f;
	Vector3 prevPos = CatmullRom(controlPoints, 0.0f);

	for (int i = 1; i <= numSamples; ++i) {
		float t = static_cast<float>(i) / numSamples;
		Vector3 currPos = CatmullRom(controlPoints, t);
		totalLength += Distance2(prevPos, currPos);
		arcLengths.push_back({ t, totalLength });
		prevPos = currPos;
	}

	return arcLengths;
}

// アーク長からtを逆算する関数
float GetTFromArcLength(const std::vector<std::pair<float, float>>& arcLengths, float targetLength) {
	for (size_t i = 1; i < arcLengths.size(); ++i) {
		if (arcLengths[i].second >= targetLength) {
			float t1 = arcLengths[i - 1].first;
			float t2 = arcLengths[i].first;
			float l1 = arcLengths[i - 1].second;
			float l2 = arcLengths[i].second;

			// 線形補間でtを求める
			return t1 + (targetLength - l1) / (l2 - l1) * (t2 - t1);
		}
	}
	return 1.0f;
}

// 曲率を計算する関数
float Curvature(const Vector3& p0, const Vector3& p1, const Vector3& p2) {
	Vector3 v1 = Subtract(p1, p0);
	Vector3 v2 = Subtract(p2, p1);
	float angle = std::acos(Dot(Normalize(v1), Normalize(v2)));
	return angle / Distance2(p0, p1);
}

// 曲率に基づくアダプティブサンプリング
std::vector<float> AdaptiveSampling(const std::vector<Vector3>& controlPoints, int baseSamples) {
	std::vector<float> samplePoints;
	samplePoints.push_back(0.0f);

	for (int i = 1; i < baseSamples; ++i) {
		float t = static_cast<float>(i) / baseSamples;
		Vector3 p0 = CatmullRom2(controlPoints, std::max(t - 0.01f, 0.0f));
		Vector3 p1 = CatmullRom2(controlPoints, t);
		Vector3 p2 = CatmullRom2(controlPoints, std::min(t + 0.01f, 1.0f));

		float curvature = Curvature(p0, p1, p2);
		float sampleRate = std::clamp(1.0f / (curvature + 0.1f), 0.01f, 0.2f);
		samplePoints.push_back(samplePoints.back() + sampleRate);
	}

	return samplePoints;
}

const float* GetMatrixPointer(const Matrix4x4& mat)
{
	return &mat.m[0][0]; // 行列データの先頭要素のアドレスを返す
}

float* GetMatrix(Matrix4x4 mat) {
	return &mat.m[0][0];  // 行列の最初の要素のポインタを返す
}


float DegreesToRadians(float degrees) { return float(degrees * ((float)M_PI / 180.0)); }

Vector3 DegreesToRadians(Vector3 degrees) {
	Vector3 resurt;

	resurt.x = float(degrees.x * ((float)M_PI / 180.0));
	resurt.y = float(degrees.y * ((float)M_PI / 180.0));
	resurt.z = float(degrees.z * ((float)M_PI / 180.0));

	return resurt;
}

float RadiansToDegrees(float radians) { return float(radians * (180.0 / (float)M_PI)); }

Vector3 RadiansToDegrees(Vector3 radians) {
	Vector3 resurt;

	resurt.x = float(radians.x * (180.0 / (float)M_PI));
	resurt.y = float(radians.y * (180.0 / (float)M_PI));
	resurt.z = float(radians.z * (180.0 / (float)M_PI));

	return resurt;
}


Vector3 GenerateRandomPosition(const Vector3& min, const Vector3& max, std::mt19937& randomEngine) {
	std::uniform_real_distribution<float> distX(min.x, max.x);
	std::uniform_real_distribution<float> distY(min.y, max.y);
	std::uniform_real_distribution<float> distZ(min.z, max.z);
	return { distX(randomEngine), distY(randomEngine), distZ(randomEngine) };
}

#pragma endregion //数学関数