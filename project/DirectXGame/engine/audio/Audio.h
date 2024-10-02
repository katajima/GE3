#pragma once
#include<cstdint>
#include <xaudio2.h>
#pragma comment(lib,"xaudio2.lib")
#include<fstream>
#include<sstream>
#include<wrl.h>
#include<cassert>

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

// 前方宣言
class WinApp;
class DirectXCommon;


class Audio
{
public:
	// インスタンス
	static Audio* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

	SoundData SoundLoadWave(const char* filename);

	void SoundUnload(SoundData* soundData);

	void SoundPlayWave(IXAudio2* xAudio2, const SoundData& soundData);

	uint32_t PlayWave(uint32_t soundDataHandle);

	uint32_t LoadWave(const char* filename);

private:

	//////------音--------///////

	Microsoft::WRL::ComPtr<IXAudio2> xAudio2;
	IXAudio2MasteringVoice* masterVoice;

	SoundData soundData1;

};

