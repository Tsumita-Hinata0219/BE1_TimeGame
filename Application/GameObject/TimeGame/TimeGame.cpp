#include "TimeGame.h"


void TimeGame::Init()
{
    nowTimer_ = 0.0f;
    isActive_ = false;
    isOverBorder_ = false;
    hasStopped_ = false;
    score_ = 0;
    scoreSent_ = false;
    rankingFetched_ = false;
    rankingText_ = "";
    fetchingRankingAfterPost_ = false; // 初期化時にフラグをリリセット
}

void TimeGame::Update()
{
    // スペースキーが押された瞬間の処理
    if (Input::GetInstance()->Trigger(DIK_SPACE)) {
        if (!isActive_ && !hasStopped_) {
            // 初期状態 -> スタート
            isActive_ = true;
        }
        else if (isActive_) {
            // 計測中 -> 停止＆スコア判定
            isActive_ = false;
            hasStopped_ = true;
            CalcScore();

            // ゲームが停止したら、まだスコアが送信されていなければ送信を開始します
            if (!scoreSent_) {
                postScoreFuture_ = PostScoreAsync(score_); // スコア送信を非同期で開始
                scoreSent_ = true; // スコア送信済みとしてマーク
                fetchingRankingAfterPost_ = true; // スコア送信後にランキング取得を待機するフラグを立てる
            }
            // ここではすぐにGetAllScoresAsyncを呼び出さない。PostScoreAsyncの完了を待つ。
        }
        else if (hasStopped_) {
            // スコア/ランキング表示中 -> 再スタート（リセット）
            Init();
            return;
        }
    }

    // タイマー進行
    if (isActive_) {
        nowTimer_ += 1.0f / 60.0f;
    }

    // スコア送信が完了したかを確認
    if (scoreSent_ && postScoreFuture_.valid() &&
        postScoreFuture_.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
        std::string response = postScoreFuture_.get();
        // std::cout << "Score Post Response: " << response << std::endl; // デバッグ出力

        // スコア送信が完了したら、すぐにランキングを取得
        if (fetchingRankingAfterPost_ && !rankingFetched_) {
            getScoresFuture_ = GetAllScoresAsync(); // ランキング取得を非同期で開始
            fetchingRankingAfterPost_ = false; // フラグをリセット
        }
    }

    // ランキング取得が完了したかを確認
    // ここではfetchingRankingAfterPost_のフラグは使わない。
    // スコア送信が完了し、かつランキング取得が開始されれば、このブロックで完了を待つ。
    if (!rankingFetched_ && getScoresFuture_.valid() &&
        getScoresFuture_.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
        rankingText_ = getScoresFuture_.get();
        rankingFetched_ = true;
        // std::cout << "Ranking Get Response: " << rankingText_ << std::endl; // デバッグ出力
    }
}

void TimeGame::Draw()
{
    ImGui::Begin("Timer");

    // タイマー表示（開始前、7秒未満、または停止中）
    if (!isActive_ || nowTimer_ < 7.0f || hasStopped_) {
        ImGui::Text("Time: %.2f", nowTimer_);
    }

    // スコア表示と再挑戦案内
    // ゲームが停止している場合に表示
    if (hasStopped_) {
        // スコア表示
        ImGui::Text("Score: %d", score_);

        // 再挑戦案内
        ImGui::Text("Press SPACE to retry");

        // ランキングが取得されていれば表示
        if (rankingFetched_) {
            ImGui::Separator();
            ImGui::Text("--- RANKING ---");

            try {
                nlohmann::json rankingJson = nlohmann::json::parse(rankingText_);
                if (rankingJson.is_array()) {
                    for (int i = 0; i < rankingJson.size(); ++i) {
                        if (rankingJson[i].is_object() && rankingJson[i].contains("score")) {
                            ImGui::Text("%d : %d", i + 1, rankingJson[i]["score"].get<int>());
                        }
                        else {
                            ImGui::Text("%d : Invalid data", i + 1);
                        }
                    }
                }
                else {
                    ImGui::Text("Ranking data is not an array.");
                }
            }
            catch (const nlohmann::json::parse_error& e) {
                ImGui::Text("Failed to parse ranking data: %s", e.what());
            }
            catch (const std::exception& e) {
                ImGui::Text("An error occurred: %s", e.what());
            }
        }
        else {
            // ランキング取得中のメッセージを表示
            ImGui::Separator();
            ImGui::Text("--- RANKING ---");
            ImGui::Text("Fetching ranking...");
        }
    }

    ImGui::End();
}

void TimeGame::CalcScore()
{
    float diff = fabsf(nowTimer_ - 10.0f);

    // 10.01秒以上の場合、ドボン（スコア0）
    if (nowTimer_ >= 10.01f) {
        score_ = 0; // ドボン
    }
    // 10.00秒に近いほど高得点（最大1000点）
    else {
        score_ = static_cast<int>((1.0f - diff) * 1000.0f);

        // 念のため、スコアがマイナスにならないようにします
        if (score_ < 0) {
            score_ = 0;
        }
    }
}