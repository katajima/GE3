#include<Windows.h>
#include"winuser.h"
#include<cstdint>
#include<string>
#include<format>
#include<d3d12.h>
#include<dxgi1_6.h>
#include<cassert>
#include<dxgidebug.h>
#include<dxcapi.h>
#include<fstream>
#include<sstream>
#include<wrl.h>

#include"Structs.h"
#include"MathFanctions.h"
#include"Input.h"
#include"WinApp.h"
#include"DirectXCommon.h"
#include"D3DResourceLeakchecker.h"
#include"Sprite.h"
#include"SpriteCommon.h"



#include"externals/DirectXTex/DirectXTex.h"
#include"externals/DirectXTex/d3dx12.h"

#include<vector>

//ImGui
#include"externals/imgui/imgui.h"
#include"externals/imgui/imgui_impl_dx12.h"
#include"externals/imgui/imgui_impl_win32.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dxcompiler.lib")

#include"StringUtility.h"
#include"Logger.h"



struct VertexData {

	Vector4 position;
	Vector2 texcoord;
	Vector3 normal;
};

struct Material {
	Vector4 color;
	int32_t enableLighting;
	float padding[3];
	Matrix4x4 uvTransform;
};
struct MaterialData {
	std::string textuerFilePath;
};

struct TransfomationMatrix
{
	Matrix4x4 WVP;
	Matrix4x4 World;
};

struct DirectionalLight {
	Vector4 color; //!< ライトの色
	Vector3 direction; //!< ライトの向き
	float intensity; //!< 輝度
};
//モデルデータ
struct ModelData
{
	std::vector<VertexData> vertices;
	MaterialData material;
};








//マテリアルデータを読み込む
MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename) {

	MaterialData materialData; // 構築するMaterialData
	std::string line; // ファイルから読んだ1行を格納するもの
	std::ifstream file(directoryPath + "/" + filename); //ファイルを開く
	assert(file.is_open()); //とりあえず開けなかったら

	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;

		// identifierに応じた処理
		if (identifier == "map_Kd") {
			std::string textureFilename;
			s >> textureFilename;
			// 連結してファイルパスにする
			materialData.textuerFilePath = directoryPath + "/" + textureFilename;
		}
	}

	return materialData;
}

//モデルデータ読み込み
ModelData LoadOdjFile(const std::string& directoryPath, const std::string& filename) {
	//必要な変数の宣言とファイルを開く
	ModelData modelData;//構築するModelData
	std::vector<Vector4> positions; //位置
	std::vector<Vector3> normals; //法線
	std::vector<Vector2> texcoords; //テクスチャ座標
	std::string line; //ファイルから読んだ1行を格納する

	std::ifstream file(directoryPath + "/" + filename);//ファイルを開く
	assert(file.is_open()); //とりあえず開けなかったら止める

	//ファイル読み、ModelDataを構築
	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier; //先頭の識別子を読む

		//頂点情報を読み込む
		if (identifier == "v") {
			Vector4 position;
			s >> position.x >> position.y >> position.z;
			position.w = 1.0f;
			position.x *= -1.0f;
			positions.push_back(position);
		}
		else if (identifier == "vt") {
			Vector2 texcoord;
			s >> texcoord.x >> texcoord.y;
			texcoord.y = 1.0f - texcoord.y;
			texcoords.push_back(texcoord);
		}
		else if (identifier == "vn") {
			Vector3 normal;
			s >> normal.x >> normal.y >> normal.z;
			normal.x *= -1.0f;
			normals.push_back(normal);
		}
		else if (identifier == "f") {
			VertexData triangle[3];
			//面は三角形限定。その他は未対応
			for (int32_t faceVertex = 0; faceVertex < 3; ++faceVertex) {
				std::string vertexDefinition;
				s >> vertexDefinition;
				// 頂点の要素へのindexは[位置/UV/法線]で格納されているので、分解してindexを取得する
				std::istringstream v(vertexDefinition);
				uint32_t elementIndices[3];
				for (int32_t element = 0; element < 3; ++element) {
					std::string index;
					std::getline(v, index, '/');// /区切りでインデックスを読んでいく
					elementIndices[element] = std::stoi(index);
				}
				//要素へのindexから、実際の要素の値を取得して、頂点を構築する
				Vector4 position = positions[elementIndices[0] - 1];
				Vector2 texcoord = texcoords[elementIndices[1] - 1];
				Vector3 normal = normals[elementIndices[2] - 1];
				//VertexData vertex = { position,texcoord,normal };
				//modelData.vertices.push_back(vertex);
				triangle[faceVertex] = { position,texcoord,normal };
			}
			//頂点を逆順で登録することで、周り順を逆にする
			modelData.vertices.push_back(triangle[2]);
			modelData.vertices.push_back(triangle[1]);
			modelData.vertices.push_back(triangle[0]);
		}
		else if (identifier == "mtllib") {
			// MaterialTemplateLibraryファイルの名前を取得する
			std::string materialFilename;
			s >> materialFilename;
			//基本的にobjファイルと同一階層にmtlは存在させるので、ディレクトリ名とファイル名を渡す
			modelData.material = LoadMaterialTemplateFile(directoryPath, materialFilename);
		}

	}

	return modelData;
};







