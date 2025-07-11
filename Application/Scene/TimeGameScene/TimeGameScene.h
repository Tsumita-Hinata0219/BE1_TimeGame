#pragma once

#include "Scene/IScene.h"
#include "System/GameManager/GameManager.h"

#include "GameObject/TimeGame/TimeGame.h"

class TimeGameScene : public IScene {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	TimeGameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TimeGameScene() = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 背景スプライトの描画処理
	/// </summary>
	void BackSpriteDraw() override;

	/// <summary>
	/// ３Dオブジェクトの描画処理
	/// </summary>
	void ModelDraw() override;

	/// <summary>
	/// 前景スプライトの描画処理
	/// </summary>
	void FrontSpriteDraw() override;

private:

	// カメラ
	CameraManager* cameraManager_ = nullptr;
	std::weak_ptr<CameraData> cameraData_;

	std::unique_ptr<TimeGame> timeGame_;
};

