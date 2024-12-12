#pragma once
#include "DirectXGame/engine/struct/Light.h"
#include<d3d12.h>
#include<dxgi1_6.h>
#include<cstdint>
#include<wrl.h>
#include<string>
#include<vector>
#include<format>
#include"Object3dCommon.h"
#include"DirectXGame/engine/Line/Line.h"
#include"DirectXGame/engine/Line/LineCommon.h"

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
private:
	
	void DrawLineWithLines(const Vector3& center, const Vector3& extent,
		std::vector<std::unique_ptr<LineDraw>>& lines);

private:
	const uint32_t kNumMaxInstance = 1000;


	Microsoft::WRL::ComPtr < ID3D12Resource> directionalLightResource;
	DirectionalLight* directionalLightData = nullptr;

	/*struct PointLights
	{
		Microsoft::WRL::ComPtr < ID3D12Resource> pointLightResource;
		PointLight* pointLightData = nullptr;
	};
	std::vector < std::unique_ptr<PointLights>> pointLights;*/


	Microsoft::WRL::ComPtr < ID3D12Resource> pointLightResource;
	PointLight* pointLightData = nullptr;
	//std::vector<PointLight> pointLightData;
	const int pointMax = 3;


	Microsoft::WRL::ComPtr < ID3D12Resource> spotLightResource;
	SpotLight* spotLightData = nullptr;
	const int spotMax = 3;


	std::vector<std::unique_ptr<LineDraw>> pointLightLines_;
	std::vector<std::unique_ptr<LineDraw>> spotLightLines_; 
	bool isLine = false;

};


