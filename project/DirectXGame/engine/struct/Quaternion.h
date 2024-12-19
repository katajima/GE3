#include <iostream>
#include <cmath>
#include "Vector3.h"
#include "Matrix4x4.h"

struct Quaternion final {
    float x, y, z, w;

    Quaternion(float x = 0, float y = 0, float z = 0, float w = 1)
        : x(x), y(y), z(z), w(w) {}

    Quaternion IdentityQuaternion(){return Quaternion(0.0f, 0.0f, 0.0f, 1.0f);}

    //共役
    Quaternion Conjugate() {return Quaternion(-x, -y, -z, w);}

    // ノルム
    float Norm() { return std::sqrt(x * x + y * y + z * z + w * w); }

    // クォータニオンの正規化関数
    Quaternion Normalize() {
        float length = std::sqrt((w * w) + (x * x) + (y * y) + (z * z));
        return {x / length, y / length, z / length,w / length};
    }

    // 軸と角度からクォータニオンを生成する関数
    Quaternion MakeQuaternion(const Vector3& axis, float angle) {
        x = axis.x * sinf(angle / 2);
        y = axis.y * sinf(angle / 2);
        z = axis.z * sinf(angle / 2);
        w = cosf(angle / 2);
        return {x,y,z,w};
    }
    Matrix4x4 MakeRotateMatrix() {
        Matrix4x4 m{};
        float ww = w * w, xx = x * x, yy = y * y, zz = z * z;
        float wx = w * x, wy = w * y, wz = w * z;
        float xy = x * y, xz = x * z;
        float yz = y * z;

        m.m[0][0] = ww + xx - yy -zz;  m.m[0][1] = 2 * (xy + wz);        m.m[0][2] = 2 * (xz - wy);       m.m[0][3] = 0;
        m.m[1][0] = 2 * (xy - wz);     m.m[1][1] = ww - xx + yy - zz;    m.m[1][2] = 2 * (yz + wx);       m.m[1][3] = 0;
        m.m[2][0] = 2 * (xz + wy);	   m.m[2][1] = 2 * (yz - wx);        m.m[2][2] = ww - xx - yy + zz;   m.m[2][3] = 0;
        m.m[3][0] = 0;                 m.m[3][1] = 0;                    m.m[3][2] = 0;                   m.m[3][3] = 1;

        return m;
    }
    

    // + 演算子のオーバーロード
    Quaternion operator+(const Quaternion& other) const;

    // - 演算子のオーバーロード
    Quaternion operator-(const Quaternion& other) const;

    // * 演算子のオーバーロード
    Quaternion operator*(const Quaternion& other) const;



};

static Quaternion Inverse(Quaternion& qua)
{
    float norm = qua.Norm();
    float normSquared = norm * norm; // ノルムの二乗を計算
    Quaternion conjugate = qua.Conjugate();
    return Quaternion(conjugate.x / normSquared, conjugate.y / normSquared, conjugate.z / normSquared, conjugate.w / normSquared);
}



