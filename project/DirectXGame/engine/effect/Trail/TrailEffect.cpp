#include "TrailEffect.h"
#include "DirectXGame/engine/3d/Object3d.h"

#include "DirectXGame/engine/MyGame/MyGame.h"

void TrailEffect::Initialize(const std::string& tex,float maxtime  ,const Color color)
{
	mesh = std::make_unique<Mesh>();

	mesh->vertices.push_back({ 0,0,0 });
	mesh->indices.push_back(1);
	mesh->Initialize(TrailEffectManager::GetInstance()->GetDxCommon());

	mesh->indices.clear();
	mesh->vertices.clear();
	mesh->maxTime = maxtime;

	material = std::make_unique<Material>();
	material->Initialize(TrailEffectManager::GetInstance()->GetDxCommon());
	material->tex_.diffuseFilePath = "resources/Texture/Image.png";
	material->tex_.diffuseFilePath = "resources/Texture/aa3.png";
	material->tex_.diffuseFilePath = "resources/Texture/uvChecker.png";
	material->tex_.diffuseFilePath = "resources/Texture/aaa.png";
	material->tex_.diffuseFilePath = tex;

	material->color = color;
	transfomation = std::make_unique<Transfomation>();

	transfomation->Initialize(TrailEffectManager::GetInstance()->GetDxCommon());

	parentTransform_.Identity();

	//parent_->mat_.Identity();

	//transform変数を作る
	//transform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	mat_.Identity();
}

void TrailEffect::Update(bool& flag, const Object3d& strM, const Object3d& endM)
{
	material->GPUData();

	// 頂点バッファの更新
	//mesh->UpdateVertexBuffer();
	//mesh->UpdateIndexBuffer();


	Vector3 str = strM.GetWorldPosition();
	Vector3 end = endM.GetWorldPosition();
	Vector3 strPre = strM.GetPreWorldPosition();
	Vector3 endPre = endM.GetPreWorldPosition();




	Vector3 leftTop		= { str.x , str.y , str.z };
	Vector3 leftBottom	= { end.x , end.y , end.z };
	Vector3 rightTop	= { strPre.x , strPre.y , strPre.z };
	Vector3 rightBottom = { endPre.x , endPre.y , endPre.z };


	if (flag) {
		mesh->vertices.push_back({ .position = {leftTop.x, leftTop.y, leftTop.z, 1.0f}, .texcoord = {0.0f, 0.0f}, .normal = {0.0f, 0.0f, 1.0f} });	// 左上
		mesh->vertices.push_back({ .position = {rightTop.x, rightTop.y, rightTop.z, 1.0f}, .texcoord = {1.0f, 0.0f}, .normal = {0.0f, 0.0f, 1.0f} });	// 右上
		mesh->vertices.push_back({ .position = {leftBottom.x, leftBottom.y, leftBottom.z, 1.0f} ,.texcoord = {0.0f,1.0f},.normal = {0.0f,0.0f,1.0f } });	// 左下
		mesh->vertices.push_back({ .position = {leftBottom.x, leftBottom.y, leftBottom.z, 1.0f} ,.texcoord = {0.0f,1.0f},.normal = {0.0f,0.0f,1.0f } });	// 左下
		mesh->vertices.push_back({ .position = {rightTop.x, rightTop.y, rightTop.z, 1.0f} ,.texcoord = {1.0f,0.0f},.normal = {0.0f,0.0f,1.0f } });	// 右上
		mesh->vertices.push_back({ .position = {rightBottom.x, rightBottom.y, rightBottom.z, 1.0f} ,.texcoord = {1.0f,1.0f},.normal = {0.0f,0.0f,1.0f } });	// 右下


		/*int index = int(mesh->indices.size());

		mesh->indices.push_back(0 + index);
		mesh->indices.push_back(1 + index);
		mesh->indices.push_back(2 + index);
		mesh->indices.push_back(2 + index);
		mesh->indices.push_back(1 + index);
		mesh->indices.push_back(3 + index);*/

		

		// タイマーの初期化
		for (int i = 0; i < 6; ++i) {
			mesh->verticesTimer.push_back({ 0.0f });
		}
		/*for (int i = 0; i < 6; ++i) {
			mesh->indicesTimer.push_back({ 0.0f });
		}*/
	}


	// タイマーの更新
	for (size_t i = 0; i < mesh->verticesTimer.size(); ++i) {
		mesh->verticesTimer[i] += MyGame::GameTime(); // 例としてフレーム時間を加算 (60FPSの想定)
	}
	//for (size_t i = 0; i < mesh->indicesTimer.size(); ++i) {
	//	mesh->indicesTimer[i] += 0.016f; // 例としてフレーム時間を加算 (60FPSの想定)
	//}

	// 時間が経過した頂点を削除
	while (!mesh->verticesTimer.empty() && mesh->verticesTimer.front() >= mesh->maxTime) {
		mesh->vertices.erase(mesh->vertices.begin());
		mesh->verticesTimer.erase(mesh->verticesTimer.begin());
	}
	/*while (!mesh->indicesTimer.empty() && mesh->indicesTimer.front() >= mesh->maxTime) {
		mesh->indices.erase(mesh->indices.begin());
		mesh->indicesTimer.erase(mesh->indicesTimer.begin());
	}*/

	

	// 頂点バッファビューを更新
	mesh->UpdateVertexBuffer();
	//mesh->GenerateIndices2();
	//mesh->UpdateIndexBuffer();



	parentTransform_ = MakeAffineMatrix({ 1,1,1 }, Vector3{ 0,0,0 }, { 0,0,0 });

	// 位置データ
	transfomation->Update(camera_, parentTransform_);

}

void TrailEffect::Draw()
{
	if (mesh->vertices.size() != 0) {
		
		TrailEffectManager::GetInstance()->DrawCommonSetting();

		transfomation->GetCommandList(1);

		material->GetCommandListMaterial(0);

		material->GetCommandListTexture(2, 7, 8);


		mesh->GetCommandList();

		// 描画コマンドの修正：インスタンス数の代わりにインデックス数を使用
		TrailEffectManager::GetInstance()->GetDxCommon()->GetCommandList()->DrawInstanced(UINT(mesh->vertices.size()), 1, 0, 0);

		//TrailEffect::GetInstance()->GetDxCommon()->GetCommandList()->DrawIndexedInstanced(UINT(mesh->indices.size()), 1, 0, 0, 0);
	}
}


