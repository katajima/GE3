#pragma once
#include "DirectXGame/engine/struct/Light.h"
#include<d3d12.h>
#include<dxgi1_6.h>
#include<cstdint>
#include<wrl.h>
#include<string>
#include<vector>
#include<format>
#include"DirectXGame/engine/3d/Object3dCommon.h"
#include"DirectXGame/engine/Line/Line.h"
#include"DirectXGame/engine/Line/LineCommon.h"

#include"DirectXGame/application/GlobalVariables/GlobalVariables.h"

class LightCommon
{
public:
	static LightCommon* instance;

	static LightCommon* GetInstance();

	void Initialize();

	void Finalize();

	void DrawLight();

	void DrawLightLine();

	void SetLineCamera(Camera* camera);

	void Update();

	DirectionalLight GetDirectionalLight(int index) {return directionalLightData[index];}
	PointLight GetPointLight(int index) {return pointLightData[index];}

	static void SetRootParameter(D3D12_ROOT_PARAMETER& parameter, int registr);

private:
	void ApplyGlobalVariables();

	void DrawLineWithLines(const Vector3& center, const Vector3& extent,
		std::vector<std::unique_ptr<LineDraw>>& lines);

private:
	const uint32_t kNumMaxInstance = 1000;


	Microsoft::WRL::ComPtr < ID3D12Resource> directionalLightResource;
	DirectionalLight* directionalLightData = nullptr;

	

	Microsoft::WRL::ComPtr < ID3D12Resource> pointLightResource;
	PointLight* pointLightData = nullptr;
	const int pointMax = 3;


	Microsoft::WRL::ComPtr < ID3D12Resource> spotLightResource;
	SpotLight* spotLightData = nullptr;
	const int spotMax = 3;


	std::vector<std::unique_ptr<LineDraw>> pointLightLines_;
	std::vector<std::unique_ptr<LineDraw>> spotLightLines_; 
	bool isLine = false;

};


