#include "DebugCamera.h"

void DebugCamera::Initialize(Input *input)
{
    input_ = input;
}

void DebugCamera::Update()
{
    InputKeyCamera();

    UpdateMatrix();
}

void DebugCamera::UpdateMatrix()
{
    // 角度から回転行列を計算する
    Matrix4x4 rotateX = MakeRotateXMatrix(rotation_.x);
    Matrix4x4 rotateY = MakeRotateYMatrix(rotation_.y);
    Matrix4x4 rotateZ = MakeRotateZMatrix(rotation_.z);
    Matrix4x4 rotationMatrix = Multiply(Multiply(rotateX, rotateY), rotateZ);

    // 座標から平行移動行列を計算する
    Matrix4x4 translationMatrix = MakeTranslateMatrix(translation_);

    // 回転行列と平行移動行列からワールド行列を計算する
    worldMatrix_ = Multiply(translationMatrix, rotationMatrix);

    // ワールド行列の逆行列をビュー行列に代入する
    viewMatrix_ = Inverse(worldMatrix_);
}

void DebugCamera::InputKeyCamera()
{

    if (input_->PushKey(DIK_D)) {
        const float speed = 1.0f;

        // カメラベクトル
        Vector3 move = { speed,0,0 };


        Matrix4x4 rotateX = MakeRotateXMatrix(rotation_.x);
        Matrix4x4 rotateY = MakeRotateYMatrix(rotation_.y);
        Matrix4x4 rotateZ = MakeRotateZMatrix(rotation_.z);
        Matrix4x4 rotationMatrix = Multiply(Multiply(rotateX, rotateY), rotateZ);

        move = Transforms(move, rotationMatrix);

        // Add the movement vector to the translation vector
        translation_ = Add(translation_, move);
    }
    if (input_->PushKey(DIK_A)) {
        const float speed = -1.0f;

        // カメラベクトル
        Vector3 move = { speed,0,0 };


        Matrix4x4 rotateX = MakeRotateXMatrix(rotation_.x);
        Matrix4x4 rotateY = MakeRotateYMatrix(rotation_.y);
        Matrix4x4 rotateZ = MakeRotateZMatrix(rotation_.z);
        Matrix4x4 rotationMatrix = Multiply(Multiply(rotateX, rotateY), rotateZ);

        move = Transforms(move, rotationMatrix);

        // Add the movement vector to the translation vector
        translation_ = Add(translation_, move);
    }
    if (input_->PushKey(DIK_W)) {
        const float speed = 1.0f;

        // カメラベクトル
        Vector3 move = { 0,speed,0 };


        Matrix4x4 rotateX = MakeRotateXMatrix(rotation_.x);
        Matrix4x4 rotateY = MakeRotateYMatrix(rotation_.y);
        Matrix4x4 rotateZ = MakeRotateZMatrix(rotation_.z);
        Matrix4x4 rotationMatrix = Multiply(Multiply(rotateX, rotateY), rotateZ);

        move = Transforms(move, rotationMatrix);

        // Add the movement vector to the translation vector
        translation_ = Add(translation_, move);
    }
    if (input_->PushKey(DIK_S)) {
        const float speed = -1.0f;

        // カメラベクトル
        Vector3 move = { 0,speed,0 };


        Matrix4x4 rotateX = MakeRotateXMatrix(rotation_.x);
        Matrix4x4 rotateY = MakeRotateYMatrix(rotation_.y);
        Matrix4x4 rotateZ = MakeRotateZMatrix(rotation_.z);
        Matrix4x4 rotationMatrix = Multiply(Multiply(rotateX, rotateY), rotateZ);

        move = Transforms(move, rotationMatrix);

        // Add the movement vector to the translation vector
        translation_ = Add(translation_, move);
    }

}
