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



void ImGuiManager::RenderGizmo(Object3d& obj/*Matrix4x4& objectMatrix*/, const Matrix4x4& viewMatrix, const Matrix4x4& projectionMatrix)
{
	static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::ROTATE);
	static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);

	ImGui::Begin("Gizmo");
	if (ImGui::Button("TRANSLATE"))
		mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	if (ImGui::Button("ROTATE"))
		mCurrentGizmoOperation = ImGuizmo::ROTATE;
	if (ImGui::Button("SCALE"))
		mCurrentGizmoOperation = ImGuizmo::SCALE;

	ImGui::Checkbox("X", &flag.x);
	ImGui::Checkbox("Y", &flag.y);
	ImGui::Checkbox("Z", &flag.z);

	ImGui::End();


	if (mCurrentGizmoOperation != ImGuizmo::SCALE)
	{
		if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
			mCurrentGizmoMode = ImGuizmo::LOCAL;
		ImGui::SameLine();
		if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
			mCurrentGizmoMode = ImGuizmo::WORLD;
	}
	switch (mCurrentGizmoOperation)
	{
	case ImGuizmo::TRANSLATE:
		break;
	case ImGuizmo::ROTATE:
		break;
	case ImGuizmo::SCALE:
		break;
	}


	ImGuizmo::BeginFrame();

	// ImGuizmoをDear ImGuiのウィンドウ内で使用するための設定
	ImGuizmo::SetOrthographic(false); // 透視投影（Perspective）の場合はfalse
	ImGuizmo::SetDrawlist();

	// ビューポートの設定（必要に応じて調整）
	ImGuizmo::SetRect(0, 0, ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y);


#pragma region 操作

	// 動かす軸を出す
	if (Input::GetInstance()->PushKey(DIK_X)) {
		flag.x = true;
		flag.y = false;
		flag.z = false;
	}
	if (Input::GetInstance()->PushKey(DIK_Y)) {
		flag.y = true;
		flag.x = false;
		flag.z = false;
	}
	if (Input::GetInstance()->PushKey(DIK_Z)) {
		flag.z = true;
		flag.x = false;
		flag.y = false;
	}



	// 操作が行われた場合、objectMatrixが更新されます 
	Vector3 translation(0.0f, 0.0f, 0.0f); // 移動ベクトルの例 
	Vector3 scale(1.0f, 1.0f, 1.0f); // スケールベクトルの例 
	Vector3 rotate(0.0f, 0.0f, 0.0f); // スケールベクトルの例 
	float rotationAngle = DegreesToRadians(1.0f); // 回転角度の例（度数法） 
	switch (mCurrentGizmoOperation) {
	case ImGuizmo::TRANSLATE:

		if (Input::GetInstance()->PushKey(DIK_UP)) {
			if (flag.x)
				translation = Vector3(1.0f, 0.0f, 0.0f);
			if (flag.y)
				translation = Vector3(0.0f, 1.0f, 0.0f);
			if (flag.z)
				translation = Vector3(0.0f, 0.0f, 1.0f);

		}
		if (Input::GetInstance()->PushKey(DIK_DOWN)) {
			if (flag.x)
				translation = Vector3(-1.0f, 0.0f, 0.0f);
			if (flag.y)
				translation = Vector3(0.0f, -1.0f, 0.0f);
			if (flag.z)
				translation = Vector3(0.0f, 0.0f, -1.0f);
		}
		obj.transform.translate = Add(obj.transform.translate, translation);
		break;
	case ImGuizmo::ROTATE:
		if (Input::GetInstance()->PushKey(DIK_UP)) {
			if (flag.x)
				rotate = Vector3(rotationAngle, 0.0f, 0.0f);
			if (flag.y)
				rotate = Vector3(0.0f, rotationAngle, 0.0f);
			if (flag.z)
				rotate = Vector3(0.0f, 0.0f, rotationAngle);
		}
		if (Input::GetInstance()->PushKey(DIK_DOWN)) {
			if (flag.x)
				rotate = Vector3(-rotationAngle, 0.0f, 0.0f);
			if (flag.y)
				rotate = Vector3(0.0f, -rotationAngle, 0.0f);
			if (flag.z)
				rotate = Vector3(0.0f, 0.0f, -rotationAngle);
		}
		obj.transform.rotate = Add(obj.transform.rotate, rotate);
		break;
	case ImGuizmo::SCALE:
		if (Input::GetInstance()->PushKey(DIK_UP)) {
			if (flag.x)
				scale = Vector3(0.1f, 0.0f, 0.0f);
			if (flag.y)
				scale = Vector3(0.0f, 0.1f, 0.0f);
			if (flag.z)
				scale = Vector3(0.0f, 0.0f, 0.1f);

		}
		if (Input::GetInstance()->PushKey(DIK_DOWN)) {
			if (flag.x)
				scale = Vector3(-0.1f, 0.0f, 0.0f);
			if (flag.y)
				scale = Vector3(0.0f, -0.1f, 0.0f);
			if (flag.z)
				scale = Vector3(0.0f, 0.0f, -0.1f);
		}
		obj.transform.scale = Add(obj.transform.scale, scale);
		break;
	}

