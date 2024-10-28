#pragma once
#include"DirectXGame/engine/base/DirectXCommon.h"

// 3Dモデル共通部分
class ModelCommon
{
public:
	// インスタンス
	static ModelCommon* GetInstance();
	// 初期化
	void Initialize(DirectXCommon* dxCommon);

	DirectXCommon* GetDxCommon() const { return dxCommon_; }
private:
	DirectXCommon* dxCommon_;

};