//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	D3DResourceLeakchecker leakCheck;
	leakCheck.~D3DResourceLeakchecker();

	//COMの初期化
	//CoInitializeEx(0, COINIT_MULTITHREADED);


	WinApp* winApp = nullptr;
	// WindowsAPI解放
	winApp = new WinApp();
	winApp->Initialize();

	//ポインタ
	Input* input = nullptr;
	// 入力初期化
	input = new Input();
	input->Intialize(winApp);

	DirectXCommon* dxCommon = nullptr;
	dxCommon = new DirectXCommon();
	dxCommon->Intialize(winApp);

	SpriteCommon* spriteCommon = nullptr;
	// スプライト共通部の初期化
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(dxCommon);

	Sprite* sprite = new Sprite();
	sprite->Initialize(spriteCommon);








	//D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	//descriptorRange[0].BaseShaderRegister = 0; // 0から始まる
	//descriptorRange[0].NumDescriptors = 1; // 数は1つ
	//descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
	//descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算
	////descriptorRange[1].BaseShaderRegister = 0; // 0から始まる
	////descriptorRange[1].NumDescriptors = 3; // 数は1つ
	////descriptorRange[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV; // CBVを使う
	////descriptorRange[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算


	////------PSO------////

	//// Roosignature(ルートシグネチャ)作成
	////ShaderとResorceをどのように関連付けるかを示したオブジェクト

	////Roosignature作成
	//D3D12_ROOT_SIGNATURE_DESC descriptionSignature{};

	//descriptionSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	//// RootParameter作成。複数指定できるのではい
	//D3D12_ROOT_PARAMETER rootParameters[4] = {};

	//rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;   // CBVを使う　// b0のbと一致する
	//rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; //PixelShaderで使う
	//rootParameters[0].Descriptor.ShaderRegister = 0;    // レジスタ番号0とバインド　　// b0の0と一致する。もしb11と紐づけたいなら11となる

	//rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;   // CBVを使う　// b0のbと一致する
	//rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX; //VertexShaderで使う
	//rootParameters[1].Descriptor.ShaderRegister = 0;    // レジスタ番号0とバインド　　// b0の0と一致する。もしb11と紐づけたいなら11となる

	//rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // DescriptorTableを使う           
	//rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	//rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange; // Tableの中身の配列を指定
	//rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange); // Tableで利用する数 

	//rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // CBVを使う           
	//rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	//rootParameters[3].Descriptor.ShaderRegister = 1; //レジスタ番号1を使う

	//descriptionSignature.pParameters = rootParameters;   // ルートパラメータ配列へのポインタ
	//descriptionSignature.NumParameters = _countof(rootParameters);  //配列の長さ


	/////Samplerの設定
	//D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	//staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; // バイリニアフィルタ
	//staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP; // 0～1の範囲外をリピート
	//staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	//staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	//staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; // 比較しない
	//staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX; // ありったけのMipmapを使う
	//staticSamplers[0].ShaderRegister = 0; //レジスタ番号0を使う
	//staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	//descriptionSignature.pStaticSamplers = staticSamplers;
	//descriptionSignature.NumStaticSamplers = _countof(staticSamplers);


	////シリアライズにしてバイナリする
	//ID3DBlob* signatureBlob = nullptr;
	//ID3DBlob* errorBlob = nullptr;

	//hr = D3D12SerializeRootSignature(&descriptionSignature,
	//	D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);

	//if (FAILED(hr)) {
	//	Logger::Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));

	//	assert(false);
	//}

	////バイナリを元に生成
	//Microsoft::WRL::ComPtr < ID3D12RootSignature> rootSignature = nullptr;

	//hr = dxCommon->GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(),
	//	signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));

	//assert(SUCCEEDED(hr));


	// InputLayout(インプットレイアウト)
	// VectorShaderへ渡す頂点データがどのようなものかを指定するオブジェクト


