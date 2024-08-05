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

#include"DirectXGame/engine/struct/Structs.h"
#include"DirectXGame/engine/math/MathFanctions.h"
#include"DirectXGame/engine/input/Input.h"
#include"DirectXGame/engine/base/WinApp.h"
#include"DirectXGame/engine/base/DirectXCommon.h"
#include"DirectXGame/engine/base/D3DResourceLeakchecker.h"
#include"DirectXGame/engine/2d/Sprite.h"
#include"DirectXGame/engine/2d/SpriteCommon.h"
#include"DirectXGame/engine/base/TextureManager.h"
#include"DirectXGame/engine/3d/Object3d.h"
#include"DirectXGame/engine/3d/Object3dCommon.h"
#include"DirectXGame/engine/3d/Model.h"
#include"DirectXGame/engine/3d/ModelCommon.h"
#include"DirectXGame/engine/3d/ModelManager.h"
#include"DirectXGame/engine/base/Camera.h"

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

#include"DirectXGame/engine/base/StringUtility.h"
#include"DirectXGame/engine/base/Logger.h"

#include <xaudio2.h>
#pragma comment(lib,"xaudio2.lib")

// チャンクヘッダ
struct ChunkHeader
{
	char id[4]; // チャンク毎のID
	int32_t size; // チャンクサイズ
};


// RIFFヘッダチャンク
struct RiffHeader
{
	ChunkHeader chunk; // "RIFF"
	char type[4];
};

// FMIチャンク
struct FormatChunk
{
	ChunkHeader chunk; // "fmt"
	WAVEFORMATEX fmt; // 波形フォーマット
};

// 音声データ
struct SoundData
{
	// 波形フォーマット
	WAVEFORMATEX wfex;
	// バッファの先頭アドレス
	BYTE* pBuffer;
	// バッファのサイズ
	unsigned int bufferSize;
};

SoundData SoundLoadWave(const char* filename) {

	//HRESULT result;

	// ファイル入力ストリームのインスタンス
	std::ifstream file;
	// .wavファイルをバイナリモードで開く
	file.open(filename, std::ios_base::binary);
	// ファイルオープン失敗を検出
	assert(file.is_open());

	////2
	// RIFFヘッダーの読み込み
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	// ファイルがRIFFかチェック
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		assert(0);
	}
	// タイプがWAVEかチェック
	if (strncmp(riff.type, "WAVE", 4) != 0) {
		assert(0);
	}
	// Formatチャンクの読み込み
	FormatChunk format = {};
	// チャンクヘッダーの確認
	file.read((char*)&format, sizeof(ChunkHeader));
	if (strncmp(format.chunk.id, "fmt ", 4) != 0) {
		assert(0);
	}
	// チャンク本体の読み込み
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	// Dataチャンクの読み込み
	ChunkHeader data;
	file.read((char*)&data, sizeof(data));
	// JUNKチャンクを検出した場合
	if (strncmp(data.id, "JUNK", 4) == 0) {
		// 読み取り位置をJUNKチャンクの終わりまで進める
		file.seekg(data.size, std::ios_base::cur);
		// 再読み込み
		file.read((char*)&data, sizeof(data));
	}

	if (strncmp(data.id, "data", 4) != 0) {
		assert(0);
	}

	// Dataチャンクのデータ部（波形データ）の読み込み
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	// Waveファイルを閉じる
	file.close();


	////4
	// returnする為の音声データ
	SoundData soundData = {};
	soundData.wfex = format.fmt;
	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundData.bufferSize = data.size;

	return soundData;
}

// 音声データ解放
void SoundUnload(SoundData* soundData)
{
	// バッファのメモリを解放
	delete[] soundData->pBuffer;
	soundData->pBuffer = 0;
	soundData->bufferSize = 0;
	soundData->wfex = {};

}

void SoundPlayWave(IXAudio2* xAudio2, const SoundData& soundData) {

	HRESULT result;

	// 波形フォーマットを元にSourceVoiceの生成
	IXAudio2SourceVoice* pSourceVoice = nullptr;
	result = xAudio2->CreateSourceVoice(&pSourceVoice, &soundData.wfex);
	assert(SUCCEEDED(result));

	// 再生する波形データ設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData.pBuffer;
	buf.AudioBytes = soundData.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	// 波形データ再生
	result = pSourceVoice->SubmitSourceBuffer(&buf);
	result = pSourceVoice->Start();
}


