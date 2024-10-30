#include"MathFanctions.h"
#include<cmath>
#include"assert.h"
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

Vector3 Subtract(const Vector3& v1, const Vector3& v2) {
	Vector3 result{};

	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;


	return result;
};

float Dot(const Vector3& v1, const Vector3& v2) {
	float result;

	result = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;


	return result;
};

float Length(const Vector3& v) {
	float result;

	result = sqrtf((v.x * v.x) + (v.y * v.y) + (v.z * v.z));

	return result;
};
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
Vector3 Normalize(const Vector3& v) {
	Vector3 result{};
	float length;

	length = sqrtf((v.x * v.x) + (v.y * v.y) + (v.z * v.z));

	if (length != 0.0f) {
		result.x = v.x / length;
		result.y = v.y / length;
		result.z = v.z / length;
	};

	return result;
};

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
				result.m[y][x] += v1.m[y][z] * v2.m[z][x];
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
Matrix4x4 MakeAffineMatrixMatrix(const  Vector3& scale, const  Vector3& rotate, const  Vector3& translate) {
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



	return result;
};
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

bool IsCollision(const AABB& aabb, const Vector3& point) {
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
#pragma endregion //数学関数