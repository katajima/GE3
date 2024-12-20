#pragma once
#include"DirectXGame/engine/math/MathFanctions.h"
#include"map"
#include"optional"


template <typename tValue>
struct Keyframe {
	float time;
	tValue value;
};

template <typename tValue>
struct AnimationCurve
{
	std::vector<Keyframe<tValue>> keyframes;
};

using KeyframeVector3 = Keyframe<Vector3>;
using KeyframeQuaternion = Keyframe<Quaternion>;




struct NodeAnimation
{
	AnimationCurve<Vector3> translate;
	AnimationCurve<Quaternion> rotate;
	AnimationCurve<Vector3> scale;
};

struct Animation
{
	float duration; // アニメーション全体の尺 (単位は秒)
	// NodeAnimationの集合。Node名でひけるようにしておく
	std::map<std::string, NodeAnimation> nodeAnimations;

	bool flag = false; // アニメーションがあるかないか
};

struct EulerTransform {
	Vector3 scale;
	Vector3 ratate; // Eulerでの回転
	Vector3 translate;
};

struct QuaternionTransform
{
	Vector3 scale;
	Quaternion rotate;
	Vector3 tarnslate;
};

struct Joint {
	QuaternionTransform transform; // Transform情報
	Matrix4x4 localMatrix; // localMatrix
	Matrix4x4 skeletonSpaceMatrix; // skeletonSpaceでの変換行列
	std::string name; // 名前
	std::vector<int32_t> children; // 子JointのIndexのリスト。いなければ空
	int32_t index; // 自身のIndex
	std::optional<int32_t> parent; // 親JointのIndex。いなければnull
};

struct  Node
{
	QuaternionTransform transform;
	Matrix4x4 localMatrix;
	std::string name;
	std::vector<Node> children;
};

struct Skeleton {
	int32_t root; // RootJointのIndex
	std::map<std::string, int32_t> jointMap; // Joint名とIndexとの辞書
	std::vector<Joint> joints; // 所属しているジョイント
};

Skeleton CreateSkeleton(const Node& rootNode);

int32_t CreateJoint(const Node& node, const std::optional<int32_t>& parent, std::vector<Joint>& joints);


void ApplyAnimation(Skeleton& skeleton, const Animation& animation, float animationTime);


Vector3 CalculateValue(const std::vector<KeyframeVector3>& keyframes, float time);

Quaternion CalculateValue(const std::vector<KeyframeQuaternion>& keyframes, float time);

void UpdateSkeleton(Skeleton& skeleton);
