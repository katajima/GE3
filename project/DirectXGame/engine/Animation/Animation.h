#pragma once
#include"DirectXGame/engine/math/MathFanctions.h"
#include"DirectXGame/engine/struct/Vector3.h"

#include"map"
#include"optional"
#include <array>
#include<d3d12.h>
#include<dxgi1_6.h>
#include<cstdint>
#include<wrl.h>
#include<string>
#include<vector>
#include<format>
#include<span>

#include"DirectXGame/engine/Line/Line.h"
#include"DirectXGame/engine/Line/LineCommon.h"

#include"imgui.h"


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
	Vector3 translate;
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

struct VertexWeightData {
	float weight;
	uint32_t vertexIndex;
};
struct JointWeightData {
	Matrix4x4 inverseBindPoseMatrix;
	std::vector<VertexWeightData> vertexWeights;
};

const uint32_t kNumMaxInfluence = 4;
struct VertexInfluence {
	std::array<float, kNumMaxInfluence> weights;
	std::array<int32_t, kNumMaxInfluence> jointIndices;
};
struct WellForGPU {
	Matrix4x4 skeletonSpaceMatrix; // 位置用
	Matrix4x4 skeletonSpaceInverseTransposeMatrix; // 法線用
};

struct SkinCluster {
	std::vector<Matrix4x4> inverseBindPoseMatrices;

	Microsoft::WRL::ComPtr < ID3D12Resource> influenceResource;
	D3D12_VERTEX_BUFFER_VIEW influenceBufferView;
	std::span<VertexInfluence> mappedInfluence;

	Microsoft::WRL::ComPtr < ID3D12Resource> paletteResource;
	std::span<WellForGPU> mappedPalette;
	std::pair<D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_GPU_DESCRIPTOR_HANDLE> paletteSrvHandle;

};







Skeleton CreateSkeleton(const Node& rootNode);

int32_t CreateJoint(const Node& node, const std::optional<int32_t>& parent, std::vector<Joint>& joints);


void ApplyAnimation(Skeleton& skeleton, const Animation& animation, float animationTime);


Vector3 CalculateValue(const std::vector<KeyframeVector3>& keyframes, float time);

Quaternion CalculateValue(const std::vector<KeyframeQuaternion>& keyframes, float time);

void UpdateSkeleton(Skeleton& skeleton);


static void UpdateSkinCluster(SkinCluster& skinCluster, const Skeleton& skeleton) {
	// サイズチェック
	assert(skinCluster.inverseBindPoseMatrices.size() == skeleton.joints.size());
	assert(skinCluster.mappedPalette.size() == skeleton.joints.size());

	for (size_t jointIndex = 0; jointIndex < skeleton.joints.size(); ++jointIndex) {
		// スケルトンスペース行列を計算
		skinCluster.mappedPalette[jointIndex].skeletonSpaceMatrix =
			skinCluster.inverseBindPoseMatrices[jointIndex] * skeleton.joints[jointIndex].skeletonSpaceMatrix;

		// 逆転置行列を計算
		skinCluster.mappedPalette[jointIndex].skeletonSpaceInverseTransposeMatrix =
			Transpose(Inverse(skinCluster.mappedPalette[jointIndex].skeletonSpaceMatrix));
	}
}



static void ValidateTransform(Joint& joint) {
	if (joint.transform.scale.x == 0.0f || joint.transform.scale.y == 0.0f || joint.transform.scale.z == 0.0f) {
		//Logger::Log("Warning: Zero scale detected. Adjusting to default value.");
		joint.transform.scale = { 1.0f, 1.0f, 1.0f }; // デフォルト値に置き換え
	}
}

static float SafeDivide(float numerator, float denominator) {
	// ゼロチェック
	if (denominator == 0.0f) {
		// デバッグ用に警告を出力
		//Logger::Log("Warning: Division by zero detected. Returning default value.");
		// デフォルト値を返すか、エラー処理を行う
		return 0.0f; // または適切な値に置き換える
	}
	return numerator / denominator;
}

static float SafeInverse(float value) {
	const float epsilon = 1e-6f; // 非常に小さな値
	return (value != 0.0f) ? (1.0f / value) : (1.0f / epsilon);
}

