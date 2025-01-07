#pragma once
#include"DirectXGame/engine/Camera/Camera.h"
#include"DirectXGame/engine/3d/Object3d.h"
#include"DirectXGame/engine/base/ImGuiManager.h"
#include"DirectXGame/engine/math/MathFanctions.h"



#include "DirectXGame/engine/collider/Collider.h"
#include"DirectXGame/engine/collider/ContactRecord.h"



class Player;

class playerWeapon : public Collider
{
public:

	///< summary>
	/// 初期化
	///</summary>
	void Initialize(Camera* camera);

	///< summary>
	/// 更新
	///</summary>
	void Update();

	///< summary>
	/// 描画
	///</summary>
	void Draw();

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision([[maybe_unused]] Collider* other) override;

	virtual Vector3 GetCenterPosition() const;



	Object3d& GetObject3D() { return objectWeapon_; }

	// カメラのビュープロジェクション
	void SetCamera(const Camera* camera) { camera_ = camera; };

	void SetOffset(const Vector3& translation) { offset = translation; }

	// 接触履歴を抹消
	void ContactRecordClear();

	void SetRad(float rad) { Collider::SetRadius(rad); };

	void SetPlayer(Player* player);

private:
	// 武器
	Object3d objectWeapon_;
	const Camera* camera_ = nullptr;
	ContactRecord contactRecord_;
	Vector3 offset;
	Player* player_;
};

