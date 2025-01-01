#include "OceanManager.h"
#include "DirectXGame/engine/base/TextureManager.h"
#include "imgui.h"

OceanManager* OceanManager::instance = nullptr;
OceanManager* OceanManager::GetInstance()
{
	if (instance == nullptr) {
		instance = new OceanManager;
	}
	return instance;
}
void OceanManager::Initialize(DirectXCommon* dxCommon)
{
	dxCommon_ = dxCommon;

	CreateGraphicsPipeline();
}
void OceanManager::DrawCommonSetting()
{
	// RootSignatureを設定。PSOに設定しているけど別途設定が必要
	dxCommon_->GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());

	dxCommon_->GetCommandList()->SetPipelineState(graphicsPipelineState.Get()); //PSOを設定

	//形状を設定。PSOに設定している物とはまた別。同じものを設定すると考えておけば良い
	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);
	
}
void OceanManager::Finalize()
{
	delete instance;
	instance = nullptr;
}

void OceanManager::CreateRootSignature()
{
	HRESULT hr;

	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0; // 0から始まる
	descriptorRange[0].NumDescriptors = 1; // 数は1つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算

	

	// Roosignature(ルートシグネチャ)作成
	//ShaderとResorceをどのように関連付けるかを示したオブジェクト

	//Roosignature作成
	D3D12_ROOT_SIGNATURE_DESC descriptionSignature{};

	descriptionSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// RootParameter作成。複数指定できるのではい
	// RootParameter作成。複数指定できるのではい
	D3D12_ROOT_PARAMETER rootParameters[9] = {};

	// マテリアルデータ (b0) をピクセルシェーダで使用する
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;   // CBVを使う　// b0のbと一致する
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; //PixelShaderで使う
	rootParameters[0].Descriptor.ShaderRegister = 0;    // レジスタ番号0とバインド　　// b0の0と一致する。もしb11と紐づけたいなら11となる

	// マテリアルデータ (b0) を頂点シェーダで使用する
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX; // 両方で使う
	rootParameters[1].Descriptor.ShaderRegister = 0;    // レジスタ番号0とバインド

	// テクスチャデータ (t0) をピクセルシェーダで使用する
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // DescriptorTableを使う           
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange; // Tableの中身の配列を指定
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange); // Tableで利用する数 

	// 方向性ライトデータ (b1) をピクセルシェーダで使用する
	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[3].Descriptor.ShaderRegister = 1;

	// カメラデータ (b2) をピクセルシェーダで使用する
	rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[4].Descriptor.ShaderRegister = 2;

	// ポイントライトデータ (b3) をピクセルシェーダで使用する
	rootParameters[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[5].Descriptor.ShaderRegister = 3;

	// スポットライトデータ (b4) をピクセルシェーダで使用する
	rootParameters[6].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[6].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[6].Descriptor.ShaderRegister = 4;

	// スポットライトデータ (b5) をバーテックスシェーダで使用する
	rootParameters[7].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[7].ShaderVisibility = D3D12_SHADER_VISIBILITY_DOMAIN;
	rootParameters[7].Descriptor.ShaderRegister = 5;

	// マテリアルデータ (b0) を頂点シェーダで使用する
	rootParameters[8].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[8].ShaderVisibility = D3D12_SHADER_VISIBILITY_DOMAIN; // 両方で使う
	rootParameters[8].Descriptor.ShaderRegister = 0;    // レジスタ番号0とバインド



	descriptionSignature.pParameters = rootParameters;
	descriptionSignature.NumParameters = _countof(rootParameters);


	///Samplerの設定
	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; // バイリニアフィルタ
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP; // 0～1の範囲外をリピート
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; // 比較しない
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX; // ありったけのMipmapを使う
	staticSamplers[0].ShaderRegister = 0; //レジスタ番号0を使う
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	descriptionSignature.pStaticSamplers = staticSamplers;
	descriptionSignature.NumStaticSamplers = _countof(staticSamplers);


	//シリアライズにしてバイナリする
	Microsoft::WRL::ComPtr < ID3DBlob> signatureBlob = nullptr;
	Microsoft::WRL::ComPtr < ID3DBlob> errorBlob = nullptr;

	hr = D3D12SerializeRootSignature(&descriptionSignature,
		D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);

	if (FAILED(hr)) {
		Logger::Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));

		assert(false);
	}

	//バイナリを元に生成

	hr = dxCommon_->GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(),
		signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));

	assert(SUCCEEDED(hr));

}