//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	D3DResourceLeakchecker leakCheck;

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

	//テクスチャマネージャー
	TextureManager::GetInstance()->Initialize(dxCommon);

	//モデルマネージャー
	ModelManager::GetInstance()->Initialize(dxCommon);

	SpriteCommon* spriteCommon = nullptr;
	// スプライト共通部の初期化
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(dxCommon);

	Camera* camera = new Camera();
	camera->SetRotate({ 0,0,0 });
	camera->SetTranslate({ 0,0,-20 });


	Object3dCommon* object3dCommon = nullptr;
	// 3Dオブジェクト共通部分の初期化
	object3dCommon = new Object3dCommon();
	object3dCommon->Initialize(dxCommon);
	object3dCommon->SetDefaltCamera(camera);

	ModelCommon* modelCommon = nullptr;
	modelCommon = new ModelCommon();
	modelCommon->Initialize(dxCommon);

	std::vector<Sprite*> sprites;
	const int MaxSprite = 1;
	bool isSprite[MaxSprite];

	for (uint32_t i = 0; i < MaxSprite; ++i) {
		Sprite* sprite = new Sprite();
		if (i == 0) {

			sprite->Initialize(spriteCommon, "resources/white.png");
		}
		if (i == 1) {
			sprite->Initialize(spriteCommon, "resources/monsterBall.png");
		}
		else {

			sprite->Initialize(spriteCommon, "resources/uvChecker.png");
		}
		isSprite[i] = true;
		sprites.push_back(sprite);
	}



	const int MaxObject3d = 4;
	bool isModel[MaxObject3d];
	bool isModelImGui[MaxObject3d];
	std::vector<Object3d*> object3ds;
	for (uint32_t i = 0; i < MaxObject3d; ++i) {
		Object3d* object3d = new Object3d();
		object3d->Initialize(object3dCommon);
		//object3d->SetCamera(object3dCommon->GetDefaltCamera());
		if (i == 1) {
			ModelManager::GetInstance()->LoadModel("bunny.obj");
			object3d->SetModel("bunny.obj");
			object3d->SetTranslate({ 4,0,10 });
			object3d->SetRotate({ 0,-3.14f,0 });
			object3d->SetScale({ 1,1,1 });

		}
		else if (i == 2) {
			ModelManager::GetInstance()->LoadModel("teapot.obj");
			object3d->SetModel("teapot.obj");
			object3d->SetTranslate({ -2,0,10 });
			object3d->SetRotate({ 0,0,0 });
			object3d->SetScale({ 1,1,1 });
		}
		else if (i == 3) {
			ModelManager::GetInstance()->LoadModel("multiMesh.obj");
			object3d->SetModel("multiMesh.obj");
			object3d->SetTranslate({ -2,0,10 });
			object3d->SetRotate({ 0,-3.14f,0 });
			object3d->SetScale({ 1,1,1 });
		}
		else {
			ModelManager::GetInstance()->LoadModel("suzanne.obj");
			object3d->SetModel("suzanne.obj");
			object3d->SetTranslate({ -2,0,10 });
			object3d->SetRotate({ 0,-3.14f,0 });
			object3d->SetScale({ 1,1,1 });
		}

		isModel[i] = true;
		isModelImGui[i] = true;

		object3ds.push_back(object3d);
	}

	HRESULT hr;

	//////------音--------///////

	Microsoft::WRL::ComPtr<IXAudio2> xAudio2;
	IXAudio2MasteringVoice* masterVoice;

	// XAudioエンジンのインスタンスを生成
	hr = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	// マスターボイスを生成
	hr = xAudio2->CreateMasteringVoice(&masterVoice);

	// 音声読み込み
	SoundData soundData1 = SoundLoadWave("resources/fanfare.wav");
	// 音声再生
	SoundPlayWave(xAudio2.Get(), soundData1);

	Vector4 colorObj;
	Vector3 directionObj;
	float intensityObj;
	bool isLight = true;
	bool isHalfLambert = true;
	bool isMate = true;
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