static void ImGuiJoint(const std::vector<Joint>& joints) {
	ImGui::Begin("Joint Info");
	for (const Joint& joint : joints) {
		//ImGui::Text("Joint Name: %s", joint.name.c_str());
		if (ImGui::CollapsingHeader(joint.name.c_str())) {
			
			ImGui::Text("Joint Index: %d", joint.index);


			Vector3 pos = joint.skeletonSpaceMatrix.GetWorldPosition();
			ImGui::InputFloat3("Position", &pos.x, "%.3f");
			ImGui::Separator();
			Matrix4x4 mat4x4 = joint.skeletonSpaceMatrix;
			ImGui::InputFloat4("mat[0][~]", &mat4x4.m[0][0], "%.3f");
			ImGui::InputFloat4("mat[1][~]", &mat4x4.m[1][0], "%.3f");
			ImGui::InputFloat4("mat[2][~]", &mat4x4.m[2][0], "%.3f");
			ImGui::InputFloat4("mat[3][~]", &mat4x4.m[3][0], "%.3f");
			
		}
	}
	ImGui::End();
}


static void ImGuiNode(const std::vector<Node>& nodes) {
	ImGui::Begin("Node Info");
	for (const Node& node : nodes) {
		ImGui::Text("Node Name: %s", node.name.c_str());


		Vector3 pos = node.localMatrix.GetWorldPosition();
		ImGui::InputFloat4("Position", &pos.x, "%.3f");
		Matrix4x4 mat4x4 = node.localMatrix;
		ImGui::InputFloat4("mat[0][~]", &mat4x4.m[0][0], "%.3f");
		ImGui::InputFloat4("mat[1][~]", &mat4x4.m[1][0], "%.3f");
		ImGui::InputFloat4("mat[2][~]", &mat4x4.m[2][0], "%.3f");
		ImGui::InputFloat4("mat[3][~]", &mat4x4.m[3][0], "%.3f");
		ImGui::Separator();
	}
	ImGui::End();
}



static void InitializeDrawLineSkeleton(const std::vector<Joint>& joints, std::vector<std::unique_ptr<LineDraw>>& lines) {
	// `lines` ベクターがジョイント数に応じて初期化されているか確認
	if (lines.size() < joints.size()) {
		lines.resize(joints.size());
		for (size_t i = 0; i < joints.size(); ++i) {
			lines[i] = std::make_unique<LineDraw>();
			lines[i]->Initialize();
		}
	}
}

static void UpdateLineSkeleton(const std::vector<Joint>& joints, std::vector<std::unique_ptr<LineDraw>>& lines, Camera* camera) {


	// 親子関係に基づいてラインを描画
	for (const Joint& joint : joints) {
		if (joint.parent.has_value()) { // 親ジョイントが存在する場合のみ描画
			// LineDraw インスタンスを使用して親子間を描画
			if (lines[joint.index]) {
				lines[joint.index]->SetCamera(camera);

				lines[joint.index]->Update();
			}
		}
	}
}

// ジョイントの深さを計算する関数
static int CalculateDepth(const std::vector<Joint>& joints, int index) {
	const Joint& joint = joints[index];
	if (!joint.parent.has_value()) {
		return 0; // ルートジョイントの深さは0
	}
	return 1 + CalculateDepth(joints, joint.parent.value());
}

// スケルトンの描画
static void DrawSkeleton(const std::vector<Joint>& joints, std::vector<std::unique_ptr<LineDraw>>& lines, const Vector3& pos, const Vector3& scale) {
	// ジョイントごとの深さを計算して保存
	std::vector<int> depths(joints.size(), 0);
	int maxDepth = 0;
	for (size_t i = 0; i < joints.size(); ++i) {
		depths[i] = CalculateDepth(joints, static_cast<int>(i));
		maxDepth = (std::max)(maxDepth, depths[i]);
	}


	for (const Joint& joint : joints) {
		if (joint.parent.has_value()) {
			const int32_t parentIndex = joint.parent.value();
			//const Vector3& parentPosition = joints[parentIndex].transform.translate;
			//const Vector3& childPosition = joint.transform.translate;

			const Vector3& parentPosition = joints[parentIndex].skeletonSpaceMatrix.GetWorldPosition() * scale;
			const Vector3& childPosition = joint.skeletonSpaceMatrix.GetWorldPosition() * scale;


			if (lines[joint.index]) {
				Vector3 offsetParentPosition = Add(parentPosition, pos);
				Vector3 offsetChildPosition = Add(childPosition, pos);

				
				// 深さに基づいて色を決定（白から黒へのグラデーション）
				float depthFactor = static_cast<float>(depths[joint.index]) / static_cast<float>(maxDepth);
				Vector4 color = { depthFactor, 1.0f, 1.0f, 1.0f }; // グラデーション (黒 → 白)

				// 線を描画
				lines[joint.index]->Draw3D(
					offsetParentPosition,
					offsetChildPosition,
					color
				);
			}
		}
	}

}



