#include "LightCommon.h"
#include "imgui.h"

LightCommon* LightCommon::instance = nullptr;

LightCommon* LightCommon::GetInstance()
{
	if (instance == nullptr) {
		instance = new LightCommon;
	}
	return instance;
}

void LightCommon::Initialize()
{
	//平行光源用のリソースを作る
	directionalLightResource = Object3dCommon::GetInstance()->GetDxCommon()->CreateBufferResource(sizeof(DirectionalLight));
	directionalLightData = nullptr;
	directionalLightResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData));
	//今回は赤を書き込んで見る //白
	*directionalLightData = DirectionalLight({ 1.0f,1.0f,1.0f,1.0f }, { 0.0f,-1.0f,0.0f }, 1.0f);
	directionalLightData->lig = 0.2f;
	directionalLightData->isLight = true;

	// 半球ライト
	directionalLightData->groundColor = { 0.7f,0.5f,0.3f };
	directionalLightData->skyColor = { 0.15f,0.7f,0.95f };
	directionalLightData->groundColor = { 1.0f,1.0f,1.0f };
	directionalLightData->skyColor = { 1.0f,1.0f,1.0f };
	directionalLightData->groundNormal = { 0.0f,1.0f,0.0f };






	pointLightResource = Object3dCommon::GetInstance()->GetDxCommon()->CreateBufferResource((sizeof(PointLight) * kNumMaxInstance));
	pointLightResource->Map(0, nullptr, reinterpret_cast<void**>(&pointLightData));


	//今回は赤を書き込んで見る //白
	pointLightData[0] = PointLight({1.0f,1.0f,1.0f,1.0f}, {0.0f,-1.0f,-10.0f});
	pointLightData[0].radius = 10.0f;
	pointLightData[0].intensity = 1.0f;
	pointLightData[0].lig = 0.2f;
	pointLightData[0].isLight = true;
	pointLightData[1] = PointLight({0.0f,1.0f,1.0f,1.0f}, {0.0f,-1.0f,10.0f});
	pointLightData[1].radius = 10.0f;
	pointLightData[1].intensity = 5.0f;
	pointLightData[1].isLight = true;
	pointLightData[1].lig = 0.2f;

	//平行光源用のリソースを作る
	spotLightResource = Object3dCommon::GetInstance()->GetDxCommon()->CreateBufferResource(sizeof(SpotLight) * kNumMaxInstance);
	spotLightResource->Map(0, nullptr, reinterpret_cast<void**>(&spotLightData));
	//今回は赤を書き込んで見る //白

	for (int i = 0; i < spotMax; i++) {
		spotLightData[i] = SpotLight({1.0f,1.0f,1.0f,1.0f}, {2.0f,1.25f,0.0f}, 10.0f, Normalize({-1.0f,-1.0f,0.0f}), 10.0f, 2.0f, std::cos(std::numbers::pi_v<float> / 3.0f), 1.0f);
		spotLightData[i].position = Vector3(10 * float(i), 0, 10);
		spotLightData[i].isLight = false;
	}

	

}

void LightCommon::Finalize()
{
	delete instance;
	instance = nullptr;
}

void LightCommon::DrawLight()
{
	////------平行光源用------////
	Object3dCommon::GetInstance()->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource->GetGPUVirtualAddress());

	////------ポイントライト用------////
	Object3dCommon::GetInstance()->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(5, pointLightResource->GetGPUVirtualAddress());

	////------スポットライト用------////
	Object3dCommon::GetInstance()->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(6, spotLightResource->GetGPUVirtualAddress());

}

void LightCommon::SetLineCamera(Camera* camera)
{
	for (int i = 0; i < pointLightLines_.size(); i++) {
		//pointLightLines_[i]->SetCamera(camera);
	}
	for (int i = 0; i < spotLightLines_.size(); i++) {
		spotLightLines_[i]->SetCamera(camera);
	}
}