//#pragma region InputLayout
//
//
//	D3D12_INPUT_ELEMENT_DESC inputElementDescs[3] = {};
//	inputElementDescs[0].SemanticName = "POSITION";
//	inputElementDescs[0].SemanticIndex = 0;
//	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
//	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
//	inputElementDescs[1].SemanticName = "TEXCOORD";
//	inputElementDescs[1].SemanticIndex = 0;
//	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
//	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
//	inputElementDescs[2].SemanticName = "NORMAL";
//	inputElementDescs[2].SemanticIndex = 0;
//	inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
//	inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
//
//	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
//	inputLayoutDesc.pInputElementDescs = inputElementDescs;
//	inputLayoutDesc.NumElements = _countof(inputElementDescs);
//
//
//#pragma endregion //InputLayout(インプットレイアウト)

//
//#pragma region BlendState
//
//
//	D3D12_BLEND_DESC blendDesc{};
//	//すべての色要素を書き込む
//	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
//	blendDesc.RenderTarget[0].BlendEnable = TRUE;
//	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
//	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
//	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
//	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
//	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
//	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
//
//#pragma endregion //BlendState(ブレンドステート)
//
//
//	// RasterizerState(ラスタライザステート)の設定
//	// 三角形の内部をピクセルに分解して、PixelShaderを起動することで、この処理の設定
//
//	D3D12_RASTERIZER_DESC rasterizerDesc{};
//
//	//裏面(時計回り)を表示しない
//	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
//
//	//三角形の中を塗りつぶす
//	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
//
//
//	// Shaderをコンパイルする
//	Microsoft::WRL::ComPtr < IDxcBlob> vertexShaderBlob = dxCommon->CompileShader(L"resources/shaders/Object3d.VS.hlsl",
//		L"vs_6_0");
//
//	assert(vertexShaderBlob != nullptr);
//
//	Microsoft::WRL::ComPtr < IDxcBlob> pixelShaderBlob = dxCommon->CompileShader(L"resources/shaders/Object3d.PS.hlsl",
//		L"ps_6_0");
//
//	assert(pixelShaderBlob != nullptr);
//
//
//	// PSOを作成する
//
//	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
//
//	graphicsPipelineStateDesc.pRootSignature = rootSignature.Get();// RootSignature
//
//	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc;// InputLayout
//
//	graphicsPipelineStateDesc.VS = { vertexShaderBlob->GetBufferPointer(),
//	vertexShaderBlob->GetBufferSize() }; // VertexShader
//
//	graphicsPipelineStateDesc.PS = { pixelShaderBlob->GetBufferPointer(),
//	pixelShaderBlob->GetBufferSize() }; // PixelShader
//
//	graphicsPipelineStateDesc.BlendState = blendDesc; //BlendState
//
//	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc;// RasterizerState
//
//	//書き込むRTVの情報
//	graphicsPipelineStateDesc.NumRenderTargets = 1;
//	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
//
//	//利用するトロポジ(形状)のタイプ。三角形
//	graphicsPipelineStateDesc.PrimitiveTopologyType =
//		D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
//
//	//どのように画面に色を打ち込むかの設定(気にしなくて良い)
//	graphicsPipelineStateDesc.SampleDesc.Count = 1;
//	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
//
//	//実際に生成
//	Microsoft::WRL::ComPtr < ID3D12PipelineState> graphicsPipelineState = nullptr;
//
//
//	//DepthStencilStateの設定を行う
//	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
//	// Depthの機能を有効化する
//	depthStencilDesc.DepthEnable = true;
//	// 書き込みします
//	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
//	// 比較関数はLessEqual。つまり、近ければ描画される
//	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
//
//
//	// DepthStencilの設定
//	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
//	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
//
//
//	hr = dxCommon->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
//		IID_PPV_ARGS(&graphicsPipelineState));
//
//	assert(SUCCEEDED(hr));


	////------平行光源用のResourceを作る------////


