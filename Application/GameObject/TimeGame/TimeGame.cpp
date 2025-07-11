#include "TimeGame.h"

void TimeGame::Init()
{
	nowTimer_ = 0.0f;
	isActive_ = false;
	isOverBorder_ = false;
	hasStopped_ = false;
	score_ = 0;
}

void TimeGame::Update()
{
	// スペースキーが押された瞬間
	if (Input::GetInstance()->Trigger(DIK_SPACE)) {
		if (!isActive_ && !hasStopped_) {
			// 初期状態 → スタート
			isActive_ = true;
		}
		else if (isActive_) {
			// 計測中 → 停止＆スコア判定
			isActive_ = false;
			hasStopped_ = true;
			CalcScore();
		}
		else if (hasStopped_) {
			// スコア表示中 → 再スタート（リセット）
			Init();
			return;
		}
	}

	// タイマー進行
	if (isActive_) {
		nowTimer_ += 1.0f / 60.0f;
	}
}

void TimeGame::Draw()
{
	ImGui::Begin("Timer");
	// タイマー表示
	// isActive_がfalse（開始前）、またはnowTimer_が7.0f未満、またはhasStopped_がtrue（停止中）の場合に表示
	if (!isActive_ || nowTimer_ < 7.0f || hasStopped_) {
		// float値を小数点以下2桁まで表示
		ImGui::Text("Time: %.2f", nowTimer_);
	}

	// スコア表示と再挑戦案内
	// ゲームが停止している場合に表示
	if (hasStopped_) {
		// スコア表示
		ImGui::Text("Score: %d", score_);

		// 再挑戦案内
		ImGui::Text("Press SPACE to retry");
	}

	ImGui::End();
}

void TimeGame::CalcScore()
{
	float diff = fabsf(nowTimer_ - 10.0f);

	// 10.01秒以上の場合にドボン
	if (nowTimer_ >= 10.01f) {
		score_ = 0; // ドボン
	}
	// 10.00秒に近いほど高得点（最大1000点）
	else {
		score_ = static_cast<int>((1.0f - diff) * 1000.0f);

		// スコアがマイナスになることを防ぐ（念のため）
		if (score_ < 0) {
			score_ = 0;
		}
	}
}
