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


class ParticleManager
{
public:
	struct Particle
	{
		Transform transform;
		Vector3 velocity;
		Vector4 color;
		float lifeTime;
		float currentTime;
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
		D3D12_GPU_DESCRIPTOR_HANDLE instancingSrvHandleGPU;
	};

public:
	// シングルトンインスタンス
	static ParticleManager* GetInstance();

	// 初期化
	void Initialize(DirectXCommon* dxCommon/*, SrvManager* srvManager*/);
	// 更新
	void Update();
	// 描画
	void Draw();
	// 終了
	void Finalize();

	

	// パーティクルの発生
	void Emit(const std::string name, const Vector3& position, uint32_t count);

	std::unordered_map<std::string, ParticleGroup>& GetParticleGroups()
	{
		return particleGroups;
	}

	void CreateParticleGroup(const std::string name, const std::string textureFilePath);

	//Particle MakeNewParticle(std::mt19937& randomEngine, const Vector3& translate);
	
	//Vector3 GetPoa() { return poa; }

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

	DirectXCommon* dxCommon_ = nullptr;
	SrvManager* srvManager_ = nullptr;


	//// 平行高原
	//struct DirectionalLight {
	//	Vector4 color; //!< ライトの色
	//	Vector3 direction; //!< ライトの向き
	//	float intensity; //!< 輝度
	//};	
	////マテリアルデータ
	//struct Material {
	//	Vector4 color;
	//	int32_t enableLighting;
	//	float padding[3];
	//	Matrix4x4 uvTransform;
	//};
	//Microsoft::WRL::ComPtr < ID3D12Resource> directionalLightResource;
	//DirectionalLight* directionalLightData = nullptr;
	////マテリアル用のリソースを作る。今回はcolor1つ分のサイズを利用する
	//Microsoft::WRL::ComPtr < ID3D12Resource> materialResource;

	//// Lightingを有効にする
	//Material* materialData = nullptr;

	std::mt19937 randomEngine_;


	std::unordered_map<std::string, ParticleGroup> particleGroups;


	const uint32_t kNumMaxInstance = 100;
	const float kDeltaTime = 1.0f / 60.0f;
	bool usebillboard = true;
	bool upData = true;
	bool upDataWind = false;
	uint32_t numInstance;

	AcceleraionField acceleraionField;
	
	Vector3 poa;
	

	
	//// バッファリソース
	//Microsoft::WRL::ComPtr < ID3D12Resource> vertexResource;
	//// バッファリソース内のデータを指すポインタ
	//VertexData* vertexData = nullptr;

	////バッファリソースの使い道を補足するバッファビュー
	//D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	


	Transform transform;

	////ルートシグネチャデスク
	//D3D12_ROOT_SIGNATURE_DESC descriptionSignature{};
	////ルートシグネチャ
	//Microsoft::WRL::ComPtr < ID3D12RootSignature> rootSignature;
	//// グラフィックスパイプラインステート
	//Microsoft::WRL::ComPtr < ID3D12PipelineState> graphicsPipelineState = nullptr;

	//D3D12_CPU_DESCRIPTOR_HANDLE instancingSrvHandleCPU;
	//D3D12_GPU_DESCRIPTOR_HANDLE instancingSrvHandleGPU;
};