void OceanManager::CreateGraphicsPipeline()
{
	HRESULT hr;
	CreateRootSignature();


	// InputLayout(インプットレイアウト)
	// VectorShaderへ渡す頂点データがどのようなものかを指定するオブジェクト


#pragma region InputLayout


	D3D12_INPUT_ELEMENT_DESC inputElementDescs[3] = {};
	inputElementDescs[0].SemanticName = "POSITION";  // 位置
	inputElementDescs[0].SemanticIndex = 0;  // セマンティックインデックス0
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;  // 位置のフォーマット
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;  // 次の要素へのオフセット

	inputElementDescs[1].SemanticName = "TEXCOORD";  // テクスチャ座標
	inputElementDescs[1].SemanticIndex = 0;  // セマンティックインデックス0
	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;  // テクスチャ座標のフォーマット
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;  // 次の要素へのオフセット

	inputElementDescs[2].SemanticName = "NORMAL";  // 法線
	inputElementDescs[2].SemanticIndex = 0;  // セマンティックインデックス0
	inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;  // 法線のフォーマット
	inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;  // 次の要素へのオフセット


	
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);


#pragma endregion //InputLayout(インプットレイアウト)


	// BlendState(ブレンドステート)の設定
	// PixeiShaderからの出力をどのように書き込むかを設定する
	//半透明処理はここの設定により制御され、書き込む色要素を決めることができる


#pragma region BlendState


	D3D12_BLEND_DESC blendDesc{};
	//すべての色要素を書き込む
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;

#pragma endregion //BlendState(ブレンドステート)


	// RasterizerState(ラスタライザステート)の設定
	// 三角形の内部をピクセルに分解して、PixelShaderを起動することで、この処理の設定

	D3D12_RASTERIZER_DESC rasterizerDesc{};

	//裏面(時計回り)を表示しない
	// カリングしない(裏面も表示させる)
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;

	//三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	//rasterizerDesc.DepthBias = 1; 
	//rasterizerDesc.DepthBiasClamp = 0.0f; 
	//rasterizerDesc.SlopeScaledDepthBias = 1.0f;

	// Shaderをコンパイルする
	Microsoft::WRL::ComPtr < IDxcBlob> vertexShaderBlob = dxCommon_->CompileShader(L"resources/shaders/Ocean/Ocean.VS.hlsl",
		L"vs_6_0");

	assert(vertexShaderBlob != nullptr);

	Microsoft::WRL::ComPtr < IDxcBlob> pixelShaderBlob = dxCommon_->CompileShader(L"resources/shaders/Ocean/Ocean.PS.hlsl",
		L"ps_6_0");

	assert(pixelShaderBlob != nullptr);

	// Shaderをコンパイルする
	Microsoft::WRL::ComPtr < IDxcBlob> DSBlob = dxCommon_->CompileShader(L"resources/shaders/Ocean/Ocean.DS.hlsl",
		L"ds_6_0");

	assert(vertexShaderBlob != nullptr);

	Microsoft::WRL::ComPtr < IDxcBlob> HSBlob = dxCommon_->CompileShader(L"resources/shaders/Ocean/Ocean.HS.hlsl",
		L"hs_6_0");

	assert(pixelShaderBlob != nullptr);


	// PSOを作成する

	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};

	graphicsPipelineStateDesc.pRootSignature = rootSignature.Get();// RootSignature

	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc;// InputLayout

	graphicsPipelineStateDesc.VS = { vertexShaderBlob->GetBufferPointer(),
	vertexShaderBlob->GetBufferSize() }; // VertexShader

	graphicsPipelineStateDesc.PS = { pixelShaderBlob->GetBufferPointer(),
	pixelShaderBlob->GetBufferSize() }; // PixelShader

	graphicsPipelineStateDesc.DS = { DSBlob->GetBufferPointer(),
	DSBlob->GetBufferSize() }; // VertexShader

	graphicsPipelineStateDesc.HS = { HSBlob->GetBufferPointer(),
	HSBlob->GetBufferSize() }; // PixelShader



	graphicsPipelineStateDesc.BlendState = blendDesc; //BlendState

	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc;// RasterizerState

	//書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	//利用するトロポジ(形状)のタイプ。三角形
	graphicsPipelineStateDesc.PrimitiveTopologyType =
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// 修正後
	graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;

	//どのように画面に色を打ち込むかの設定(気にしなくて良い)
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;



	//DepthStencilStateの設定を行う
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	// Depthの機能を有効化する
	depthStencilDesc.DepthEnable = true;
	// 書き込みします
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	// 比較関数はLessEqual。つまり、近ければ描画される
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;


	// DepthStencilの設定
	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;


	hr = dxCommon_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&graphicsPipelineState));

	assert(SUCCEEDED(hr));
}