#pragma region MyRegion

	//平行光源用のリソースを作る
	Microsoft::WRL::ComPtr < ID3D12Resource> directionalLightResource = dxCommon->CreateBufferResource(sizeof(DirectionalLight));


	DirectionalLight* directionalLightData = nullptr;

	directionalLightResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData));


	//今回は赤を書き込んで見る //白
	*directionalLightData = DirectionalLight({ 1.0f,1.0f,1.0f,1.0f }, { 0.0f,-1.0f,0.0f }, 1.0f);


#pragma endregion //平行光源用のResource


	////------Material用のResourceを作る------////


#pragma region Material_Resource

	//マテリアル用のリソースを作る。今回はcolor1つ分のサイズを利用する
	Microsoft::WRL::ComPtr < ID3D12Resource> materialResource = dxCommon->CreateBufferResource(sizeof(Material));

	//マテリアルにデータを書き込む
	//Vector4* materialData = nullptr;

	// Lightingを有効にする
	Material* materialData = nullptr;
	//materialDataSprit->enableLighting = false;

	//書き込むためのアドレスを取得
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));

	materialData->uvTransform = MakeIdentity4x4();

	//今回は赤を書き込んで見る //白
	*materialData = Material({ 1.0f, 1.0f, 1.0f, 1.0f }, { true }); //RGBA

#pragma endregion //マテリアル用Resource


	////------MaterialSprite用のResourceを作る------////

#pragma region Material_Resource

	//マテリアル用のリソースを作る。今回はcolor1つ分のサイズを利用する
	Microsoft::WRL::ComPtr < ID3D12Resource> materialResourceObj = dxCommon->CreateBufferResource(sizeof(Material));

	//マテリアルにデータを書き込む
	//Vector4* materialData = nullptr;

	// Lightingを有効にする
	Material* materialDataObj = nullptr;

	//書き込むためのアドレスを取得
	materialResourceObj->Map(0, nullptr, reinterpret_cast<void**>(&materialDataObj));

	//スプライトはLightingしないのでfalseにする
	materialDataObj->enableLighting = false;

	materialDataObj->uvTransform = MakeIdentity4x4();

	//今回は赤を書き込んで見る //白
	*materialDataObj = Material({ 1.0f, 1.0f, 1.0f, 1.0f }, { false }); //RGBA

#pragma endregion //マテリアルOBJスプライト用Resource


	////------Matrix用のResourceを作る------////


#pragma region Matrix_Resource


	////------transformationMatrixResource------////三角形用

	Microsoft::WRL::ComPtr < ID3D12Resource> transformationMatrixResource = dxCommon->CreateBufferResource(sizeof(TransfomationMatrix));

	//データを書き込む
	TransfomationMatrix* transformationMatrixData = nullptr;

	//書き込むためのアドレスを取得
	transformationMatrixResource->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData));

	//単位行列を書き込んでおく
	transformationMatrixData->WVP = MakeIdentity4x4();
	transformationMatrixData->World = MakeIdentity4x4();


	////------transformationMatrixResourceSphar------////球用

	Microsoft::WRL::ComPtr < ID3D12Resource> transformationMatrixResourceSphar = dxCommon->CreateBufferResource(sizeof(TransfomationMatrix));

	//データを書き込む
	TransfomationMatrix* transformationMatrixDataSphar = nullptr;

	//書き込むためのアドレスを取得
	transformationMatrixResourceSphar->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixDataSphar));

	//単位行列を書き込んでおく
	transformationMatrixDataSphar->World = MakeIdentity4x4();
	transformationMatrixDataSphar->WVP = MakeIdentity4x4();


	Microsoft::WRL::ComPtr < ID3D12Resource> transformationMatrixResourceObj = dxCommon->CreateBufferResource(sizeof(TransfomationMatrix));

	//データを書き込む
	TransfomationMatrix* transformationMatrixDataObj = nullptr;

	//書き込むためのアドレスを取得
	transformationMatrixResourceObj->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixDataObj));

	//単位行列を書き込んでおく
	transformationMatrixDataObj->World = MakeIdentity4x4();
	transformationMatrixDataObj->WVP = MakeIdentity4x4();

#pragma endregion //Matrix用_Resource


	////------VertexBufferViewを作成する------////


