#pragma once
#include"DirectXGame/engine/struct/Structs.h"
#include"LineCommon.h"
#include"DirectXGame/engine/Camera/Camera.h"
#include<d3d12.h>
#include<dxgi1_6.h>
#include<cstdint>
#include<wrl.h>

#include"DirectXGame/engine/Mesh/Mesh.h"

class LineCommon;



class LineDraw
{
public:
	void Initialize();

	void Update();

	void Draw3D(const Vector3& p1, const Vector3& p2, const Vector4& color);

	void SetCamera(Camera* camera) { this->camera = camera; }

	void SetMatrix(const Matrix4x4& mat) { mat_ = mat; };
	void SetTransform(const Transform tra) { transform = tra; };

	void DrawMeshLine(Mesh* mesh);

	void DrawCapselLine(const Vector3& torans ,const Capsule& capsure);

private:
	Camera* camera = nullptr;

	LineCommon* lineCommon_ = nullptr;

	std::unique_ptr<Mesh> mesh_;

	
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
	Matrix4x4 mat_;

	
	Microsoft::WRL::ComPtr < ID3D12Resource> materialResource;
	Material* materialData;

	

	TransfomationMatrix* transfomationMatrixData;
	Microsoft::WRL::ComPtr < ID3D12Resource> transformationMatrixResource;

	Transform cameraTransform;

public:
	Transform transform;
};


