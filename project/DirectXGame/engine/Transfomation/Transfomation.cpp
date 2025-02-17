#include "Transfomation.h"


void Transfomation::Initialize(DirectXCommon* dxCommon)
{
	dxCommon_ = dxCommon;

	//トランスフォーム
	resource_ = dxCommon_->CreateBufferResource(sizeof(TransfomationMatrix));

	//書き込むためのアドレスを取得
	resource_->Map(0, nullptr, reinterpret_cast<void**>(&data_));

	//単位行列を書き込んでおく
	data_->WVP = MakeIdentity4x4();
	data_->World = MakeIdentity4x4();
	data_->worldInverseTranspose = MakeIdentity4x4();
}

void Transfomation::Update(Model* model, Camera* camera, Matrix4x4& local, Matrix4x4& mat)
{
	Matrix4x4 worldViewProjectionMatrix{};

	if (camera) {
		// WVP計算
		Matrix4x4 worldViewProjectionMatrix{};

		worldViewProjectionMatrix = Multiply(local, mat); // ワールド変換

		worldViewProjectionMatrix = Multiply(worldViewProjectionMatrix, camera->GetViewMatrix()); // ビュー変換
		worldViewProjectionMatrix = Multiply(worldViewProjectionMatrix, camera->GetProjectionMatrix()); // 射影変換



		if (model) {

			data_->WVP = worldViewProjectionMatrix;
			data_->World = Multiply(local, mat);

		}
		else {
			data_->WVP = worldViewProjectionMatrix;
			data_->World = mat;
		}
	}
	else {
		worldViewProjectionMatrix = mat;
		data_->WVP = worldViewProjectionMatrix;
		data_->World = mat;
	}

	data_->worldInverseTranspose = Transpose(Inverse(mat));

}

void Transfomation::Update(Camera* camera, Matrix4x4& mat)
{
	Matrix4x4 worldViewProjectionMatrix{};

	if (camera) {

		worldViewProjectionMatrix = mat;

		worldViewProjectionMatrix = Multiply(worldViewProjectionMatrix, camera->GetViewMatrix()); // ビュー変換
		worldViewProjectionMatrix = Multiply(worldViewProjectionMatrix, camera->GetProjectionMatrix()); // 射影変換

		data_->WVP = worldViewProjectionMatrix;
		data_->World = mat;
	}
	else {
		worldViewProjectionMatrix = mat;
		data_->WVP = worldViewProjectionMatrix;
		data_->World = mat;
	}

	data_->worldInverseTranspose = Transpose(Inverse(mat));
}

void Transfomation::UpdateSkinning(Model* model, Camera* camera, Matrix4x4& local, Matrix4x4& mat)
{
	Matrix4x4 worldViewProjectionMatrix{};

	if (camera) {

		worldViewProjectionMatrix = mat;

		worldViewProjectionMatrix = Multiply(worldViewProjectionMatrix, camera->GetViewMatrix()); // ビュー変換
		worldViewProjectionMatrix = Multiply(worldViewProjectionMatrix, camera->GetProjectionMatrix()); // 射影変換



		if (model) {

			data_->WVP = worldViewProjectionMatrix;
			data_->World = Multiply(local, mat);

		}
		else {
			data_->WVP = worldViewProjectionMatrix;
			data_->World = mat;
		}
	}
	else {
		worldViewProjectionMatrix = mat;
		data_->WVP = worldViewProjectionMatrix;
		data_->World = mat;
	}

	data_->worldInverseTranspose = Transpose(Inverse(mat));

}

void Transfomation::UpdateSprite(Matrix4x4& mat)
{
	data_->World = mat;
	data_->WVP = mat;
}

void Transfomation::GetCommandList(int index)
{
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(index, resource_->GetGPUVirtualAddress());
}

void Transfomation::SetRootParameter(D3D12_ROOT_PARAMETER& parameter, int ShaderRegister)
{
	parameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;   // CBVを使う　// b0のbと一致する
	parameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX; //PixelShaderで使う
	parameter.Descriptor.ShaderRegister = ShaderRegister;    // レジスタ番号0とバインド　　// b0の0と一致する。もしb11と紐づけたいなら11となる
}
