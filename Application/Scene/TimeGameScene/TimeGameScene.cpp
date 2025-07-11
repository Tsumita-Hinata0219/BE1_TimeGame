#include "TimeGameScene.h"

TimeGameScene::TimeGameScene()
{
	cameraManager_ = CameraManager::GetInstance();
	timeGame_ = std::make_unique<TimeGame>();
}


/// <summary>
/// 初期化処理
/// </summary>
void TimeGameScene::Initialize()
{
	cameraManager_->ReSet();
	cameraData_ = cameraManager_->GetCameraDataWeak();
	cameraData_.lock()->Init({ 0.15f, 0.0f, 0.0f }, { 0.0f, 4.0f, -20.0f });

	timeGame_->Init();
}


/// <summary>
/// 更新処理
/// </summary>
void TimeGameScene::Update()
{
	cameraData_.lock()->Update();
	cameraData_.lock()->DrawImGui();
	timeGame_->Update();
	timeGame_->Draw();
}


/// <summary>
/// 背景スプライトの描画処理
/// </summary>
void TimeGameScene::BackSpriteDraw()
{
}


/// <summary>
/// ３Dオブジェクトの描画処理
/// </summary>
void TimeGameScene::ModelDraw()
{
}


/// <summary>
/// 前景スプライトの描画処理
/// </summary>
void TimeGameScene::FrontSpriteDraw()
{
}

