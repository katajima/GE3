#pragma once
#include"BaseScene.h"
#include"AbstractSceneFactory.h"

class SceneManager
{
public:
	static SceneManager* instance;

	static SceneManager* GetInstance();
	
	void Finalize();

	void Update();

	void Draw2D();
	
	void Draw3D();
	
	

	~SceneManager();

	void SetSceneFactory(AbstractSceneFactory* sceneFactory) { sceneFactory_ = sceneFactory; };

	/// <summary>
	/// 次のシーン予約
	/// </summary>
	/// <param name="sceneName"></param>
	void ChangeScene(const std::string& sceneName);

private:
	// 今のシーン(実行中シーン)
	BaseScene* scene_ = nullptr;
	// 次のシーン
	BaseScene* nextScene_ = nullptr;

	// シーンファクトリー (借りてくる)
	AbstractSceneFactory* sceneFactory_ = nullptr;
};

