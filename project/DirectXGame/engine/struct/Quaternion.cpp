#include"Quaternion.h"

// + 演算子のオーバーロード
Quaternion Quaternion::operator+(const Quaternion& other) const {

    return Quaternion(x + other.x, y + other.y, z + other.z, w + other.w);
}

// - 演算子のオーバーロード
Quaternion Quaternion::operator-(const Quaternion& other) const {
    return Quaternion(x - other.x, y - other.y, z - other.z, w - other.w);
}

// * 演算子のオーバーロード（クォータニオンの乗算）
Quaternion Quaternion::operator*(const Quaternion& other) const {
    return Quaternion(
        w * other.x + x * other.w + y * other.z - z * other.y,
        w * other.y - x * other.z + y * other.w + z * other.x,
        w * other.z + x * other.y - y * other.x + z * other.w,
        w * other.w - x * other.x - y * other.y - z * other.z
    );
}
Quaternion Quaternion::operator*(const float& other) const { 
    return Quaternion(w * other, x * other, y * other, z * other); 
}

Quaternion Quaternion::operator-() const
{
    return Quaternion(-x, -y, -z, -w);
}

