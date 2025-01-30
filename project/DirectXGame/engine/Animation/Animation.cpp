#include "Animation.h"

Skeleton CreateSkeleton(const Node& rootNode)
{
	Skeleton skeleton{};
	skeleton.root = CreateJoint(rootNode, {}, skeleton.joints);

	// 名前とindexのマッピングを行いアクセスしやすくする
	for (const Joint& joint : skeleton.joints) {
		skeleton.jointMap.emplace(joint.name, joint.index);
	}

	UpdateSkeleton(skeleton);

	return skeleton;
}

int32_t CreateJoint(const Node& node, const std::optional<int32_t>& parent, std::vector<Joint>& joints)
{
	Joint joint{};
	joint.name = node.name;
	joint.localMatrix = node.localMatrix;
	joint.skeletonSpaceMatrix  = MakeIdentity4x4();
	joint.transform = node.transform;
	joint.index = int32_t(joints.size()); // 登録されている数をIndexに
	joint.parent = parent;
	joints.push_back(joint); // SkeletonのJoint列に追加
	for (const Node& child : node.children) {
		// 子Jointを作成し、そのIndexを登録
		int32_t childIndex = CreateJoint(child, joint.index, joints);
		joints[joint.index].children.push_back(childIndex);
	}
	// 自身のIndexを返す
	return joint.index;
}

void ApplyAnimation(Skeleton& skeleton, const Animation& animation, float animationTime)
{
	
	for (Joint& joint : skeleton.joints) {
		// 対象のJointのAnimationがあれば、値の適用を行う。下記のif文はC++17から可能になった初期化付きif文。
		if (auto it = animation.nodeAnimations.find(joint.name); it != animation.nodeAnimations.end()) {
			const NodeAnimation& rootNodeAnimation = (*it).second;
			joint.transform.translate = CalculateValue(rootNodeAnimation.translate.keyframes, animationTime);
			joint.transform.rotate = CalculateValue(rootNodeAnimation.rotate.keyframes, animationTime);
			joint.transform.scale = CalculateValue(rootNodeAnimation.scale.keyframes, animationTime);
		}
	}
}



Vector3 CalculateValue(const std::vector<KeyframeVector3>& keyframes, float time)
{
	assert(!keyframes.empty()); // キーがないものは返す値がわからないのでダメ
	if (keyframes.size() == 1 || time <= keyframes[0].time) { // キーが一つか、時刻がキーフレーム前なら最初の値を返す
		return keyframes[0].value;
	}

	for (size_t index = 0; index < keyframes.size() - 1; ++index) {
		size_t nextIndex = index + 1;
		// indexとnextIndexの2つのkeyframeを取得して範囲内に時刻があるかを判断
		if (keyframes[index].time <= time && time <= keyframes[nextIndex].time) {
			// 範囲内を補間する
			float t = (time - keyframes[index].time) / (keyframes[nextIndex].time - keyframes[index].time);
			return Lerp(keyframes[index].value, keyframes[nextIndex].value, t);
		}
	}
	// ここまで来た場合は一番最後の時刻よりも後ろなので最後の値を返すことにする
	return (*keyframes.rbegin()).value;
}

Quaternion CalculateValue(const std::vector<KeyframeQuaternion>& keyframes, float time)
{
	assert(!keyframes.empty()); // キーがないものは返す値がわからないのでダメ
	if (keyframes.size() == 1 || time <= keyframes[0].time) {
		return keyframes[0].value; // 最初のキーを返す
	}

	if (time >= keyframes.back().time) {
		return keyframes.back().value; // 最後のキーを返す
	}


	for (size_t index = 0; index < keyframes.size() - 1; ++index) {
		size_t nextIndex = index + 1;
		// indexとnextIndexの2つのkeyframeを取得して範囲内に時刻があるかを判断
		if (keyframes[index].time <= time && time <= keyframes[nextIndex].time) {
			// 範囲内を補間する
			float t = (time - keyframes[index].time) / (keyframes[nextIndex].time - keyframes[index].time);
			return Slerp(keyframes[index].value, keyframes[nextIndex].value, t);
		}
	}
	// ここまで来た場合は一番最後の時刻よりも後ろなので最後の値を返すことにする
	return (*keyframes.rbegin()).value;
}

void UpdateSkeleton(Skeleton& skeleton)
{
	// すべてのJointを更新。親が先に処理されるので通常ループで処理可能になっている
	for (Joint& joint : skeleton.joints) {
		// ローカル行列を計算
		
		joint.localMatrix = MakeAffineMatrix(joint.transform.scale, joint.transform.rotate, joint.transform.translate);
		
		// 親が存在する場合は親の変換を適用
		if (joint.parent) {
			// 親の行列を先に掛ける
			joint.skeletonSpaceMatrix = joint.localMatrix * skeleton.joints[*joint.parent].skeletonSpaceMatrix;
		}
		else {
			// 親がいない場合はローカル行列がそのままスケルトンスペース行列になる
			joint.skeletonSpaceMatrix = joint.localMatrix;
		}
	}
}