#pragma region MyRegion

	////------VertexResourceを生成する------////

	Microsoft::WRL::ComPtr < ID3D12Resource> vertexResource = dxCommon->CreateBufferResource(sizeof(VertexData) * 6);

	//頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

	//リソースの先頭のアドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();

	//使用するリソースのサイズは頂点3つ分のサイズ
	vertexBufferView.SizeInBytes = sizeof(VertexData) * 6;

	// 1頂点あたりのサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);


	//Resourceにデータを書き込む
	VertexData* vertexData = nullptr;

	//書き込むためのアドレスを取得
	vertexResource->Map(0, nullptr,
		reinterpret_cast<void**>(&vertexData));
	//左下
	vertexData[0].position = { -0.5f,-0.5f,0.0f,1.0f };
	vertexData[0].texcoord = { 0.0f,1.0f };
	vertexData[0].normal = { vertexData[0].position.x,vertexData[0].position.y,vertexData[0].position.z };
	//上
	vertexData[1].position = { 0.0f,0.5f,0.0f,1.0f };
	vertexData[1].texcoord = { 0.5f,0.0f };
	vertexData[1].normal = { vertexData[1].position.x,vertexData[1].position.y,vertexData[1].position.z };
	//右下
	vertexData[2].position = { 0.5f,-0.5f,0.0f,1.0f };
	vertexData[2].texcoord = { 1.0f,1.0f };
	vertexData[2].normal = { vertexData[2].position.x,vertexData[2].position.y,vertexData[2].position.z };
	//左下２
	vertexData[3].position = { -0.5f,-0.5f,0.5f,1.0f };
	vertexData[3].texcoord = { 0.0f,1.0f };
	vertexData[3].normal = { vertexData[3].position.x,vertexData[3].position.y,vertexData[3].position.z };

	//上２
	vertexData[4].position = { 0.0f,0.0f,0.0f,1.0f };
	vertexData[4].texcoord = { 0.5f,0.5f };
	vertexData[4].normal = { vertexData[4].position.x,vertexData[4].position.y,vertexData[4].position.z };

	//右下２
	vertexData[5].position = { 0.5f,-0.5f,-0.5f,1.0f };
	vertexData[5].texcoord = { 1.0f,1.0f };
	vertexData[5].normal = { vertexData[5].position.x,vertexData[5].position.y,vertexData[5].position.z };


#pragma endregion //三角形


	////------円------////


