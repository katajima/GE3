#pragma once
#include <windows.h>
#include<cstdint>
#include<string>
#include<fstream>
#include<sstream>
#include<wrl.h>
#include<d3d12.h>
#include<dxgi1_6.h>
#include<dxcapi.h>
using namespace Microsoft::WRL;
#include<vector>
#include"externals/DirectXTex/DirectXTex.h"
#include"externals/DirectXTex/d3dx12.h"
#include"DirectXGame/engine/math/MathFanctions.h"
#include"DirectXGame/engine/struct/Structs.h"
#include"DirectXCommon.h"
#include"SrvManager.h"
#include<random>
#include<numbers>

struct Particle
{
	Transform transform;
	Vector3 velocity;
	Vector4 color;
	float lifeTime;
	float currentTime;
};
struct ParticleForGPU
{
	Matrix4x4 WVP;
	Matrix4x4 World;
	Vector4 color;
};
// 頂点データ
struct VertexData {

	Vector4 position;
	Vector2 texcoord;
	Vector3 normal;
};
//マテリアルデータ
struct MaterialData {
	std::string textuerFilePath;
	//テクスチャ番号
	uint32_t textureIndex = 0;
};
struct  Node
{
	Matrix4x4 localMatrix;
	std::string name;
	std::vector<Node> children;
};
//モデルデータ
struct ModelData
{
	std::vector<VertexData> vertices;
	MaterialData material;
	Node rootNode;
};
struct AcceleraionField {
	Vector3 acceleration;
	AABB area;
};
struct ParticleGroup
{
	MaterialData materialData;
	std::list<Particle> particle;
	uint32_t srvIndex;
	Microsoft::WRL::ComPtr<ID3D12Resource> resource;
	uint32_t instanceCount; // インスタンス数
	ParticleForGPU* instanceData; // インスタンシングデータを書き込むためのポインタ
};
class ParticleManager
{
private:


public:
	// シングルトンインスタンス
	static ParticleManager* GetInstance();

	// 初期化
	void Initialize(DirectXCommon* dxCommon, SrvManager* srvManager);

	//
	void Finalize();

	// 更新
	void Update();

	// 描画
	void Draw();

	// パーティクルの発生
	void Emit(const std::string name, const Vector3& position, uint32_t count);

	std::unordered_map<std::string, ParticleGroup>& GetParticleGroups()
	{
		return particleGroups;
	}

	void CreateParticleGroup(const std::string name, const std::string textureFilePath);

	Particle MakeNewParticle(std::mt19937& randomEngine, const Vector3& translate);
	
private:
	// ルートシグネチャの作成
	void CreateRootSignature();
	// グラフィックスパイプラインの作成
	void CreateGraphicsPipeline();
private:
	static ParticleManager* instance;
	ParticleManager() = default;
	~ParticleManager() = default;
	ParticleManager(ParticleManager&) = delete;
	ParticleManager& operator=(ParticleManager&) = delete;

	std::mt19937 randomEngine_;

	std::unordered_map<std::string, ParticleGroup> particleGroups;


	const uint32_t kNumMaxInstance = 100;
	const float kDeltaTime = 1.0f / 60.0f;
	bool usebillboard = true;
	bool upData = true;
	bool upDataWind = false;
	uint32_t numInstance;

	AcceleraionField acceleraionField;
	
	
	

	DirectXCommon* dxCommon_ = nullptr;
	SrvManager* srvManager_ = nullptr;

	// バッファリソース
	Microsoft::WRL::ComPtr < ID3D12Resource> vertexResource;
	// バッファリソース内のデータを指すポインタ
	VertexData* vertexData = nullptr;

	//バッファリソースの使い道を補足するバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	


	Transform transform;

	//ルートシグネチャデスク
	D3D12_ROOT_SIGNATURE_DESC descriptionSignature{};
	//ルートシグネチャ
	Microsoft::WRL::ComPtr < ID3D12RootSignature> rootSignature;
	// グラフィックスパイプラインステート
	Microsoft::WRL::ComPtr < ID3D12PipelineState> graphicsPipelineState = nullptr;

};