#pragma endregion


	// ギズモを操作
	bool isManipulated = ImGuizmo::Manipulate(
		GetMatrixPointer(viewMatrix),        // カメラのビュー行列
		GetMatrixPointer(projectionMatrix),  // カメラのプロジェクション行列
		mCurrentGizmoOperation,              // 操作タイプ（移動、回転、スケール）
		mCurrentGizmoMode,                   // 座標系（ローカル、ワールド）
		GetMatrix(obj.mat_)       // 操作対象のオブジェクトの行列
	);

}


void ImGuiManager::RenderGizmo2(Object3d& obj, const Camera& camera) {
	ImGui::Begin("Gizmo");

	static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::ROTATE);
	static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);

	// ギズモを操作
	bool isManipulated = ImGuizmo::Manipulate( 
		&camera.GetViewMatrix().m[0][0], 
		&camera.GetProjectionMatrix().m[0][0], 
		mCurrentGizmoOperation, 
		mCurrentGizmoMode, 
		&obj.mat_.m[0][0] );


	if (isManipulated) {
		std::cout << "Manipulate succeeded" << std::endl; // オブジェクトの行列を取得 
		float translation[3], rotation[3], scale[3]; 
		ImGuizmo::DecomposeMatrixToComponents(&obj.mat_.m[0][0], translation, rotation, scale); 
		obj.transform.translate = Vector3(translation[0], translation[1], translation[2]);
		obj.transform.rotate = Vector3(DegreesToRadians(rotation[0]), DegreesToRadians(rotation[1]), DegreesToRadians(rotation[2]));
		obj.transform.scale = Vector3(scale[0], scale[1], scale[2]); 
		std::cout << "Translation: " << translation[0] << ", " << translation[1] << ", " << translation[2] << std::endl;
		std::cout << "Rotation: " << rotation[0] << ", " << rotation[1] << ", " << rotation[2] << std::endl; 
		std::cout << "Scale: " << scale[0] << ", " << scale[1] << ", " << scale[2] << std::endl; 
	} else { 
		std::cout << "Manipulate failed" << std::endl; 
	}
	if (ImGuizmo::IsUsing()) { 
		std::cout << "ImGuizmo is using" << std::endl; 
	} else { 
		std::cout << "ImGuizmo is not using" << std::endl; 
	}

	if (ImGui::Button("TRANSLATE")) mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	if (ImGui::Button("ROTATE")) mCurrentGizmoOperation = ImGuizmo::ROTATE;
	if (ImGui::Button("SCALE")) mCurrentGizmoOperation = ImGuizmo::SCALE;

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
		ImGuizmo::DecomposeMatrixToComponents(&obj.mat_.m[0][0], translation, rotation, scale);
		Vector3 matrixTranslation = { translation[0], translation[1], translation[2] };
		Vector3 matrixRotation = { rotation[0], rotation[1], rotation[2] };
		Vector3 matrixScale = { scale[0], scale[1], scale[2] };
		ImGui::InputFloat3("Tr", &matrixTranslation.x);
		ImGui::InputFloat3("Rt", &matrixRotation.x);
		ImGui::InputFloat3("Sc", &matrixScale.x);
		ImGuizmo::RecomposeMatrixFromComponents(translation, rotation, scale, &obj.mat_.m[0][0]);
	}
	else {
		std::cout << "ImGuizmo is not using" << std::endl;
	}

	ImGuiIO& io = ImGui::GetIO();
	//ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
	ImGuizmo::Manipulate(&camera.GetViewMatrix().m[3][3], &camera.GetProjectionMatrix().m[3][3], mCurrentGizmoOperation, mCurrentGizmoMode, &obj.mat_.m[3][3], NULL,NULL);

	ImGui::End();
}