#pragma region 球

	float pi = 3.14f;
	const uint32_t kSubdivision = 32; //分割数
	const float kLonEvery = pi * 2 / float(kSubdivision);      //経度分割1つ分の角度φ
	const float kLatEvery = pi / float(kSubdivision);      //緯度分割1つ分の角度θ
	const float r = 0.5f;
	const uint32_t vetexSphar4 = kSubdivision * kSubdivision * 4;

	Microsoft::WRL::ComPtr < ID3D12Resource> vertexResouceSphar
		= dxCommon->CreateBufferResource(sizeof(VertexData) * vetexSphar4);

	//頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferViewSphar{};
	//リソースの先頭のアドレスを作成する
	vertexBufferViewSphar.BufferLocation = vertexResouceSphar->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点6つの分のサイズ
	vertexBufferViewSphar.SizeInBytes = sizeof(VertexData) * vetexSphar4;
	//1頂点当たりのサイズ
	vertexBufferViewSphar.StrideInBytes = sizeof(VertexData);

	//頂点データを設定する
	VertexData* vertexDataSphar = nullptr;
	vertexResouceSphar->Map(0, nullptr, reinterpret_cast<void**>(&vertexDataSphar));


	//　緯度の方向に分割　-π/2 ～ π/2
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = -pi / 2.0f + kLatEvery * latIndex;// 現在の緯度θ
		//　経度の方向に分割　0 ～ 2π
		//　頂点の方向に分割しながら線を描く
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			uint32_t start = (latIndex * kSubdivision + lonIndex) * 4; // θ
			float lon = lonIndex * kLonEvery; //φ

			float u = float(lonIndex) / float(kSubdivision);
			float v = 1.0f - float(latIndex) / float(kSubdivision);

			//頂点データを入力する。基準点a
			vertexDataSphar[start].position.x = r * std::cos(lat) * std::cos(lon);
			vertexDataSphar[start].position.y = r * std::sin(lat);
			vertexDataSphar[start].position.z = r * std::cos(lat) * std::sin(lon);
			vertexDataSphar[start].position.w = 1.0f;
			vertexDataSphar[start].texcoord = { u,v + (1.0f / float(kSubdivision - 1)) };
			vertexDataSphar[start].normal.x = vertexDataSphar[start].position.x;
			vertexDataSphar[start].normal.y = vertexDataSphar[start].position.y;
			vertexDataSphar[start].normal.z = vertexDataSphar[start].position.z;

			////基準点b
			vertexDataSphar[start + 1].position.x = r * std::cos(lat + kLatEvery) * std::cos(lon);
			vertexDataSphar[start + 1].position.y = r * std::sin(lat + kLatEvery);
			vertexDataSphar[start + 1].position.z = r * std::cos(lat + kLatEvery) * std::sin(lon);
			vertexDataSphar[start + 1].position.w = 1.0f;
			vertexDataSphar[start + 1].texcoord = { u,v };
			vertexDataSphar[start + 1].normal.x = vertexDataSphar[start + 1].position.x;
			vertexDataSphar[start + 1].normal.y = vertexDataSphar[start + 1].position.y;
			vertexDataSphar[start + 1].normal.z = vertexDataSphar[start + 1].position.z;


			////基準点c
			vertexDataSphar[start + 2].position.x = r * std::cos(lat) * std::cos(lon + kLonEvery);
			vertexDataSphar[start + 2].position.y = r * std::sin(lat);
			vertexDataSphar[start + 2].position.z = r * std::cos(lat) * std::sin(lon + kLonEvery);
			vertexDataSphar[start + 2].position.w = 1.0f;
			vertexDataSphar[start + 2].texcoord = { u + (1.0f / float(kSubdivision - 1)),v + (1.0f / float(kSubdivision - 1)) };
			vertexDataSphar[start + 2].normal.x = vertexDataSphar[start + 2].position.x;
			vertexDataSphar[start + 2].normal.y = vertexDataSphar[start + 2].position.y;
			vertexDataSphar[start + 2].normal.z = vertexDataSphar[start + 2].position.z;



			//基準点d
			vertexDataSphar[start + 3].position.x = r * std::cos(lat + kLatEvery) * std::cos(lon + kLonEvery);
			vertexDataSphar[start + 3].position.y = r * std::sin(lat + kLatEvery);
			vertexDataSphar[start + 3].position.z = r * std::cos(lat + kLatEvery) * std::sin(lon + kLonEvery);
			vertexDataSphar[start + 3].position.w = 1.0f;
			vertexDataSphar[start + 3].texcoord = { u + (1.0f / float(kSubdivision - 1)),v };
			vertexDataSphar[start + 3].normal.x = vertexDataSphar[start + 3].position.x;
			vertexDataSphar[start + 3].normal.y = vertexDataSphar[start + 3].position.y;
			vertexDataSphar[start + 3].normal.z = vertexDataSphar[start + 3].position.z;


		}
	}

#pragma endregion //球体


#pragma region Sphar

	const uint32_t indexSphar6 = kSubdivision * kSubdivision * 6;

	//index用のあれやこれを作る
	Microsoft::WRL::ComPtr < ID3D12Resource> indexResourceSphar = dxCommon->CreateBufferResource(sizeof(uint32_t) * indexSphar6);

	D3D12_INDEX_BUFFER_VIEW indexBufferViewSphar{};

	// リソースの先頭のアドレスから使う
	indexBufferViewSphar.BufferLocation = indexResourceSphar->GetGPUVirtualAddress();

	// 使用するリソースのサイズはインデック6つ分のサイズ
	indexBufferViewSphar.SizeInBytes = sizeof(uint32_t) * indexSphar6;

	// インデックはuint32_tとする
	indexBufferViewSphar.Format = DXGI_FORMAT_R32_UINT;


	// インデックリソースにデータを書き込む
	uint32_t* indexDataSphar = nullptr;
	indexResourceSphar->Map(0, nullptr, reinterpret_cast<void**>(&indexDataSphar));
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			uint32_t start = (latIndex * kSubdivision + lonIndex) * 6; // θ
			uint32_t start2 = (latIndex * kSubdivision + lonIndex) * 4; // θ
			indexDataSphar[start] = (start2 + 0);
			indexDataSphar[start + 1] = (start2 + 1);
			indexDataSphar[start + 2] = (start2 + 2);


			indexDataSphar[start + 3] = (start2 + 1);
			indexDataSphar[start + 4] = (start2 + 3);
			indexDataSphar[start + 5] = (start2 + 2);
		}
	}

