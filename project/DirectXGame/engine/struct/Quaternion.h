#include <iostream>
#include <cmath>
#include "Vector3.h"
#include "Matrix4x4.h"
#include <limits>

static float Lerp(float a, float b, float t)
{
    return a + (b - a) * t;
}

struct Quaternion final {
    float x, y, z, w;

    Quaternion(float x = 0, float y = 0, float z = 0, float w = 1)
        : x(x), y(y), z(z), w(w) {}

    static Quaternion IdentityQuaternion(){return Quaternion(0.0f, 0.0f, 0.0f, 1.0f);}

    //共役
    Quaternion Conjugate() {return Quaternion(-x, -y, -z, w);}

    // ノルム
    float Norm() { return std::sqrt(x * x + y * y + z * z + w * w); }

    // クォータニオンの正規化関数
    Quaternion Normalize() const{
        float length = std::sqrt((w * w) + (x * x) + (y * y) + (z * z));
        return {x / length, y / length, z / length,w / length };
    }
    void Normalize2() { 
        float magnitude = std::sqrt(w * w + x * x + y * y + z * z); 
        if (magnitude > FLT_EPSILON) { 
            float invMagnitude = 1.0f / magnitude; 
            w *= invMagnitude; x *= invMagnitude; y *= invMagnitude; z *= invMagnitude; 
        } 
    }

    float Dot(const Quaternion& q) const {

        return q.w * w + q.x *x + q.y * y + q.z * z;
    };


    // 軸と角度からクォータニオンを生成する関数
    Quaternion MakeQuaternion(const Vector3& axis, float angle) {
        Vector3 norAxis = axis.Normalize();

        x = norAxis.x * sinf(angle / 2);
        y = norAxis.y * sinf(angle / 2);
        z = norAxis.z * sinf(angle / 2);
        w = cosf(angle / 2);
        return {x,y,z,w};
    }
   
    
    
    Matrix4x4 MakeRotateMatrix() const {
        Matrix4x4 m{};
        m.Identity();
        float ww = w * w, xx = x * x, yy = y * y, zz = z * z;
        float wx = w * x, wy = w * y, wz = w * z;
        float xy = x * y, xz = x * z;
        float yz = y * z;

        m.m[0][0] = ww + xx - yy -zz;  m.m[0][1] = 2 * (xy + wz);        m.m[0][2] = 2 * (xz - wy);        m.m[0][3] = 0;
        m.m[1][0] = 2 * (xy - wz);     m.m[1][1] = ww - xx + yy - zz;    m.m[1][2] = 2 * (yz + wx);        m.m[1][3] = 0;
        m.m[2][0] = 2 * (xz + wy);	   m.m[2][1] = 2 * (yz - wx);        m.m[2][2] = ww - xx - yy + zz;    m.m[2][3] = 0;
        m.m[3][0] = 0;                 m.m[3][1] = 0;                    m.m[3][2] = 0;                    m.m[3][3] = 1;

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

    Quaternion operator-() const;

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

static Quaternion Lerp(const Quaternion& q0, const Quaternion& q1, float _t)
{
    return Quaternion(Lerp(q0.x, q1.x, _t), Lerp(q0.y, q1.y, _t), Lerp(q0.z, q1.z, _t), Lerp(q0.w, q1.w, _t));
}

static Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, float t) {
    Quaternion q0_t = q0, q1_t = q1;

    // 内積を計算
    float dot = Dot(q0_t, q1_t);

    

    // 負の内積の場合、片方の符号を反転して最短経路に補間
    if (dot < 0.0f) {
        q1_t = -q1_t; // クォータニオン全体を反転
        dot = -dot;
    }

    // 内積が1に近い場合（ほぼ同一方向）の処理
    const float DOT_THRESHOLD = 1.0f - FLT_EPSILON; // 定数名を定義
    if (dot >= DOT_THRESHOLD) {
        Quaternion result{};
        result = Lerp(q0_t, q1_t, t);
        result.Normalize(); // 正規化を追加
        return result;
    }

    // なす角を計算
    float theta = std::acos(dot);
    float sinTheta = std::sin(theta);

    // sinThetaが小さい場合（数値的に不安定な場合）の処理
    if (sinTheta < FLT_EPSILON) {
        Quaternion result{};
        result = Lerp(q0_t, q1_t, t);
        result.Normalize(); // 正規化を追加
        return result;
    }

    // 補間係数を計算
    float scale0 = std::sin((1.0f - t) * theta) / sinTheta;
    float scale1 = std::sin(t * theta) / sinTheta;

    // 補間後のクォータニオンを計算
    Quaternion result{
        (scale0 * q0_t.x) + (scale1 * q1_t.x),
        (scale0 * q0_t.y) + (scale1 * q1_t.y),
        (scale0 * q0_t.z) + (scale1 * q1_t.z),
        (scale0 * q0_t.w) + (scale1 * q1_t.w)
    };
    result.Normalize(); // 最終結果の正規化
    return result.Normalize();
}
static Quaternion Slerp2(const Quaternion& q0, const Quaternion& q1, float t) {
    Quaternion q0_t = q0, q1_t = q1;

    float dot = Dot(q0_t, q1_t); // 内積

    if (dot < 0) {
        q0_t.Conjugate(); // もう片方の回転を利用する
        dot =-dot;
    }

    float theta = std::acos(dot);

    float scale0 = std::sin((1 - t) * theta) / std::sin(theta);
    float scale1 = std::sin(t * theta) / std::sin(theta);

    return Quaternion{ ((scale0 * q0_t.x) + (scale1 * q1_t.x)),((scale0 * q0_t.y) + (scale1 * q1_t.y)),
         ((scale0 * q0_t.z) + (scale1 * q1_t.z)), ((scale0 * q0_t.w) + (scale1 * q1_t.w)) };

}





