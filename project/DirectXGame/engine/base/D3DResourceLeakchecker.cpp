#include"D3DResourceLeakchecker.h"
#include<d3d12.h>
#include<dxgi1_6.h>
#include<dxgidebug.h>
#include<wrl.h>

D3DResourceLeakchecker::~D3DResourceLeakchecker()
{
	//リソースリークチェック
	Microsoft::WRL::ComPtr < IDXGIDebug> debug;

	if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug)))) {
		debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
		debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
		debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
	}
}