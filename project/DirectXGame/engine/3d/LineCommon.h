#pragma once
#include <array>
#include <cstdint>
#include <d3d12.h>
#include <memory>
#include <string>
#include <wrl.h>
#include "DirectXGame/engine/struct/Structs.h"
#include "DirectXGame/engine/base/Camera.h"
#include "DirectXGame/engine/base/DirectXCommon.h"

class LineCommon
{
public:

public:
	static LineCommon* GetInstance();

	// 終了
	void Finalize();

	// 初期化
	void Initialize(DirectXCommon* dxCommon);

	DirectXCommon* GetDxCommon() const { return dxCommon_; }

	void DrawCommonSetting();

	void SetDefaltCamera(const Camera* camera) { came = camera; }

	//Camera* GetDefaltCamera() const { return defaultCamera; }

	//void DrawLine3d(const Vector3& p1, const Vector3& p2, const Vector4& color, Camera* camera);

private:
	// ルートシグネチャの作成
	void CreateRootSignature();
	// グラフィックスパイプラインの作成
	void CreateGraphicsPipeline();


private:
	static LineCommon* instance;

	//Camera* defaultCamera = nullptr;

	const Camera* came = nullptr;

	// ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	// パイプラインステートオブジェクト
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState;

	DirectXCommon* dxCommon_;


	// 頂点データ
	struct VertexData {

		Vector4 position;
	};
	//マテリアルデータ
	struct Material {
		Vector4 color;
	};
	// 座標変換行列データ
	struct TransfomationMatrix
	{
		Matrix4x4 WVP;
		Matrix4x4 World;
	};

	// バッファリソース
	Microsoft::WRL::ComPtr < ID3D12Resource> vertexResource;
	Microsoft::WRL::ComPtr < ID3D12Resource> indexResource;
	// バッファリソース内のデータを指すポインタ
	VertexData* vertexData = nullptr;

	//バッファリソースの使い道を補足するバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW indexBufferView;
	Microsoft::WRL::ComPtr < ID3D12Resource> materialResource;
	Material* materialData;



	TransfomationMatrix* transfomationMatrixData;
	Microsoft::WRL::ComPtr < ID3D12Resource> transformationMatrixResource;

	Transform cameraTransform;

public:
	Transform transform;

};