#pragma region ImGui

		Vector3 cameraR = camera->GetRotate();
		Vector3 cameraT = camera->GetTranslate();
		ImGui::Begin("Window");
		ImGui::DragFloat3("camera Rotate", &cameraR.x, 0.01f);
		camera->SetRotate(cameraR);
		ImGui::DragFloat3("camera translate", &cameraT.x, 0.1f);
		camera->SetTranslate(cameraT);

		ImGui::Checkbox("model1", &isModel[0]);
		ImGui::Checkbox("model2", &isModel[1]);
		ImGui::Checkbox("model3", &isModel[2]);
		ImGui::Checkbox("model4", &isModel[3]);
		ImGui::Checkbox("sprite", &isSprite[0]);



		Vector2 spritePos = sprites[0]->GetPosition();
		ImGui::DragFloat2("sprite translate", &spritePos.x, 1);
		sprites[0]->SetPosition(spritePos);


		Vector3 translateObj1 = object3ds[0]->GetTranslate();
		Vector3 rotateObj1 = object3ds[0]->GetRotate();
		Vector3 scaleObj1 = object3ds[0]->GetScale();

		if (ImGui::CollapsingHeader("Model 1", isModelImGui[0])) {
			ImGui::DragFloat3("1 translate", &translateObj1.x, 0.1f);
			ImGui::DragFloat3("1 rotate", &rotateObj1.x, 0.01f);
			ImGui::DragFloat3("1 scale", &scaleObj1.x, 0.01f);
			object3ds[0]->SetTranslate(translateObj1);
			object3ds[0]->SetRotate(rotateObj1);
			object3ds[0]->SetScale(scaleObj1);
		}

		Vector3 translateObj2 = object3ds[1]->GetTranslate();
		Vector3 rotateObj2 = object3ds[1]->GetRotate();
		Vector3 scaleObj2 = object3ds[1]->GetScale();
		Vector3 uvTranslate2 = object3ds[1]->GetUvTranslate();
		Vector3 uvRotate2 = object3ds[1]->GetUvRotate();
		Vector3 uvScale2 = object3ds[1]->GetUvScale();
		if (ImGui::CollapsingHeader("Model 2", isModelImGui[1])) {
			ImGui::DragFloat3("2 translate", &translateObj2.x, 0.1f);
			ImGui::DragFloat3("2 rotate", &rotateObj2.x, 0.01f);
			ImGui::DragFloat3("2 scale", &scaleObj2.x, 0.01f);
			ImGui::DragFloat3("2 uv translate", &uvTranslate2.x, 0.01f);
			ImGui::DragFloat3("2 uv rotate", &uvRotate2.x, 0.01f);
			ImGui::DragFloat3("2 uv scale", &uvScale2.x, 0.01f);


			object3ds[1]->SetTranslate(translateObj2);
			object3ds[1]->SetRotate(rotateObj2);
			object3ds[1]->SetScale(scaleObj2);
			object3ds[1]->SetUvTranslate(uvTranslate2);
			object3ds[1]->SetUvRotate(uvRotate2);
			object3ds[1]->SetUvScale(uvScale2);

		}

		Vector3 translateObj3 = object3ds[2]->GetTranslate();
		Vector3 rotateObj3 = object3ds[2]->GetRotate();
		Vector3 scaleObj3 = object3ds[2]->GetScale();
		Vector3 uvTranslate3 = object3ds[2]->GetUvTranslate();
		Vector3 uvRotate3 = object3ds[2]->GetUvRotate();
		Vector3 uvScale3 = object3ds[2]->GetUvScale();
		if (ImGui::CollapsingHeader("Model 3", isModelImGui[2])) {
			ImGui::DragFloat3("3 translate", &translateObj3.x, 0.1f);
			ImGui::DragFloat3("3 rotate", &rotateObj3.x, 0.01f);
			ImGui::DragFloat3("3 scale", &scaleObj3.x, 0.01f);
			ImGui::DragFloat3("3 uv translate", &uvTranslate3.x, 0.01f);
			ImGui::DragFloat3("3 uv rotate", &uvRotate3.x, 0.01f);
			ImGui::DragFloat3("3 uv scale", &uvScale3.x, 0.01f);

			object3ds[2]->SetTranslate(translateObj3);
			object3ds[2]->SetRotate(rotateObj3);
			object3ds[2]->SetScale(scaleObj3);
			object3ds[2]->SetUvTranslate(uvTranslate3);
			object3ds[2]->SetUvRotate(uvRotate3);
			object3ds[2]->SetUvScale(uvScale3);
		}

		Vector3 translateObj4 = object3ds[3]->GetTranslate();
		Vector3 rotateObj4 = object3ds[3]->GetRotate();
		Vector3 scaleObj4 = object3ds[3]->GetScale();
		Vector3 uvTranslate4 = object3ds[3]->GetUvTranslate();
		Vector3 uvRotate4 = object3ds[3]->GetUvRotate();
		Vector3 uvScale4 = object3ds[3]->GetUvScale();

		if (ImGui::CollapsingHeader("Model 4", isModelImGui[3])) {
			ImGui::DragFloat3("4 translate", &translateObj4.x, 0.1f);
			ImGui::DragFloat3("4 rotate", &rotateObj4.x, 0.01f);
			ImGui::DragFloat3("4 scale", &scaleObj4.x, 0.01f);
			ImGui::DragFloat3("4 uv translate", &uvTranslate4.x, 0.01f);
			ImGui::DragFloat3("4 uv rotate", &uvRotate4.x, 0.01f);
			ImGui::DragFloat3("4 uv scale", &uvScale4.x, 0.01f);


			object3ds[3]->SetTranslate(translateObj4);
			object3ds[3]->SetRotate(rotateObj4);
			object3ds[3]->SetScale(scaleObj4);

			object3ds[3]->SetUvTranslate(uvTranslate4);
			object3ds[3]->SetUvRotate(uvRotate4);
			object3ds[3]->SetUvScale(uvScale4);
		}

		if (ImGui::CollapsingHeader("Light", isMate)) {
			for (uint32_t i = 0; i < MaxObject3d; ++i) {

				colorObj = object3ds[i]->GetDirectionalLightColor();
				directionObj = object3ds[i]->GetDirectionalLightDirection();
				intensityObj = object3ds[i]->GetDirectionalLightIntensity();
				isHalfLambert = object3ds[i]->GetHalfLambert();
			}
			ImGui::ColorEdit4("dire color", &colorObj.x);
			ImGui::DragFloat3("dire direction", &directionObj.x, 0.1f);
			directionObj = Nomalize(directionObj);
			ImGui::DragFloat("dire intensity", &intensityObj, 0.01f);
			ImGui::Checkbox("isLight", &(isLight));
			ImGui::Checkbox("isHalfLambert", &(isHalfLambert));
			for (uint32_t i = 0; i < MaxObject3d; ++i) {
				object3ds[i]->SetDirectionalLightColor(colorObj);
				object3ds[i]->SetDirectionalLightDirection(directionObj);
				object3ds[i]->SetDirectionalLightIntensity(intensityObj);
				object3ds[i]->SetLighting(isLight);
				object3ds[i]->SetHalfLambert(isHalfLambert);
			}
		}
		ImGui::End();

