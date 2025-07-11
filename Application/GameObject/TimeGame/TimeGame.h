#pragma once
#include <cmath>
#include <string>
#include <sstream>
#include <iomanip>
#include <imgui.h>
#include <future>
#include "../GameObject.h"

#include "../Client/Client.h"

class TimeGame {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	TimeGame() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TimeGame() = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

private:

	void CalcScore();


private:
	float nowTimer_ = 0.0f;
	bool isActive_ = false;
	bool isOverBorder_ = false;
	bool hasStopped_ = false;
	int score_ = 0;


	// 非同期処理管理
	bool scoreSent_ = false;   
	bool fetchingRankingAfterPost_ = false;
	std::future<std::string> postScoreFuture_;
	std::future<std::string> getScoresFuture_;

	std::string rankingText_;         
	bool rankingFetched_ = false;      
};

