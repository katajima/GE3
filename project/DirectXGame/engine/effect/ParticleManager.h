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
#include"DirectXGame/engine/struct/Material.h"
#include"DirectXGame/engine/base/DirectXCommon.h"
#include"DirectXGame/engine/base/SrvManager.h"
#include<random>
#include<numbers>
#include "DirectXGame/engine/base/Camera.h"
#include"DirectXGame/engine/3d/Object3dCommon.h"
#include "DirectXGame/engine/3d/Model.h"
#include"DirectXGame/engine/Line/Line.h"
#include"DirectXGame/engine/Line/LineCommon.h"




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

	// 
	enum class EmitType
	{
		kRandom,   // ランダム
		kConstant, // 定数
	};

#pragma region structs
	template<typename T>
	struct MaxMin
	{
		T min;
		T max;
	};
	struct Constant {
		Vector3 centar;
		Vector4 color;
		Vector3 size;
		Vector3 rotate;
		float lifeTime;
		Vector3 velocity;
	};


	// エミッター構造体
	struct Emiter
	{
		Vector3 center;

		// ランダム用
		MaxMin<Vector3> renge;     //出現位置 (Vector3の範囲)
		MaxMin<Vector4> color;     // 色 (Vector3の範囲)
		MaxMin<Vector3> size;        // 大きさ (floatの範囲)
		MaxMin<Vector3> rotate;      // 回転 (floatの範囲)
		MaxMin<float> lifeTime;    // 生存時間 (floatの範囲)
		MaxMin<Vector3> velocity;  // 速度 (Vector3の範囲)

		// 定数用
		Constant cons;



		float frequency_;		// < 発生頻度
		float frequencyTime_;	// < 頻度用時刻
		float count;
	};

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
		std::string name; // 名前
		MaterialData materialData;
		std::list<Particle> particle;
		uint32_t srvIndex;
		Microsoft::WRL::ComPtr<ID3D12Resource> resource;
		Microsoft::WRL::ComPtr<ID3D12Resource> materialResource;
		uint32_t instanceCount; // インスタンス数
		ParticleForGPU* instanceData; // インスタンシングデータを書き込むためのポインタ
		D3D12_GPU_DESCRIPTOR_HANDLE instancingSrvHandleGPU;
		D3D12_CPU_DESCRIPTOR_HANDLE instancingSrvHandleCPU;
		Model* model;
		Emiter emiter;
		std::vector < std::unique_ptr <LineDraw>> line_;
		bool usebillboard = true;
		bool isAlpha = false;
		bool isLine = true;
		EmitType emitType = EmitType::kRandom; // 
	};

	
	
#pragma endregion // 構造体

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

	void DrawCommonSetting();

	// パーティクルの発生
	void Emit(const std::string name, const Vector3& position, uint32_t count);

	std::unordered_map<std::string, ParticleGroup>& GetParticleGroups()
	{
		return particleGroups;
	}

	void CreateParticleGroup(const std::string name, const std::string textureFilePath, Model* model, Camera* camera);

	void SetCamera(Camera* camera) { this->camera = camera; }

	void DrawAABB(/*const EmiterAABB& emitAABB, *//*std::vector<std::unique_ptr<LineDraw>>& lineDraw_*/);

	void SetPos(const std::string name,const Vector3& position);
	
private:
	// ルートシグネチャの作成
	void CreateRootSignature();
	// グラフィックスパイプラインの作成
	void CreateGraphicsPipeline();

	// minmax
	void LimitMaxMin();

	// ランダム
	void RandParticle(const std::string name, const Vector3& position);

	// 定数
	void ConstantParticle(const std::string name, const Vector3& position);

private:
	static ParticleManager* instance;
	ParticleManager() = default;
	~ParticleManager() = default;
	ParticleManager(ParticleManager&) = delete;
	ParticleManager& operator=(ParticleManager&) = delete;

	DirectXCommon* dxCommon_ = nullptr;
	SrvManager* srvManager_ = nullptr;


	

	Microsoft::WRL::ComPtr < ID3D12Resource> directionalLightResource;
	//DirectionalLight* directionalLightData = nullptr;
	//マテリアル用のリソースを作る。今回はcolor1つ分のサイズを利用する
	Microsoft::WRL::ComPtr < ID3D12Resource> materialResource;

	//// Lightingを有効にする
	Material* materialData = nullptr;

	std::mt19937 randomEngine_;


	std::unordered_map<std::string, ParticleGroup> particleGroups;


	const uint32_t kNumMaxInstance = 1000;
	const float kDeltaTime = 1.0f / 60.0f;
	bool usebillboard = true;
	bool upData = true;
	bool upDataWind = false;
	uint32_t numInstance;

	AcceleraionField acceleraionField;
	
	Camera* camera = nullptr;

	
	//// バッファリソース
	Microsoft::WRL::ComPtr < ID3D12Resource> vertexResource;
	//// バッファリソース内のデータを指すポインタ
	VertexData* vertexData = nullptr;
	////バッファリソースの使い道を補足するバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	
	Transform transform;

	////ルートシグネチャデスク
	D3D12_ROOT_SIGNATURE_DESC descriptionSignature{};
	////ルートシグネチャ
	Microsoft::WRL::ComPtr < ID3D12RootSignature> rootSignature;
	//// グラフィックスパイプラインステート
	Microsoft::WRL::ComPtr < ID3D12PipelineState> graphicsPipelineState = nullptr;

	Model* model_;

	
	//

};



