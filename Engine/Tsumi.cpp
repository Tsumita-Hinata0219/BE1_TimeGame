#include "Tsumi.h"

Tsumi::Tsumi()
{
	winApp_ = WinApp::GetInstance();
	dcMgr_ = DirectXManager::GetInstance();
	shaderMgr_ = ShaderManager::GetInstance();
	pipeLineMgr_ = PipeLineManager::GetInstance();
	textureMgr_ = TextureManager::GetInstance();
	imGuiMgr_ = ImGuiManager::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	descAlloc_ = DescriptorAllocator::GetInstance();
	descriptorMgr_ = DescriptorManager::GetInstance();
}

/// <summary>
/// インスタンスの取得
/// </summary>
Tsumi* Tsumi::GetInstance() {
	static Tsumi instance;
	return &instance;
}


/// <summary>
/// 初期化処理
/// </summary>
void Tsumi::Initialize() {

	// WinAppの初期化処理
	winApp_->Initialize(L"Shape Conquest");

	// DirectXCommonの初期化処理
	dcMgr_->Initialize();

	// ShaderManagerの初期化処理
	shaderMgr_->Initialize();

	// PipeLineManagerの初期化処理
	pipeLineMgr_->CreatePipeLine();

	// TextureManagerの初期化処理
	textureMgr_->Initialize();

	// ImGuiの初期化処理
	imGuiMgr_->Initialize();

	// Inputの初期化処理
	input_->Initialize();

	// Audioの初期化処理
	audio_->Initialize();

	// DescriptorAllocatorの初期化処理
	descAlloc_->Init();

	// DescriptorManagerの初期化処理
	descriptorMgr_->Init();

	// uvCheckerを読み込んでおく
	textureMgr_->LoadTexture("Texture", "uvChecker.png");
}


/// <summary>
/// 解放処理
/// </summary>
void Tsumi::Finalize() {

	audio_->Finalize();
	imGuiMgr_->Release();
	textureMgr_->Finalize();
	descAlloc_->Finalize();
	descriptorMgr_->Clear();
}


/// <summary>
/// フレームの開始
/// </summary>
void Tsumi::BeginFlame() {

	imGuiMgr_->BeginFrame();
	input_->BeginFrame();
	descAlloc_->BeginFrame();
	descriptorMgr_->BeginFrame();
	pipeLineMgr_->PipeLineReset();
}


/// <summary>
/// フレームの終了
/// </summary>
void Tsumi::EndFlame() {

	imGuiMgr_->EndFrame();
}


/// <summary>
/// メッセージの処理
/// </summary>
bool Tsumi::ProcessMessage() {

	if (winApp_->ProcessMessage()) {
		return true;
	}
	else {

		return false;
	}
}