#pragma endregion

		// Input
		input->Update();

		camera->Update();

		// 3Dモデル
		for (uint32_t i = 0; i < MaxObject3d; ++i) {

			object3ds[i]->Update();
			object3ds[i]->SetCamera(object3dCommon->GetDefaltCamera());

		}

		// スプライト
		for (uint32_t i = 0; i < MaxSprite; ++i) {
			sprites[i]->Update();

		}





		//開発用UIの処理。実際に開発用のUIを出す場合はここをゲーム固有の処理に置き換える
		ImGui::ShowDemoWindow();

		// 描画前処理
		dxCommon->PreDraw();

		//////////////---------3Dモデル-------------///////////////

		// 3Dオブジェクトの描画準備
		object3dCommon->DrawCommonSetting();

		//3Dオブジェクトの描画
		for (uint32_t i = 0; i < MaxObject3d; ++i) {
			if (isModel[i] == true) {
				object3ds[i]->Draw();
			}
		}


		//////////////--------スプライト-----------///////////////////


		// 2Dオブジェクトの描画準備
		spriteCommon->DrawCommonSetting();


		// 2Dオブジェクトの描画
		for (uint32_t i = 0; i < MaxSprite; ++i) {
			if (isSprite[i] == true) {
				sprites[i]->Draw();
			}
		}

		//描画後処理
		dxCommon->PostDraw();

	}



	delete input;

	dxCommon->Finalize();
	delete dxCommon;

	// WindowsAPIの終了処理
	winApp->Finalize();
	// WindowsAPI解放
	delete winApp;
	for (uint32_t i = 0; i < MaxSprite; ++i) {
		delete sprites[i];
	}
	delete spriteCommon;

	//delete object3d;


	for (uint32_t i = 0; i < MaxObject3d; ++i) {
		delete object3ds[i];
	}
	delete object3dCommon;

	for (uint32_t i = 0; i < MaxObject3d; ++i) {
		//delete models[i];
	}
	/*for (uint32_t i = 0; i < MaxModel; ++i) {
		delete models[i];
	}*/
	delete modelCommon;

	//テクスチャマネージャーの終了
	TextureManager::GetInstance()->Finalize();

	// モデルマネージャーの終了
	ModelManager::GetInstance()->Finalize();

	return 0;
}
