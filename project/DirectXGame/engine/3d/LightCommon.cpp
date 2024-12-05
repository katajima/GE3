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


	pointLightResource = Object3dCommon::GetInstance()->GetDxCommon()->CreateBufferResource(sizeof(PointLight));
	pointLightData = nullptr;
	pointLightResource->Map(0, nullptr, reinterpret_cast<void**>(&pointLightData));


	//今回は赤を書き込んで見る //白
	*pointLightData = PointLight({ 1.0f,1.0f,1.0f,1.0f }, { 0.0f,-1.0f,-10.0f }, 100.0f);
	pointLightData->radius = 10.0f;
	pointLightData->intensity = 1.0f;



	//平行光源用のリソースを作る
	spotLightResource = Object3dCommon::GetInstance()->GetDxCommon()->CreateBufferResource(sizeof(SpotLight));
	spotLightResource->Map(0, nullptr, reinterpret_cast<void**>(&spotLightData));
	//今回は赤を書き込んで見る //白
	*spotLightData = SpotLight({ 1.0f,1.0f,1.0f,1.0f }, { 2.0f,1.25f,0.0f }, 700.0f, Normalize({ -1.0f,-1.0f,0.0f }), 4.0f, 2.0f, std::cos(std::numbers::pi_v<float> / 3.0f), 1.0f);
	spotLightData->position = Vector3(10,10,10);

	
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
		pointLightLines_[i]->SetCamera(camera);
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

		if (ImGui::BeginTabBar("Light"))
		{
			if (ImGui::BeginTabItem("directionalLightData"))
			{
				ImGui::DragFloat3("direction", &directionalLightData->direction.x, 0.1f);
				directionalLightData->direction = Normalize(directionalLightData->direction);
				ImGui::DragFloat("intensity", &directionalLightData->intensity, 0.1f);
				if (0 >= directionalLightData->intensity)
					directionalLightData->intensity = 0;
				ImGui::ColorEdit4("color", &directionalLightData->color.x);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("pointLightData")) {
				ImGui::DragFloat3("position", &pointLightData->position.x, 0.1f);
				ImGui::DragFloat("intensity", &pointLightData->intensity, 0.1f);
				if (0 >= pointLightData->intensity)
					pointLightData->intensity = 0;
				ImGui::DragFloat("direction", &pointLightData->decay, 0.1f);
				ImGui::DragFloat("radius", &pointLightData->radius, 0.1f);

				ImGui::ColorEdit4("color", &pointLightData->color.x);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("spotLightData")) {
				ImGui::DragFloat3("position", &spotLightData->position.x, 0.1f);
				ImGui::DragFloat3("direction", &spotLightData->direction.x, 0.1f);
				spotLightData->direction = Normalize(spotLightData->direction);
				ImGui::DragFloat("intensity", &spotLightData->intensity, 0.1f);
				if (0 >= spotLightData->intensity)
					spotLightData->intensity = 0;
				ImGui::DragFloat("distance", &spotLightData->distance, 0.1f);
				ImGui::DragFloat("decay", &spotLightData->decay, 0.1f);
				ImGui::DragFloat("cosFalloffStart", &spotLightData->cosFalloffStart, 0.1f);
				ImGui::DragFloat("cosAngle", &spotLightData->cosAngle, 0.1f);
				
				ImGui::ColorEdit4("color", &spotLightData->color.x);
				ImGui::EndTabItem();
			}
		}
		ImGui::EndTabBar();
	}
	ImGui::End();

	for (int i = 0; i < spotLightLines_.size(); i++) {
		spotLightLines_[i]->Update();
	}
	for (int i = 0; i < pointLightLines_.size(); i++) {
		pointLightLines_[i]->Update();
	}
#endif
}


void LightCommon::DrawLightLine() {
	// PointLight のライン描画
	DrawLineWithLines(pointLightData->position, Vector3{ 1, 1, 1 }, pointLightLines_);

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
