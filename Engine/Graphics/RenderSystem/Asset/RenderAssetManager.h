#pragma once

#include "../Structure/RenderStructure.h"
#include "../Loader/RenderDataLoader.h"


namespace RenderSystem {

class RenderAssetManager {

private:

	// モデルデータのマップ
	std::map<std::string, Rendering::SceneData> sceneDataMap_;


private: // シングルトン

	// コンストラクタ、デストラクタ
	RenderAssetManager() = default;
	~RenderAssetManager() = default;
	RenderAssetManager(const RenderAssetManager&) = delete;
	const RenderAssetManager& operator=(const RenderAssetManager&) = delete;


public:

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	static RenderAssetManager* GetInstance();

	/// <summary>
	/// ファイルを読み込む
	/// </summary>
	void LoadData(const std::string& path, const std::string fileName);

	/// <summary>
	/// SceneDataの取得
	/// </summary>
	std::optional<Rendering::SceneData> GetSceneData(const std::string& name) const;

	/// <summary>
	/// 指定のモデルデータの破棄
	/// </summary>
	void ModelRemove(std::string name);

	/// <summary>
	/// 全てのモデルデータの破棄
	/// </summary>
	void Clear();

};

}
