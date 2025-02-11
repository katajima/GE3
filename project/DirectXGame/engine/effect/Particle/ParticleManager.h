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
#include<memory>
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
#include "DirectXGame/engine/Camera/Camera.h"
#include"DirectXGame/engine/3d/Object3dCommon.h"
#include "DirectXGame/engine/3d/Model.h"
#include"DirectXGame/engine/Line/Line.h"
#include"DirectXGame/engine/Line/LineCommon.h"
#include "DirectXGame/engine/Material/Material.h"
#include "DirectXGame/engine/Primitive/Primitive.h"
#include "../../3d/Object3d.h"


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
	enum class RasterizerType
	{
		MODE_SOLID_BACK,
		MODE_SOLID_NONE,
	};

	enum class BlendType
	{
		MODE_ADD,
		MODE_SUBTRACT,
		MODE_MUlLIPLY,
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
		int count;

		MaxMin<Vector3> renge;
		MaxMin<Vector3> velocityRenge;  // 速度 (Vector3の範囲)
	};


	// エミッター構造体
	struct Emiter
	{
		//Vector3 center;
		//Matrix4x4 mat;
		// ランダム用
		MaxMin<Vector3> renge;     //出現位置 (Vector3の範囲)
		MaxMin<Vector4> color;     // 色 (Vector3の範囲)
		MaxMin<Vector3> size;        // 大きさ (floatの範囲)
		MaxMin<Vector3> rotate;      // 回転 (floatの範囲)
		MaxMin<Vector3> rotateVelocity;// 回転 (floatの範囲)
		MaxMin<float> lifeTime;    // 生存時間 (floatの範囲)
		MaxMin<Vector3> velocity;  // 速度 (Vector3の範囲)


		WorldTransform worldtransform;
		bool isEmit = false;

		// 定数用
		Constant cons;



		
		float count;
	};

	struct Particle
	{	
		Transform transform;
		Vector3 velocity;
		Vector3 acceleration;
		Vector4 color;
		float lifeTime;
		float currentTime;

		Transform strtTransform;
		Vector3 rotateVelocity;
	};
	struct AcceleraionField {
		Vector3 acceleration;
		AABB area;
	};

	struct ParticleGroup
	{
		std::string name; // 名前
		std::unique_ptr<Material> material = nullptr;
		std::list<Particle> particle;
		bool flag;
		uint32_t srvIndex;
		Microsoft::WRL::ComPtr<ID3D12Resource> resource;
		uint32_t instanceCount; // インスタンス数
		ParticleForGPU* instanceData; // インスタンシングデータを書き込むためのポインタ
		D3D12_GPU_DESCRIPTOR_HANDLE instancingSrvHandleGPU;
		D3D12_CPU_DESCRIPTOR_HANDLE instancingSrvHandleCPU;
		//Model* model;
		Mesh* mesh;
		Emiter emiter;
		std::vector < std::unique_ptr <LineDraw>> line_;
		bool usebillboard = true;
		bool isAlpha = false;
		bool isLine = true;
		bool isGravity = false;
		bool isLifeTimeScale_ = false;
		bool isRotateVelocity = false;
		bool isBounce = false;
		EmitType emitType = EmitType::kRandom; 
		RasterizerType rasteType;
		BlendType blendType;
	};

	
	
#pragma endregion // 構造体

public:


	// シングルトンインスタンス
	static ParticleManager* GetInstance();

	// 初期化
	void Initialize(DirectXCommon* dxCommon);
	// 更新
	void Update();
	// 描画
	void Draw();
	// 終了
	void Finalize();

	void DrawCommonSetting(RasterizerType rasteType,BlendType blendType);

	// パーティクルの発生
	void Emit(const std::string name,const std::string emitName, EmitType type);
	void Emit(const std::string name,const std::string emitName, const Constant& cons);

	std::unordered_map<std::string, ParticleGroup>& GetParticleGroups()
	{
		return particleGroups;
	}
	ParticleGroup& GetParticleGroups(const std::string name)
	{
		return particleGroups[name];
	}



	void CreateParticleGroup(const std::string name, const std::string textureFilePath, Model* model,bool flag = false, RasterizerType rasteType = RasterizerType::MODE_SOLID_BACK, BlendType blendType = BlendType::MODE_ADD);

	void CreateParticleGroup(const std::string name, const std::string textureFilePath, Primitive* primitive, bool flag = false, RasterizerType rasteType = RasterizerType::MODE_SOLID_BACK, BlendType blendType = BlendType::MODE_ADD);

	void SetCamera(Camera* camera) { this->camera_ = camera; }

	void DrawAABB();

	void SetPos(const std::string name,const Vector3& position);
	
	
	void SetObject(const std::string name, WorldTransform& obj);
private: //セット系

	void SetPos();



private:
	// ルートシグネチャの作成
	void CreateRootSignature();
	// グラフィックスパイプラインの作成
	void CreateGraphicsPipeline();

	// minmax
	void LimitMaxMin();

	// ランダム
	void RandParticle(const std::string name);

	// 定数
	void ConstantParticle(const std::string name, const Constant& cons);
	// 定数
	void ConstantParticle2(const std::string name, const Constant& cons);


private:
	void GraphicsPipelineState(Microsoft::WRL::ComPtr < ID3D12RootSignature>& rootSignature, Microsoft::WRL::ComPtr < ID3D12PipelineState>& graphicsPipelineState
		, D3D12_RASTERIZER_DESC rasterizerDesc, D3D12_BLEND_DESC blendDesc);

	void BlendAdd();

	void BlendSubtract();

	void BlendMuliply();

private:
	static ParticleManager* instance;
	ParticleManager() = default;
	~ParticleManager() = default;
	ParticleManager(ParticleManager&) = delete;
	ParticleManager& operator=(ParticleManager&) = delete;

	DirectXCommon* dxCommon_ = nullptr;
	SrvManager* srvManager_ = nullptr;


	

	

	std::mt19937 randomEngine_;


	std::unordered_map<std::string, ParticleGroup> particleGroups;


	const uint32_t kNumMaxInstance = 12000;
	//float kDeltaTime;
	bool usebillboard = true;
	bool upData = true;
	bool upDataWind = false;
	uint32_t numInstance;

	AcceleraionField acceleraionField;
	
	Camera* camera_ = nullptr;

	
	Transform transform;


	const float kGravitationalAcceleration = 9.8f;
	////ルートシグネチャデスク
	D3D12_ROOT_SIGNATURE_DESC descriptionSignature{};
	////ルートシグネチャ
	Microsoft::WRL::ComPtr < ID3D12RootSignature> rootSignature;
	//// グラフィックスパイプラインステート
	Microsoft::WRL::ComPtr < ID3D12PipelineState> graphicsPipelineState[6];

	

	D3D12_BLEND_DESC blendDesc{};
	D3D12_RASTERIZER_DESC rasterizerDesc{};
};



