#include "WorldTransform.h"

void WorldTransform::Initialize()
{
	scale_ = { 1,1,1 };
	rotate_ = {};
	translate_ = { 0,0,0 };
	worldMat_ = MakeIdentity4x4();
	worldPreMat_ = MakeIdentity4x4();
}

void WorldTransform::Update()
{
	worldPreMat_ = worldMat_;

	worldMat_ = MakeAffineMatrix(scale_, rotate_, translate_);

	// 親がいれば
	if (parent_) {
		worldMat_ = worldMat_ * parent_->worldMat_;
	}
}
