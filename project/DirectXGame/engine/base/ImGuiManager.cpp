#include"ImGuiManager.h"
#include "DirectXCommon.h"
#include "WinApp.h"
#include <iostream>//用いるヘッダファイルが変わります。

// ギズモの操作モード
static ImGuizmo::OPERATION currentOperation = ImGuizmo::TRANSLATE; // 初期値は移動


ImGuiManager* ImGuiManager::GetInstance() {
	static ImGuiManager instance;
	return &instance;
}

void ImGuiManager::Initialize(DirectXCommon* dxCommon)
{
#ifdef _DEBUG
	HRESULT result;

	dxCommon_ = dxCommon;

	// デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.NumDescriptors = 1;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	// デスクリプタヒープ生成
	result = dxCommon_->GetDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&srvHeap_));
	assert(SUCCEEDED(result));


	// コンテキストの生成
	ImGui::CreateContext();
	// Imguiのスタイルを設定 黒
	ImGui::StyleColorsDark();

	// プラットフォームとレンダラーのバックエンドを設定する
	ImGui_ImplWin32_Init(WinApp::GetHwnd());
	ImGui_ImplDX12_Init(
		dxCommon_->GetDevice().Get(), static_cast<int>(dxCommon_->GetBackBufferCount()),
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, srvHeap_.Get(),
		srvHeap_->GetCPUDescriptorHandleForHeapStart(),
		srvHeap_->GetGPUDescriptorHandleForHeapStart());
#endif // _DEBUG

}

void ImGuiManager::Finalize()
{
#ifdef _DEBUG
	// 後始末
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	// デスクリプタヒープを解放
	srvHeap_.Reset();
#endif // _DEBUG
}

void ImGuiManager::Begin()
{
#ifdef _DEBUG
	// ImGuiフレーム開始
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGuizmo::BeginFrame();
#endif // _DEBUG
}

void ImGuiManager::End()
{
#ifdef _DEBUG
	// 描画前準備
	ImGui::Render();
#endif // _DEBUG
}

void ImGuiManager::Draw()
{
#ifdef _DEBUG
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList().Get();

	// デスクリプタヒープの配列をセットするコマンド
	ID3D12DescriptorHeap* ppHeaps[] = { srvHeap_.Get() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// 描画コマンドを発行
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);
#endif // _DEBUG
}


void ImGuiManager::RenderGizmo2(Object3d& obj, const Camera& camera, const char* name) {
	

	if (ImGui::BeginTabBar("Gizmo"))
	{
		if (ImGui::BeginTabItem(name))
		{



			static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::ROTATE);
			static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);

			// ギズモを操作
			bool isManipulated = ImGuizmo::Manipulate(
				&camera.GetViewMatrix().m[0][0],
				&camera.GetProjectionMatrix().m[0][0],
				mCurrentGizmoOperation,
				mCurrentGizmoMode,
				&obj.worldtransform_.worldMat_.m[0][0]);


			if (isManipulated) {
				std::cout << "Manipulate succeeded" << std::endl; // オブジェクトの行列を取得 
				float translation[3], rotation[3], scale[3];
				ImGuizmo::DecomposeMatrixToComponents(&obj.worldtransform_.worldMat_.m[0][0], translation, rotation, scale);
				obj.worldtransform_.translate_ = Vector3(translation[0], translation[1], translation[2]);
				obj.worldtransform_.rotate_ = Vector3(DegreesToRadians(rotation[0]), DegreesToRadians(rotation[1]), DegreesToRadians(rotation[2]));
				obj.worldtransform_.scale_ = Vector3(scale[0], scale[1], scale[2]);
				std::cout << "Translation: " << translation[0] << ", " << translation[1] << ", " << translation[2] << std::endl;
				std::cout << "Rotation: " << rotation[0] << ", " << rotation[1] << ", " << rotation[2] << std::endl;
				std::cout << "Scale: " << scale[0] << ", " << scale[1] << ", " << scale[2] << std::endl;
			}
			else {
				std::cout << "Manipulate failed" << std::endl;
			}
			if (ImGuizmo::IsUsing()) {
				std::cout << "ImGuizmo is using" << std::endl;
			}
			else {
				std::cout << "ImGuizmo is not using" << std::endl;
			}

			if (ImGui::Button("TRANSLATE")) mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
			if (ImGui::Button("ROTATE")) mCurrentGizmoOperation = ImGuizmo::ROTATE;
			if (ImGui::Button("SCALE")) mCurrentGizmoOperation = ImGuizmo::SCALE;

			if (Input::GetInstance()->IsTriggerKey(DIK_G)) {
				mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
			}
			if (Input::GetInstance()->IsTriggerKey(DIK_R)) {
				mCurrentGizmoOperation = ImGuizmo::ROTATE;
			}
			if (Input::GetInstance()->IsTriggerKey(DIK_S)) {
				mCurrentGizmoOperation = ImGuizmo::SCALE;
			}

			if (mCurrentGizmoOperation != ImGuizmo::SCALE) {
				if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
					mCurrentGizmoMode = ImGuizmo::LOCAL;
				ImGui::SameLine();
				if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
					mCurrentGizmoMode = ImGuizmo::WORLD;
			}

			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			ImGuizmo::SetRect(0, 0, ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y);
			ImGuizmo::Enable(true);




			bool over = ImGuizmo::IsOver();
			ImGui::Checkbox("IsOver", &over);
			bool isUse = ImGuizmo::IsUsing();
			ImGui::Checkbox("IsUsing", &isUse);

			if (ImGuizmo::IsUsing()) {
				std::cout << "ImGuizmo is using" << std::endl;
				// オブジェクトの行列を取得
				float translation[3], rotation[3], scale[3];
				ImGuizmo::DecomposeMatrixToComponents(&obj.worldtransform_.worldMat_.m[0][0], translation, rotation, scale);
				Vector3 matrixTranslation = { translation[0], translation[1], translation[2] };
				Vector3 matrixRotation = { rotation[0], rotation[1], rotation[2] };
				Vector3 matrixScale = { scale[0], scale[1], scale[2] };
				ImGui::InputFloat3("Tr", &matrixTranslation.x);
				ImGui::InputFloat3("Rt", &matrixRotation.x);
				ImGui::InputFloat3("Sc", &matrixScale.x);
				ImGuizmo::RecomposeMatrixFromComponents(translation, rotation, scale, &obj.worldtransform_.worldMat_.m[0][0]);
			}
			else {
				std::cout << "ImGuizmo is not using" << std::endl;
			}

			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
}

void ImGuiManager::SetCustomColorScheme()
{
	ImGuiStyle& style = ImGui::GetStyle();

	// 背景色
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);  // ダークグレー

	// ボタンの色
	style.Colors[ImGuiCol_Button] = ImVec4(0.2f, 0.7f, 0.2f, 1.0f);  // 緑
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.4f, 1.0f, 0.4f, 1.0f);  // 明るい緑
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.1f, 0.5f, 0.1f, 1.0f);  // ダーク緑

	// ヘッダーの色
	style.Colors[ImGuiCol_Header] = ImVec4(0.2f, 0.3f, 0.8f, 1.0f);  // 青
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.4f, 0.5f, 1.0f, 1.0f);  // 明るい青
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.1f, 0.2f, 0.6f, 1.0f);  // ダーク青
}





