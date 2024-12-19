#pragma once
#include "DirectXGame/engine/scene/AbstractSceneFactory.h"
#include"TitleScene.h"
#include"GamePlayScene.h"
#include"TestScene.h"


// このゲーム用のシーン工場
class SceneFactory : public AbstractSceneFactory 
{
public:

	/// <summary>
	/// シーン生成
	/// </summary>
	/// <param name="sceneName"></param>
	/// <returns></returns>
	BaseScene* CreateScene(const std::string& sceneName) override;

};