#pragma endregion //球体インデックス


	///------

	//モデル読み込み
	ModelData modeldata = LoadOdjFile("resources", "fence.obj");

	//頂点リソースを作る
	Microsoft::WRL::ComPtr < ID3D12Resource> vertexResourceObj = dxCommon->CreateBufferResource(sizeof(VertexData) * modeldata.vertices.size());

	//頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferViewObj{};
	vertexBufferViewObj.BufferLocation = vertexResourceObj->GetGPUVirtualAddress();
	vertexBufferViewObj.SizeInBytes = UINT(sizeof(VertexData) * modeldata.vertices.size());
	vertexBufferViewObj.StrideInBytes = sizeof(VertexData);

	//頂点データを書き込む
	VertexData* vertexDataObj = nullptr;
	vertexResourceObj->Map(0, nullptr, reinterpret_cast<void**>(&vertexDataObj));
	std::memcpy(vertexDataObj, modeldata.vertices.data(), sizeof(VertexData) * modeldata.vertices.size());




	////------Texture------////


#pragma region Texture

	/// Textureを読んで転送する

	//一枚目のTextureを読んで転送する
	DirectX::ScratchImage mipImages = dxCommon->LoadTexture("resources/uvChecker.png");
	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();
	Microsoft::WRL::ComPtr < ID3D12Resource> textureResource = dxCommon->CreateTextureResource(metadata);

	Microsoft::WRL::ComPtr < ID3D12Resource> intermediateResource = dxCommon->UploadTextureData(textureResource.Get(), mipImages);



	///metadataを基にSRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; // 2Dテクスチャ
	srvDesc.Texture2D.MipLevels = UINT(metadata.mipLevels);


	// SRVを作成するDescriptorHeapの場所を決める
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU = dxCommon->GetSrvDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU = dxCommon->GetSrvDescriptorHeap()->GetGPUDescriptorHandleForHeapStart();


	// 先頭はImGuiが使っているのでその次を使う
	textureSrvHandleCPU.ptr += dxCommon->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	textureSrvHandleGPU.ptr += dxCommon->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);


	// SRV
	dxCommon->GetDevice()->CreateShaderResourceView(textureResource.Get(), &srvDesc, textureSrvHandleCPU);


#pragma endregion //Texture


	////------Texture------////モンスターボール


#pragma region Texture

	//二枚目のTextureを読んで転送する
	DirectX::ScratchImage mipImages2 = dxCommon->LoadTexture(modeldata.material.textuerFilePath);
	const DirectX::TexMetadata& metadata2 = mipImages2.GetMetadata();
	Microsoft::WRL::ComPtr < ID3D12Resource> textureResource2 = dxCommon->CreateTextureResource(metadata2);

	Microsoft::WRL::ComPtr < ID3D12Resource> intermediateResource2 = dxCommon->UploadTextureData(textureResource2.Get(), mipImages2);


	//
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc2{};
	srvDesc2.Format = metadata2.format;
	srvDesc2.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc2.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; // 2Dテクスチャ
	srvDesc2.Texture2D.MipLevels = UINT(metadata2.mipLevels);

	// SRVを作成するDescriptorHeapの場所を決める
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU2 = dxCommon->GetSRVCPUDescriptorHandle(2);
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU2 = dxCommon->GetSRVGPUDescriptorHandle(2);


	// SRV
	dxCommon->GetDevice()->CreateShaderResourceView(textureResource2.Get(), &srvDesc2, textureSrvHandleCPU2);