void LightCommon::Update()
{
#ifdef _DEBUG
	ImGui::Begin("engine");

	if (ImGui::CollapsingHeader("Light")) {

		//if (ImGui::CollapsingHeader("DirectionalLight")) {
			if (ImGui::BeginTabBar("directionalLightData"))
			{
				if (ImGui::BeginTabItem("directionalLightData"))
				{
						bool is = directionalLightData->isLight;
						ImGui::Checkbox("isLighting", &is);
						directionalLightData->isLight = is;
						ImGui::DragFloat3("direction", &directionalLightData->direction.x, 0.1f);
						directionalLightData->direction = Normalize(directionalLightData->direction);
						ImGui::DragFloat("intensity", &directionalLightData->intensity, 0.1f);
						if (0 >= directionalLightData->intensity)
							directionalLightData->intensity = 0;
						ImGui::DragFloat("lig", &directionalLightData->lig, 0.1f);
						ImGui::ColorEdit3("groundColor", &directionalLightData->groundColor.x);
						ImGui::ColorEdit3("skyColor", &directionalLightData->skyColor.x);

						ImGui::ColorEdit4("color", &directionalLightData->color.x);
						ImGui::EndTabItem();
					
				}
			}
		//}
		ImGui::EndTabBar();
		if (ImGui::BeginTabBar("pointLightData"))
		{
			for (int i = 0; i < 2; i++) {
				std::string str = "light" + std::to_string(i);
				if (ImGui::BeginTabItem(str.c_str())) {
					bool is = pointLightData[i].isLight;
					ImGui::Checkbox("isLighting", &is);
					pointLightData[i].isLight = is;
					ImGui::DragFloat3("position", &pointLightData[i].position.x, 0.1f);
					ImGui::DragFloat("intensity", &pointLightData[i].intensity, 0.1f);
					if (0 >= pointLightData[i].intensity)
						pointLightData[i].intensity = 0;
					ImGui::DragFloat("decay", &pointLightData[i].decay, 0.1f);
					ImGui::DragFloat("radius", &pointLightData[i].radius, 0.1f);
					ImGui::DragFloat("lig", &pointLightData[i].lig, 0.1f);

					ImGui::ColorEdit4("color", &pointLightData[i].color.x);
					ImGui::EndTabItem();
				}
			}
		}
		ImGui::EndTabBar();
		if (ImGui::BeginTabBar("spotLightData"))
		{
			for (int i = 0; i < spotMax; i++) {
				std::string str = "light" + std::to_string(i);
				if (ImGui::BeginTabItem(str.c_str())) {
					bool is = spotLightData[i].isLight;
					ImGui::Checkbox("isLighting", &is);
					spotLightData[i].isLight = is;

					ImGui::DragFloat3("position", &spotLightData[i].position.x, 0.1f);
					
					ImGui::DragFloat3("direction", &spotLightData[i].direction.x, 0.1f);
					spotLightData[i].direction = Normalize(spotLightData[i].direction);
					
					ImGui::DragFloat("intensity", &spotLightData[i].intensity, 0.1f);
					if (0 >= spotLightData[i].intensity)
						spotLightData[i].intensity = 0;
					ImGui::DragFloat("distance", &spotLightData[i].distance, 0.1f);
					
					ImGui::DragFloat("decay", &spotLightData[i].decay, 0.1f);
					ImGui::DragFloat("cosFalloffStart", &spotLightData[i].cosFalloffStart, 0.1f);
					ImGui::DragFloat("cosAngle", &spotLightData[i].cosAngle, 0.01f);

					ImGui::ColorEdit4("color", &spotLightData[i].color.x);
					ImGui::EndTabItem();
				}
			}
		}
		ImGui::EndTabBar();
	}
	ImGui::End();

	for (int i = 0; i < spotLightLines_.size(); i++) {
		spotLightLines_[i]->Update();
	}
	for (int i = 0; i < pointLightLines_.size(); i++) {
		//pointLightLines_[i]->Update();
	}
#endif
}


void LightCommon::DrawLightLine() {
	// PointLight のライン描画
	//DrawLineWithLines(pointLightData->position, Vector3{ 1, 1, 1 }, pointLightLines_);

	// SpotLight のライン描画
	DrawLineWithLines(spotLightData->position, Vector3{ 1, 1, 1 }, spotLightLines_);
}

void LightCommon::DrawLineWithLines(const Vector3& center, const Vector3& extent,
	std::vector<std::unique_ptr<LineDraw>>& lines) {
	// 必要なライン数を確認
	size_t requiredLines = 24 / 2;
	if (lines.size() < requiredLines) {
		// 不足分を追加生成
		for (size_t i = lines.size(); i < requiredLines; ++i) {
			auto line = std::make_unique<LineDraw>();
			line->Initialize(LineCommon::GetInstance());
			lines.push_back(std::move(line));
		}
	}

	// 各頂点を計算
	Vector3 v0 = { center.x - extent.x, center.y - extent.y, center.z - extent.z }; // 左下前
	Vector3 v1 = { center.x + extent.x, center.y - extent.y, center.z - extent.z }; // 右下前
	Vector3 v2 = { center.x + extent.x, center.y + extent.y, center.z - extent.z }; // 右上前
	Vector3 v3 = { center.x - extent.x, center.y + extent.y, center.z - extent.z }; // 左上前
	Vector3 v4 = { center.x - extent.x, center.y - extent.y, center.z + extent.z }; // 左下後
	Vector3 v5 = { center.x + extent.x, center.y - extent.y, center.z + extent.z }; // 右下後
	Vector3 v6 = { center.x + extent.x, center.y + extent.y, center.z + extent.z }; // 右上後
	Vector3 v7 = { center.x - extent.x, center.y + extent.y, center.z + extent.z }; // 左上後

	// ライン描画用の配列
	Vector3 positions[24] = {
		// 前面
		v0, v1, v1, v2, v2, v3, v3, v0,
		// 背面
		v4, v5, v5, v6, v6, v7, v7, v4,
		// 左側面
		v0, v4, v3, v7,
		// 右側面
		v1, v5, v2, v6
	};

	// 描画
	Vector4 color = { 1, 1, 0, 1 }; // 白色
	for (size_t i = 0; i < requiredLines; ++i) {
		lines[i]->Draw3D(positions[i * 2], positions[i * 2 + 1], color);
	}
}
