#include"../DirectXCommon/Base.h"
#include"../WinApi/WinApi.h"
#include"../WinApi/Msg.h"
#include"../DirectXCommon/DirectX12.h"
#include"../Scene/SceneManager.h"
#include"../Pipeline/PreCreateBasePipeLine.h"
#include"../Pipeline/PreCreateBaseRootSignature.h"
#include"../DirectXCommon/DirectX12Device.h"
#include"../RenderTarget/RenderTargetStatus.h"
#include"../Imgui/MyImgui.h"
#include"../Helper/OutPutDebugStringAndCheckResult.h"
#include"../Input/KeyBoradInputManager.h"
#include"../Input/ControllerInputManager.h"
#include<time.h>


//ターゲットがデバックの時のみコンソール用のmain文を参照する
#ifdef _DEBUG
int main()
#else
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
#endif // DEBUG
{
#ifdef _DEBUG
	//DebugLayer
	CComPtr<ID3D12Debug> spDebugController0;
	CComPtr<ID3D12Debug1> spDebugController1;
	D3D12GetDebugInterface(IID_PPV_ARGS(&spDebugController0));
	spDebugController0->QueryInterface(IID_PPV_ARGS(&spDebugController1));
	spDebugController1->EnableDebugLayer();
	spDebugController1->SetEnableGPUBasedValidation(true);
#endif
	//CheckDirectXError
	int CheckWinError = 0;
	bool CheckMessageFlag = true;
	HRESULT CheckDirectXError = 0;
	

	WinApi winApi;
	Msg msg;
	DirectX12 directX;
	CheckWinError = winApi.CreateMyWindow(WIN_X, WIN_Y);
	if (CheckResult("ウィンドウの生成", CheckWinError)) 
	{
		return 0;
	}
	CheckDirectXError = directX.Create(WIN_X, WIN_Y, winApi.hwnd);
	//CheckDirectXError = S_FALSE;
	if (CheckResult("DirextX12の生成", CheckDirectXError)) 
	{
		return 0;
	}

	

	MyImgui imgui;
	imgui.Create(winApi.hwnd);

	RenderTargetStatus::Instance()->CreateDoubleBuffer(directX.swapchain);

	//もう一度繰り返さないようシングルトンにする
	PreCreateBaseRootSignature prepareR;
	PreCreateBasePipeLine prepareP;

	SceneManager sm;	
	KeyBoradInputManager::Instance()->CreateDevice(&winApi.hwnd, &winApi.window.hInstance);


	OutputDebugStringA("ゲームのメインループを開始します\n");
	srand(static_cast<UINT>(time(NULL)));

	while (CheckMessageFlag)
	{
		CheckMessageFlag = msg.CheckMessage();
		imgui.NewFlame();
		KeyBoradInputManager::Instance()->InputLog();
		ControllerInputManager::Instance()->InputLog();
#ifdef _DEBUG
		winApi.FPS();
#endif

		sm.Update();
		sm.Draw();
		imgui.Set();
		CameraMgr::Instance()->Record();
		RenderTargetStatus::Instance()->SwapResourceBarrier();
		directX.ActCommand();
	}
	winApi.UnregisterWindowClass();



	return 0;
}

