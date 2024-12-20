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

    // クォータニオンを使用してベクトルを回転させる関数
    Vector3 RotateVector(const Vector3& v) const {
        Quaternion qv = {v.x, v.y, v.z ,0};
        Quaternion qConjugate = {-x, -y, -z ,w};

        Quaternion temp;
        temp.w = w * qv.w - x * qv.x - y * qv.y - z * qv.z;
        temp.x = w * qv.x + x * qv.w + y * qv.z - z * qv.y;
        temp.y = w * qv.y - x * qv.z + y * qv.w + z * qv.x;
        temp.z = w * qv.z + x * qv.y - y * qv.x + z * qv.w;

        Quaternion result;
        result.w = temp.w * qConjugate.w - temp.x * qConjugate.x - temp.y * qConjugate.y - temp.z * qConjugate.z;
        result.x = temp.w * qConjugate.x + temp.x * qConjugate.w + temp.y * qConjugate.z - temp.z * qConjugate.y;
        result.y = temp.w * qConjugate.y - temp.x * qConjugate.z + temp.y * qConjugate.w + temp.z * qConjugate.x;
        result.z = temp.w * qConjugate.z + temp.x * qConjugate.y - temp.y * qConjugate.x + temp.z * qConjugate.w;

        return { result.x, result.y, result.z };
    }

   
    // + 演算子のオーバーロード
    Quaternion operator+(const Quaternion& other) const;

    // - 演算子のオーバーロード
    Quaternion operator-(const Quaternion& other) const;

    // * 演算子のオーバーロード
    Quaternion operator*(const Quaternion& other) const;

    Quaternion operator*(const float& other) const;



};

// 内積の計算
static float Dot(const Quaternion& q1, const Quaternion& q2) {
    return q1.w * q2.w + q1.x * q2.x + q1.y * q2.y + q1.z * q2.z;
}

static Quaternion Inverse(Quaternion& qua)
{
    float norm = qua.Norm();
    float normSquared = norm * norm; // ノルムの二乗を計算
    Quaternion conjugate = qua.Conjugate();
    return Quaternion(conjugate.x / normSquared, conjugate.y / normSquared, conjugate.z / normSquared, conjugate.w / normSquared);
}

static Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, float t) {
    Quaternion q0_t = q0, q1_t = q1;

    float dot = Dot(q0_t, q1_t); // 内積
    if (dot < 0) {
        q0_t.Conjugate(); // もう片方の回転を利用する
        dot = -dot;
    }


    // なす角を求める
    float theta = std::acos(dot);

    // ... thetaとsinを使って補間関数scale0,scale1を求める
    float scale0 = std::sin((1 - t) * theta) / std::sin(theta); 
    float scale1 = std::sin(t * theta) / std::sin(theta);

   
    // それぞれの補間関数を利用して補間後のQuaternionを求める
    return Quaternion{ ((scale0 * q0_t.x) + (scale1 * q1_t.x)),((scale0 * q0_t.y) + (scale1 * q1_t.y)),
    ((scale0 * q0_t.z) + (scale1 * q1_t.z)), ((scale0 * q0_t.w) + (scale1 * q1_t.w)) };
}