#pragma endregion //Texture2

	//transform変数を作る
	Transform transform{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

	//カメラ
	Transform cameraTransform{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-10.0f} };

	// CPUで動かす用のTransformを作る
	Transform transformSprite{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

	//UVTransform用
	Transform uvTransformSprite{
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};

	//UVTransform用
	Transform uvTransformSphar{
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};

	//UVTransform用
	Transform uvTransformObj{
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};

	//透視射影行列
	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(winApp->GetClientWidth()) / float(winApp->GetClientHeight()), 0.1f, 100.0f);





	materialDataObj->enableLighting = 1;

	//ウィンドウの×ボタンが押されるまでループ
	while (true) {
		// Windowsのメッセージ処理
		if (winApp->ProcessMessage()) {
			// ゲームループを抜ける
			break;
		};

		//ゲームの処理
		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();


		input->Update();

		sprite->Update();


		transform.rotate.y = 3.14f;
		ImGui::Begin("Window");
		ImGui::DragFloat3("translate", &transform.translate.x);
		ImGui::DragFloat3("rotate", &transform.rotate.x);
		ImGui::DragFloat3("camera.translate.x", &cameraTransform.translate.x, 0.1f);
		ImGui::SliderFloat3("camera.rotate", &cameraTransform.rotate.x, -3.14f, 3.14f);
		ImGui::DragInt("enableLighting", &materialDataObj->enableLighting);
		ImGui::DragFloat3("LightDirection", &directionalLightData->direction.x);
		ImGui::DragFloat("Intensity", &directionalLightData->intensity, 0.01f);
		directionalLightData->direction = Nomalize(directionalLightData->direction);
		ImGui::ColorEdit4("color", &materialDataObj->color.x);
		ImGui::DragFloat2("UVTranslate", &uvTransformSprite.translate.x, 0.01f, -10.0f, 10.0f);
		ImGui::DragFloat2("UVSScale", &uvTransformSprite.scale.x, 0.1f, -10.0f, 10.0f);
		ImGui::SliderAngle("UVRotate", &uvTransformSprite.rotate.z);
		ImGui::Text("PushKey [DIK_SPACE] = Log [HIT 0]");
		ImGui::End();

		Matrix4x4 cameraMatrix = MakeAffineMatrixMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		////透視射影行列
		projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(winApp->GetClientWidth()) / float(winApp->GetClientHeight()), 0.1f, 100.0f);



		Matrix4x4 worldMatrixObj = MakeAffineMatrixMatrix(transform.scale, transform.rotate, transform.translate);
		Matrix4x4 worldViewProjectionMatrixObj = Multiply(worldMatrixObj, Multiply(viewMatrix, projectionMatrix));
		transformationMatrixDataObj->World = worldViewProjectionMatrixObj;
		transformationMatrixDataObj->WVP = worldViewProjectionMatrixObj;


		//UVTransformMaterial
		Matrix4x4 uvTransformMatrix = MakeScaleMatrix(uvTransformSprite.scale);
		uvTransformMatrix = Multiply(uvTransformMatrix, MakeRotateZMatrix(uvTransformSprite.rotate.z));
		uvTransformMatrix = Multiply(uvTransformMatrix, MakeTranslateMatrix(uvTransformSprite.translate));
		
		//UVTransformMaterial//球
		Matrix4x4 uvTransformMatrixSphar = MakeScaleMatrix(uvTransformSphar.scale);
		uvTransformMatrixSphar = Multiply(uvTransformMatrixSphar, MakeRotateZMatrix(uvTransformSphar.rotate.z));
		uvTransformMatrixSphar = Multiply(uvTransformMatrixSphar, MakeTranslateMatrix(uvTransformSphar.translate));
		materialData->uvTransform = uvTransformMatrixSphar;

		//UVTransformMaterial//球
		Matrix4x4 uvTransformMatrixObj = MakeScaleMatrix(uvTransformObj.scale);
		uvTransformMatrixObj = Multiply(uvTransformMatrixObj, MakeRotateZMatrix(uvTransformObj.rotate.z));
		uvTransformMatrixObj = Multiply(uvTransformMatrixObj, MakeTranslateMatrix(uvTransformObj.translate));
		materialDataObj->uvTransform = uvTransformMatrixObj;

		//開発用UIの処理。実際に開発用のUIを出す場合はここをゲーム固有の処理に置き換える
		ImGui::ShowDemoWindow();

		// 描画前処理
		dxCommon->PreDraw();

		spriteCommon->DrawCommonSetting();


		////------平行光源用------////


		dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource->GetGPUVirtualAddress());

		dxCommon->GetCommandList()->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU);

		sprite->Draw();

		dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResourceObj->GetGPUVirtualAddress());

		dxCommon->GetCommandList()->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU2);
		//トランスフォームMatrixResource
		dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResourceObj->GetGPUVirtualAddress());
		dxCommon->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferViewObj); //VBVを設定

		dxCommon->GetCommandList()->DrawInstanced(UINT(modeldata.vertices.size()), 1, 0, 0);
		
		
		





		//描画後処理
		dxCommon->PostDraw();

	}



	delete input;

	dxCommon->Finalize();
	delete dxCommon;

	/*signatureBlob->Release();
	if (errorBlob) {
		errorBlob->Release();
	}
	pixelShaderBlob->Release();
	vertexShaderBlob->Release();*/

	// WindowsAPIの終了処理
	winApp->Finalize();
	// WindowsAPI解放
	delete winApp;

	delete sprite;

	delete spriteCommon;

	return 0;
}
