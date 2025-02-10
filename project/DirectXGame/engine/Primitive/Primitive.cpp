#include "Primitive.h"

#include"PrimitiveCommon.h"

void Primitive::Initialize(ShapeType type, const std::string& tex, const Color color, bool isLine)
{
	
	//std::is_same<T,>


	mesh = std::make_unique<Mesh>();

	type_ = type;

	isLine_ = isLine;

	MeshInitialize();

	mesh->Initialize(PrimitiveCommon::GetInstance()->GetDxCommon());


	if (type == ShapeType::AnimationPlane) {
		mesh->vertices.clear();
		mesh->indices.clear();
	}


	material = std::make_unique<Material>();
	material->Initialize(PrimitiveCommon::GetInstance()->GetDxCommon());
	material->tex_.diffuseFilePath = tex;

	material->color = color;
	transfomation = std::make_unique<Transfomation>();

	transfomation->Initialize(PrimitiveCommon::GetInstance()->GetDxCommon());



	//transform変数を作る
	transform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	mat_.Identity();

	line_ = std::make_unique<LineDraw>();
	line_->Initialize();

}

void Primitive::Update()
{
	material->GPUData();
	material->transform.translate.y += 0.001f;
	mat_ = MakeAffineMatrix(transform.scale, Vector3(transform.rotate), transform.translate);


	//MeshUpdateImGui();

	MeshUpdate();

	transfomation->Update(camera_, mat_);

	line_->SetCamera(camera_);
	line_->SetTransform(transform);
	line_->Update();
}



void Primitive::OnCollision(Collider* other)
{
}

Vector3 Primitive::GetCenterPosition() const
{

	return mat_.GetWorldPosition();

}

void Primitive::MeshInitialize()
{
	switch (type_)
	{
	case Primitive::ShapeType::None:

		break;
	case Primitive::ShapeType::Plane:
		CreatePlane();
		break;
	case Primitive::ShapeType::AnimationPlane:
		//CreateAnimationPlane(true, 2, 5, 0.1f, 3, 1, Vector2{ 1,0 });
		mesh->vertices.push_back({});
		mesh->indices.push_back(0);
		break;
	case Primitive::ShapeType::Triangle:
		CreateTriangle();
		break;
	case Primitive::ShapeType::Circle:
		CreateCircle(2.0f, 16);
		break;
	case Primitive::ShapeType::Star:
		CreateStar(2.0f, 5.0f, 5);
		break;
	case Primitive::ShapeType::Crescent:
		CreateCrescent(12.0f, 16.0f, 8.0f, 16);
		break;
	case Primitive::ShapeType::Ring:
		CreateRing(2.0f, 5.0f, 16);
		break;
	case Primitive::ShapeType::Arrow:
		CreateArrow(16, 4, 8, 8, 16);
		break;
	case Primitive::ShapeType::Cross:
		CreateCross(10, 5);
		break;
	case Primitive::ShapeType::Cube:
		CreateCube({ 1,1,1 });
		break;
	case Primitive::ShapeType::Sphere:
		CreateSphere(3.0f, 16, 16, false);
		break;
	case Primitive::ShapeType::Cylinder:
		CreateCylinder(3.0f, 10.0f, 16);
		break;
	case Primitive::ShapeType::Tube:
		CreateTube(3.0f, 2.0f, 10.0f, 16);
		break;
	case Primitive::ShapeType::Pyramid:
		CreatePyramid(3.0f, 3.0f, 16);
		break;
	case Primitive::ShapeType::Torus:
		CreateTorus(2.0f, 5.0f, 5, 16);
		break;
	case Primitive::ShapeType::Spring:
		CreateSpring(10, 10, 5, 16, 10);
		break;
	default:
		break;
	}
}

void Primitive::MeshUpdate()
{
	oCross_ = cross_;
	oStar = star;
	oCrescent = crescent;
	oRing = ring;
	oSphere = sphere;
	oCylinder = cylinder;
	oPyramid = pyramid;

	switch (type_)
	{
	case Primitive::ShapeType::None:
		break;
	case Primitive::ShapeType::Plane:
		break;
	case Primitive::ShapeType::AnimationPlane:
		CreateAnimationPlane(anime.flag, anime.isLoop, anime.isUV, anime.num, anime.count, anime.interval, anime.width, anime.height, anime.direction);
		if (anime.flag) {
			mesh->UpdateVertexBuffer();
			mesh->UpdateIndexBuffer();
		}

		break;
	case Primitive::ShapeType::Triangle:
		break;
	case Primitive::ShapeType::Circle:

		break;
	case Primitive::ShapeType::Star:
		if ((oStar!= star)) {
			CreateStar(star.innerRadius_, star.outerRadius_, star.segments_);
			mesh->UpdateVertexBuffer();
			mesh->UpdateIndexBuffer();
		}
		break;
	case Primitive::ShapeType::Crescent:
		if ((oCrescent != crescent)) {
			CreateCrescent(crescent.innerRadius_, crescent.outerRadius_, crescent.distance_, crescent.segments_);
			mesh->UpdateVertexBuffer();
			mesh->UpdateIndexBuffer();
		}
		break;
	case Primitive::ShapeType::Ring:
		if ((oRing!= ring)) {
			CreateRing(ring.innerRadius_, ring.outerRadius_, ring.segments_);
			mesh->UpdateVertexBuffer();
			mesh->UpdateIndexBuffer();
		}
		break;
	case Primitive::ShapeType::Arrow:
		break;
	case Primitive::ShapeType::Cross:
		if ((oCross_.armWidth != cross_.armWidth) || (oCross_.armLength != cross_.armLength)) {
			CreateCross(cross_.armLength, cross_.armWidth);
			mesh->UpdateVertexBuffer();
			mesh->UpdateIndexBuffer();
		}
		break;
	case Primitive::ShapeType::Cube:
		if (oCube.size != cube.size) {
			CreateCube(cube.size);
			mesh->UpdateVertexBuffer();
			mesh->UpdateIndexBuffer();
		}
		break;
	case Primitive::ShapeType::Sphere:
		if ((sphere != oSphere)) {
			CreateSphere(sphere.radius_, 16, 16, false);
			mesh->UpdateVertexBuffer();
			mesh->UpdateIndexBuffer();
		}

		break;
	case Primitive::ShapeType::Cylinder:
		if ((oCylinder != cylinder)) {
			CreateCylinder(cylinder.height_, cylinder.radius_, cylinder.segments_);
			mesh->UpdateVertexBuffer();
			mesh->UpdateIndexBuffer();
		}
		break;
	case Primitive::ShapeType::Tube:
		if ((oTube != tube)) {
			CreateTube(tube.radius_, tube.innerRadius_, tube.height_, tube.segments_);
			mesh->UpdateVertexBuffer();
			mesh->UpdateIndexBuffer();
		}
		break;
	case Primitive::ShapeType::Pyramid:
		if ((oPyramid != pyramid)) {
			CreatePyramid(pyramid.radius_, pyramid.height_, pyramid.segments_);
			mesh->UpdateVertexBuffer();
			mesh->UpdateIndexBuffer();
		}
		break;
	case Primitive::ShapeType::Torus:
		if ((torus != oTorus)) {
			CreateTorus(torus.innerRadius_, torus.outerRadius_, torus.tubeSegments_, torus.segments_);
			mesh->UpdateVertexBuffer();
			mesh->UpdateIndexBuffer();
		}
		break;
	case Primitive::ShapeType::Spring:
		if ((oSpring.height != spring.height) || (oSpring.width != spring.width) ||
			(oSpring.spacing != spring.spacing) || (oSpring.length != spring.length) ||
			(oSpring.segments != spring.segments) || (oSpring.turns != spring.turns) || (oSpring.thickness != spring.thickness)) {
			CreateSpring(spring.length, spring.width, spring.height, spring.turns, spring.segments, spring.thickness);
			mesh->UpdateVertexBuffer();
			mesh->UpdateIndexBuffer();
		}
		break;
	default:
		break;
	}
}

void Primitive::MeshUpdateImGui()
{
	ImGui::Begin("Primitive");
	if (ImGui::CollapsingHeader(name_.c_str())) {
		std::string str = name_ + "_translate";
		ImGui::DragFloat3(str.c_str(), &transform.translate.x, 0.1f);
		str = name_ + "_rotate";
		ImGui::DragFloat3(str.c_str(), &transform.rotate.x, 0.01f);
		str = name_ + "_scale";
		ImGui::DragFloat3(str.c_str(), &transform.scale.x, 0.1f);
		int i = (int)mesh->vertices.size();
		ImGui::InputInt("index2", &i);
		switch (type_)
		{
		case Primitive::ShapeType::None:
			break;
		case Primitive::ShapeType::Plane:
			break;
		case Primitive::ShapeType::AnimationPlane:
			oAnime = anime;

			if (ImGui::CollapsingHeader("AnimationPlane")) {
				ImGui::Checkbox("flag", &anime.flag);
				ImGui::Checkbox("isLoop", &anime.isLoop);
				ImGui::Checkbox("isUV", &anime.isUV);
				ImGui::DragInt("num", &anime.num);
				ImGui::DragInt("count", &anime.count);
				ImGui::DragFloat("interval", &anime.interval, 0.01f);
				ImGui::DragFloat("width", &anime.width, 0.01f);
				ImGui::DragFloat("height", &anime.height, 0.01f);
				ImGui::DragFloat2("direction", &anime.direction.x, 0.01f);
			}

			break;
		case Primitive::ShapeType::Triangle:
			break;
		case Primitive::ShapeType::Circle:

			break;
		case Primitive::ShapeType::Star:
			if (ImGui::CollapsingHeader("Star")) {
				ImGui::DragFloat("innerRadius", &star.innerRadius_, 0.1f);
				ImGui::DragFloat("outerRadius", &star.outerRadius_, 0.1f);
				ImGui::DragInt("segments_", &star.segments_);
				if (star.segments_ <= 3) {
					star.segments_ = 3;
				}
			}

			break;
		case Primitive::ShapeType::Crescent:
			if (ImGui::CollapsingHeader("Crescent")) {
				ImGui::DragFloat("innerRadius", &crescent.innerRadius_, 0.1f);
				ImGui::DragFloat("outerRadius", &crescent.outerRadius_, 0.1f);
				ImGui::DragFloat("distance", &crescent.distance_);
				ImGui::DragInt("segments", &crescent.segments_);
				if (crescent.segments_ <= 8) {
					crescent.segments_ = 8;
				}
				if (crescent.distance_ <= 0) {
					crescent.distance_ = 0.0001f;
				}
				if (crescent.innerRadius_ >= crescent.outerRadius_) {
					crescent.innerRadius_ = crescent.outerRadius_;
				}
			}

			break;
		case Primitive::ShapeType::Ring:
			if (ImGui::CollapsingHeader("Ring")) {
				ImGui::DragFloat("innerRadius", &ring.innerRadius_, 0.1f);
				ImGui::DragFloat("outerRadius", &ring.outerRadius_, 0.1f);
				ImGui::DragInt("segments_", &ring.segments_);
				if (ring.segments_ <= 5) {
					ring.segments_ = 5;
				}
			}

			break;
		case Primitive::ShapeType::Arrow:
			break;
		case Primitive::ShapeType::Cross:

			if (ImGui::CollapsingHeader("Cross")) {
				ImGui::DragFloat("armLength", &cross_.armLength, 0.1f);
				ImGui::DragFloat("armWidth", &cross_.armWidth, 0.1f);
			}

			break;
		case Primitive::ShapeType::Cube:
			oCube = cube;

			ImGui::DragFloat3("size", &cube.size.x, 0.1f);

			break;
		case Primitive::ShapeType::Sphere:
			//if (ImGui::CollapsingHeader("Sphere")) {
			ImGui::DragFloat("radius", &sphere.radius_, 0.1f);
			//}

			break;
		case Primitive::ShapeType::Cylinder:
			if (ImGui::CollapsingHeader("Cylinder")) {
				ImGui::DragFloat("height", &cylinder.height_, 0.1f);
				ImGui::DragFloat("radius", &cylinder.radius_, 0.1f);
				ImGui::DragInt("segments", &cylinder.segments_);
				if (cylinder.segments_ <= 5) {
					cylinder.segments_ = 5;
				}
			}

			break;
		case Primitive::ShapeType::Tube:
			if (ImGui::CollapsingHeader("Tube")) {
				ImGui::DragFloat("height", &tube.height_, 0.1f);
				ImGui::DragFloat("radius", &tube.radius_, 0.1f);
				ImGui::DragInt("segments", &tube.segments_);
				if (tube.segments_ <= 3) {
					tube.segments_ = 3;
				}


			}

			break;
		case Primitive::ShapeType::Pyramid:
			if (ImGui::CollapsingHeader("Pyramid")) {
				ImGui::DragFloat("height", &pyramid.height_, 0.1f);
				ImGui::DragFloat("radius", &pyramid.radius_, 0.1f);
				ImGui::DragInt("segments", &pyramid.segments_);
				if (pyramid.segments_ <= 3) {
					pyramid.segments_ = 3;
				}
			}

			break;
		case Primitive::ShapeType::Torus:
			if (ImGui::CollapsingHeader("Torus")) {
				ImGui::DragFloat("innerRadius", &torus.innerRadius_, 0.1f);
				ImGui::DragFloat("outerRadius", &torus.outerRadius_, 0.1f);
				ImGui::DragInt("tubeSegments", &torus.tubeSegments_);
				ImGui::DragInt("segments", &torus.segments_);
				if (torus.segments_ <= 3) {
					torus.segments_ = 3;
				}
				if (torus.tubeSegments_ <= 3) {
					torus.tubeSegments_ = 3;
				}
			}

			break;
		case Primitive::ShapeType::Spring:
			oSpring = spring;

			if (ImGui::CollapsingHeader("Spring")) {
				ImGui::DragFloat("height", &spring.height, 0.1f);
				ImGui::DragFloat("width", &spring.width, 0.1f);
				ImGui::DragFloat("spacing", &spring.spacing, 0.1f);
				ImGui::DragFloat("length", &spring.length, 0.1f);
				ImGui::DragInt("turns", &spring.turns, 0.1f);
				ImGui::DragFloat("thickness", &spring.thickness, 0.1f);
				ImGui::DragInt("segments", &spring.segments);
				if (spring.segments <= 3) {
					spring.segments = 3;
				}

			}
			break;
		default:
			break;
		}
	}
	ImGui::End();
}

void Primitive::Draw()
{
	PrimitiveCommon::GetInstance()->DrawCommonSetting();
	if (mesh->vertices.size() != 0) {


		transfomation->GetCommandList(1);

		material->GetCommandListMaterial(0);

		material->GetCommandListTexture(2, 7, 8);


		mesh->GetCommandList();

		// 描画コマンドの修正：インスタンス数の代わりにインデックス数を使用
		//PrimitiveCommon::GetInstance()->GetDxCommon()->GetCommandList()->DrawInstanced(UINT(mesh->vertices.size()), 1, 0, 0);

		PrimitiveCommon::GetInstance()->GetDxCommon()->GetCommandList()->DrawIndexedInstanced(UINT(mesh->indices.size()), 1, 0, 0, 0);
	}
	if (isLine_)
		line_->DrawMeshLine(mesh.get());
}


/// 2D形

// 平面
void Primitive::CreatePlane()
{

	mesh->vertices.push_back({ .position = {1.0f,1.0f,0.0f,1.0f} ,.texcoord = {1.0f,0.0f},.normal = {0.0f,0.0f,1.0f } });	// 左上
	mesh->vertices.push_back({ .position = {-1.0f,1.0f,0.0f,1.0f} ,.texcoord = {0.0f,0.0f},.normal = {0.0f,0.0f,1.0f } });	// 右上
	mesh->vertices.push_back({ .position = {1.0f,-1.0f,0.0f,1.0f} ,.texcoord = {1.0f,1.0f},.normal = {0.0f,0.0f,1.0f } });	// 左下
	mesh->vertices.push_back({ .position = {-1.0f,-1.0f,0.0f,1.0f} ,.texcoord = {0.0f,1.0f},.normal = {0.0f,0.0f,1.0f } });	// 右下

	mesh->indices.push_back(0);
	mesh->indices.push_back(2);
	mesh->indices.push_back(1);
	mesh->indices.push_back(1);
	mesh->indices.push_back(2);
	mesh->indices.push_back(3);
}

void Primitive::CreateAnimationPlane(bool flag, bool isLoop, bool isUV, int num, int count, float interval, float width, float height, const Vector2& direction) {
	if (!flag) {
		mesh->vertices.clear();
		mesh->indices.clear();
		count_ = 0;
		return;
	}
	if (count_ >= count) {
		if (isLoop) {
			count_ = 0;
			mesh->vertices.clear();
			mesh->indices.clear();
		}
		return;
	}


	// ベースのオフセット
	Vector2 baseOffset = { direction.x * width, direction.y * height };

	static float timer = 1.0f / 60.0f;

	float u;
	float v;
	if (direction.x == 0) {
		u = 1 * width * (count)*num;
	}
	else {
		u = direction.x * width * (count)*num;
	}
	if (direction.y == 0) {
		v = 1 * height * (count)*num;
	}
	else {
		v = direction.y * height * (count)*num;
	}


	if (0 >= timer_) {
		for (int j = 0; j < num; ++j) {
			float offsetX, offsetY;



			if (count_ == 0) {
				if (j == 0) {
					offsetX = baseOffset.x * ((count_) * (num)+j);
					offsetY = baseOffset.y * ((count_) * (num)+j);
				}
				else {
					offsetX = baseOffset.x * (j);
					offsetY = baseOffset.y * (j);
				}
			}
			else {
				offsetX = direction.x * width * ((count_)*num + j);
				offsetY = direction.y * height * ((count_)*num + j);
			}


			Vector2 uv1 = { (offsetX - width) / 2 / u,(offsetY + height) / 2 / v };
			Vector2 uv2 = { (offsetX + width) / 2 / u,(offsetY + height) / 2 / v };
			Vector2 uv3 = { (offsetX + width) / 2 / u,(offsetY - height) / 2 / v };
			Vector2 uv4 = { (offsetX - width) / 2 / u,(offsetY - height) / 2 / v };

			// 頂点データの生成
			if (!isUV) {
				mesh->vertices.push_back({ .position = {offsetX - width / 2.0f, offsetY + height / 2.0f, 0.0f, 1.0f}, .texcoord = {0.0f, 1.0f}, .normal = {0.0f, 0.0f, 1.0f} });
				mesh->vertices.push_back({ .position = {offsetX + width / 2.0f, offsetY + height / 2.0f, 0.0f, 1.0f}, .texcoord = {1.0f, 1.0f}, .normal = {0.0f, 0.0f, 1.0f} });
				mesh->vertices.push_back({ .position = {offsetX + width / 2.0f, offsetY - height / 2.0f, 0.0f, 1.0f}, .texcoord = {1.0f, 0.0f}, .normal = {0.0f, 0.0f, 1.0f} });
				mesh->vertices.push_back({ .position = {offsetX - width / 2.0f, offsetY - height / 2.0f, 0.0f, 1.0f}, .texcoord = {0.0f, 0.0f}, .normal = {0.0f, 0.0f, 1.0f} });
			}
			else {
				mesh->vertices.push_back({ .position = {offsetX - width / 2.0f, offsetY + height / 2.0f, 0.0f, 1.0f}, .texcoord = uv1, .normal = {0.0f, 0.0f, 1.0f} });
				mesh->vertices.push_back({ .position = {offsetX + width / 2.0f, offsetY + height / 2.0f, 0.0f, 1.0f}, .texcoord = uv2, .normal = {0.0f, 0.0f, 1.0f} });
				mesh->vertices.push_back({ .position = {offsetX + width / 2.0f, offsetY - height / 2.0f, 0.0f, 1.0f}, .texcoord = uv3, .normal = {0.0f, 0.0f, 1.0f} });
				mesh->vertices.push_back({ .position = {offsetX - width / 2.0f, offsetY - height / 2.0f, 0.0f, 1.0f}, .texcoord = uv4, .normal = {0.0f, 0.0f, 1.0f} });
			}

			int baseIndex = ((count_) * (num)+j) * 4;

			// インデックスデータの生成
			mesh->indices.push_back(baseIndex + 0);
			mesh->indices.push_back(baseIndex + 1);
			mesh->indices.push_back(baseIndex + 2);
			mesh->indices.push_back(baseIndex + 2);
			mesh->indices.push_back(baseIndex + 3);
			mesh->indices.push_back(baseIndex + 0);
		}
		timer_ = interval;
		count_++;
	}
	timer_ -= timer;
}


// 三角面
void Primitive::CreateTriangle()
{
	mesh->vertices.push_back({ .position = {0.0f,1.0f,0.0f,1.0f} ,.texcoord = {0.5f,0.0f},.normal = {0.0f,0.0f,1.0f } });	// 上
	mesh->vertices.push_back({ .position = {-1.0f,-1.0f,0.0f,1.0f} ,.texcoord = {0.0f,1.0f},.normal = {0.0f,0.0f,1.0f } });	// 右下
	mesh->vertices.push_back({ .position = {1.0f,-1.0f,0.0f,1.0f} ,.texcoord = {1.0f,1.0f},.normal = {0.0f,0.0f,1.0f } });	// 左下

	mesh->indices.push_back(0);
	mesh->indices.push_back(2);
	mesh->indices.push_back(1);
}

// 円
void Primitive::CreateCircle(float radius, int segments)
{
	// 中心の頂点
	mesh->vertices.push_back({
		.position = {0.0f, 0.0f, 0.0f, 1.0f},  // 中心座標
		.texcoord = {0.5f, 0.5f},              // UV中心
		.normal = {0.0f, 0.0f, 1.0f}           // 法線
		});

	// 円周の頂点
	for (int i = 0; i <= segments; ++i) {
		float theta = 2.0f * float(M_PI) * i / segments; // 角度を計算
		float x = radius * cos(theta);
		float y = radius * sin(theta);

		mesh->vertices.push_back({
			.position = {x, y, 0.0f, 1.0f},       // 円周の座標
			.texcoord = {(x / (2.0f * radius) + 0.5f), (y / (2.0f * radius) + 0.5f)}, // UV座標
			.normal = {0.0f, 0.0f, 1.0f}          // 法線
			});
	}

	// インデックスの設定
	for (int i = 1; i <= segments; ++i) {
		mesh->indices.push_back(0);       // 中心点
		mesh->indices.push_back(i + 1);   // 次の頂点
		mesh->indices.push_back(i);       // 現在の頂点
	}
}

// リング(平面)
void Primitive::CreateRing(float innerRadius, float outerRadius, int segments)
{
	if (innerRadius >= outerRadius || segments < 3) {
		throw std::invalid_argument("Invalid parameters: innerRadius must be smaller than outerRadius, and segments must be >= 3.");
	}

	// 既存の頂点とインデックスをクリア
	mesh->vertices.clear();
	mesh->indices.clear();

	// 頂点生成
	for (int i = 0; i <= segments; ++i) {
		float theta = 2.0f * float(M_PI) * i / segments; // 角度を計算
		float cosTheta = cos(theta);
		float sinTheta = sin(theta);

		// 外側の頂点
		mesh->vertices.push_back({
			.position = {outerRadius * cosTheta, outerRadius * sinTheta, 0.0f, 1.0f},  // 外側の座標
			.texcoord = {-cosTheta * 0.5f + 0.5f, -sinTheta * 0.5f + 0.5f},             // UV座標
			.normal = {0.0f, 0.0f, 1.0f}                                             // 法線
			});

		// 内側の頂点
		mesh->vertices.push_back({
			.position = {innerRadius * cosTheta, innerRadius * sinTheta, 0.0f, 1.0f}, // 内側の座標
			.texcoord = {-cosTheta * 0.25f + 0.5f, -sinTheta * 0.25f + 0.5f},           // UV座標
			.normal = {0.0f, 0.0f, 1.0f}                                             // 法線
			});
	}

	// インデックス生成
	for (int i = 0; i < segments; ++i) {
		int outer1 = i * 2;
		int inner1 = outer1 + 1;
		int outer2 = outer1 + 2;
		int inner2 = inner1 + 2;

		// 一つ目の三角形
		mesh->indices.push_back(outer1);
		mesh->indices.push_back(inner1);
		mesh->indices.push_back(outer2);

		// 二つ目の三角形
		mesh->indices.push_back(outer2);
		mesh->indices.push_back(inner1);
		mesh->indices.push_back(inner2);
	}
}

// 星型
void Primitive::CreateStar(float innerRadius, float outerRadius, int segments) {
	if (segments < 3) {
		throw std::invalid_argument("Segments must be >= 3.");
	}

	// 既存の頂点とインデックスをクリア
	mesh->vertices.clear();
	mesh->indices.clear();

	// 中心の頂点を追加
	mesh->vertices.push_back({
		.position = {0.0f, 0.0f, 0.0f, 1.0f}, // 中心の頂点座標
		.texcoord = {0.5f, 0.5f},             // 中心のUV座標
		.normal = {0.0f, 0.0f, -1.0f}         // 法線を逆方向に
		});

	// 頂点生成
	for (int i = 0; i < segments * 2; ++i) {
		float theta = float(M_PI) * i / segments;
		if (segments % 2 != 0) {
			theta -= (float(M_PI) / segments); // 奇数の場合、角度を調整
		}
		float radius = (i % 2 == 0) ? outerRadius : innerRadius;
		float x = radius * cos(theta);
		float y = radius * sin(theta);

		mesh->vertices.push_back({
			.position = {x, y, 0.0f, 1.0f}, // 頂点座標
			.texcoord = {x / (2.0f * outerRadius) + 0.5f, -y / (2.0f * outerRadius) + 0.5f}, // UV座標
			.normal = {0.0f, 0.0f, -1.0f} // 法線を逆方向に
			});
	}

	// インデックス生成
	for (int i = 1; i <= segments * 2; ++i) {
		mesh->indices.push_back(0); // 中心点
		mesh->indices.push_back((i % (segments * 2)) + 1); // 次の頂点（ループ処理）
		mesh->indices.push_back(i);
	}
}

// 三日月	
void Primitive::CreateCrescent(float innerRadius, float outerRadius, float distance, int segments) {
	if (segments < 3 || innerRadius >= outerRadius || distance <= 0) {
		throw std::invalid_argument("Invalid parameters.");
	}

	// 既存の頂点とインデックスをクリア
	mesh->vertices.clear();
	mesh->indices.clear();

	// 外側の円の頂点を生成
	for (int i = 0; i <= segments; ++i) {
		float theta = 2.0f * float(M_PI) * i / segments;
		float x = outerRadius * cos(theta);
		float y = outerRadius * sin(theta);

		mesh->vertices.push_back({
			.position = {x, y, 0.0f, 1.0f},
			.texcoord = {0.5f + 0.5f * cos(theta), 0.5f + 0.5f * sin(theta)},
			.normal = {0.0f, 0.0f, -1.0f} // 法線を逆に設定
			});
	}

	// 内側の円の頂点を生成
	for (int i = 0; i <= segments; ++i) {
		float theta = 2.0f * float(M_PI) * i / segments;
		float x = innerRadius * cos(theta) + distance;
		float y = innerRadius * sin(theta);

		mesh->vertices.push_back({
			.position = {x, y, 0.0f, 1.0f},
			.texcoord = {0.5f + 0.5f * cos(theta), 0.5f + 0.5f * sin(theta)},
			.normal = {0.0f, 0.0f, -1.0f} // 法線を逆に設定
			});
	}

	// クレセント形状のインデックス生成
	int outerOffset = 0;
	int innerOffset = segments + 1;

	for (int i = 0; i < segments; ++i) {
		int next = (i + 1) % (segments + 1);

		// 外側の円の三角形（反転）
		mesh->indices.push_back(innerOffset + i);
		mesh->indices.push_back(outerOffset + next);
		mesh->indices.push_back(outerOffset + i);

		mesh->indices.push_back(innerOffset + i);
		mesh->indices.push_back(innerOffset + next);
		mesh->indices.push_back(outerOffset + next);
	}
}
// 矢印
void Primitive::CreateArrow(float shaftLength, float shaftRadius, float headLength, float headRadius, int segments) {
	if (segments < 3) {
		throw std::invalid_argument("Segments must be >= 3.");
	}

	// 既存の頂点とインデックスをクリア
	mesh->vertices.clear();
	mesh->indices.clear();

	// 矢のシャフトの生成（円柱）
	float halfShaftLength = shaftLength / 2.0f;
	for (int i = 0; i <= segments; ++i) {
		float theta = 2.0f * float(M_PI) * i / segments;
		float x = shaftRadius * cos(theta);
		float z = shaftRadius * sin(theta);

		// 上部の頂点
		mesh->vertices.push_back({
			.position = {x, halfShaftLength, z, 1.0f},
			.texcoord = {static_cast<float>(i) / segments, 0.0f},
			.normal = {x / shaftRadius, 0.0f, z / shaftRadius}
			});

		// 下部の頂点
		mesh->vertices.push_back({
			.position = {x, -halfShaftLength, z, 1.0f},
			.texcoord = {static_cast<float>(i) / segments, 1.0f},
			.normal = {x / shaftRadius, 0.0f, z / shaftRadius}
			});
	}

	// 矢のシャフトのインデックス生成（反転）
	for (int i = 0; i < segments; ++i) {
		int top1 = i * 2;
		int top2 = (i + 1) * 2;
		int bottom1 = i * 2 + 1;
		int bottom2 = (i + 1) * 2 + 1;

		// 三角形を生成
		mesh->indices.push_back(top2);
		mesh->indices.push_back(bottom1);
		mesh->indices.push_back(top1);

		mesh->indices.push_back(bottom2);
		mesh->indices.push_back(bottom1);
		mesh->indices.push_back(top2);
	}

	// 矢の頭の生成（円錐）
	float headBaseY = halfShaftLength;
	float headTipY = headBaseY + headLength;
	for (int i = 0; i <= segments; ++i) {
		float theta = 2.0f * float(M_PI) * i / segments;
		float x = headRadius * cos(theta);
		float z = headRadius * sin(theta);

		// 頂点を追加
		mesh->vertices.push_back({
			.position = {x, headBaseY, z, 1.0f},
			.texcoord = {static_cast<float>(i) / segments, 1.0f},
			.normal = {x / headRadius, 0.0f, z / headRadius}
			});
	}

	// 頭の頂点（先端）
	mesh->vertices.push_back({
		.position = {0.0f, headTipY, 0.0f, 1.0f},
		.texcoord = {0.5f, 0.0f}, // UV座標を先端に集める
		.normal = {0.0f, 1.0f, 0.0f}
		});

	// 矢の頭のインデックス生成（反転）
	int headBaseOffset = (segments + 1) * 2;
	int headTipIndex = (int)mesh->vertices.size() - 1;
	for (int i = 0; i < segments; ++i) {
		int base1 = headBaseOffset + i;
		int base2 = headBaseOffset + i + 1;

		// 頭の三角形を生成

		mesh->indices.push_back(base2);
		mesh->indices.push_back(base1);
		mesh->indices.push_back(headTipIndex);

	}
}
// 十字
void Primitive::CreateCross(float armLength, float armWidth) {
	// 既存の頂点とインデックスをクリア
	mesh->vertices.clear();
	mesh->indices.clear();

	float halfArmLength = armLength / 2.0f;
	float halfArmWidth = armWidth / 2.0f;

	// 頂点データの生成（12頂点）
	mesh->vertices.push_back({ .position = {-halfArmWidth,  halfArmLength, 0.0f, 1.0f}, .texcoord = {0.25f, 0.0f}, .normal = {0.0f, 0.0f, 1.0f} }); // 上左
	mesh->vertices.push_back({ .position = { halfArmWidth,  halfArmLength, 0.0f, 1.0f}, .texcoord = {0.75f, 0.0f}, .normal = {0.0f, 0.0f, 1.0f} }); // 上右
	mesh->vertices.push_back({ .position = { halfArmWidth,  halfArmWidth, 0.0f, 1.0f}, .texcoord = {0.75f, 0.25f}, .normal = {0.0f, 0.0f, 1.0f} }); // 中央上右
	mesh->vertices.push_back({ .position = { halfArmLength, halfArmWidth, 0.0f, 1.0f}, .texcoord = {1.0f, 0.25f}, .normal = {0.0f, 0.0f, 1.0f} }); // 中央右上
	mesh->vertices.push_back({ .position = { halfArmLength, -halfArmWidth, 0.0f, 1.0f}, .texcoord = {1.0f, 0.75f}, .normal = {0.0f, 0.0f, 1.0f} }); // 中央右下
	mesh->vertices.push_back({ .position = { halfArmWidth, -halfArmWidth, 0.0f, 1.0f}, .texcoord = {0.75f, 0.75f}, .normal = {0.0f, 0.0f, 1.0f} }); // 中央下右
	mesh->vertices.push_back({ .position = { halfArmWidth, -halfArmLength, 0.0f, 1.0f}, .texcoord = {0.75f, 1.0f}, .normal = {0.0f, 0.0f, 1.0f} }); // 下右
	mesh->vertices.push_back({ .position = {-halfArmWidth, -halfArmLength, 0.0f, 1.0f}, .texcoord = {0.25f, 1.0f}, .normal = {0.0f, 0.0f, 1.0f} }); // 下左
	mesh->vertices.push_back({ .position = {-halfArmWidth, -halfArmWidth, 0.0f, 1.0f}, .texcoord = {0.25f, 0.75f}, .normal = {0.0f, 0.0f, 1.0f} }); // 中央下左
	mesh->vertices.push_back({ .position = {-halfArmLength, -halfArmWidth, 0.0f, 1.0f}, .texcoord = {0.0f, 0.75f}, .normal = {0.0f, 0.0f, 1.0f} }); // 中央左下
	mesh->vertices.push_back({ .position = {-halfArmLength, halfArmWidth, 0.0f, 1.0f}, .texcoord = {0.0f, 0.25f}, .normal = {0.0f, 0.0f, 1.0f} }); // 中央左上
	mesh->vertices.push_back({ .position = {-halfArmWidth, halfArmWidth, 0.0f, 1.0f}, .texcoord = {0.25f, 0.25f}, .normal = {0.0f, 0.0f, 1.0f} }); // 中央上左

	// インデックスデータの生成
	// 上のアーム
	mesh->indices.push_back(0); mesh->indices.push_back(1); mesh->indices.push_back(2);
	mesh->indices.push_back(0); mesh->indices.push_back(2); mesh->indices.push_back(11);

	// 右のアーム
	mesh->indices.push_back(2); mesh->indices.push_back(3); mesh->indices.push_back(4);
	mesh->indices.push_back(2); mesh->indices.push_back(4); mesh->indices.push_back(5);

	// 下のアーム
	mesh->indices.push_back(5); mesh->indices.push_back(7); mesh->indices.push_back(8);
	mesh->indices.push_back(5); mesh->indices.push_back(6); mesh->indices.push_back(7);

	// 左のアーム
	mesh->indices.push_back(8); mesh->indices.push_back(9); mesh->indices.push_back(10);
	mesh->indices.push_back(8); mesh->indices.push_back(10); mesh->indices.push_back(11);

	// 中央の四角形
	mesh->indices.push_back(2); mesh->indices.push_back(5); mesh->indices.push_back(8);
	mesh->indices.push_back(2); mesh->indices.push_back(8); mesh->indices.push_back(11);
}








// 



///3D形


// 立方体
void Primitive::CreateCube(Vector3 sizes)
{
	// 既存の頂点とインデックスをクリア
	mesh->vertices.clear();
	mesh->indices.clear();

	// 各面の法線
	Vector3 normals[] = {
		{ 0.0f,  0.0f,  1.0f}, // 前面
		{ 0.0f,  0.0f, -1.0f}, // 背面
		{ 0.0f,  1.0f,  0.0f}, // 上面
		{ 0.0f, -1.0f,  0.0f}, // 底面
		{ 1.0f,  0.0f,  0.0f}, // 右側面
		{-1.0f,  0.0f,  0.0f}  // 左側面
	};

	Vector3 size = sizes * 1.0f;

	// 各面の頂点座標 (1つの面に4頂点)
	Vector4 positions[][4] = {
		// 前面
		{{ size.x,  size.y,  size.z, 1.0f}, {-size.x,  size.y,  size.z, 1.0f}, { size.x, -size.y,  size.z, 1.0f}, {-size.x, -size.y,  size.z, 1.0f}},
		// 背面
		{{ size.x,  size.y, -size.z, 1.0f}, { size.x, -size.y, -size.z, 1.0f}, {-size.x,  size.y, -size.z, 1.0f}, {-size.x, -size.y, -size.z, 1.0f}},
		// 上面
		{{ size.x,  size.y, -size.z, 1.0f}, {-size.x,  size.y, -size.z, 1.0f}, { size.x,  size.y,  size.z, 1.0f}, {-size.x,  size.y,  size.z, 1.0f}},
		// 底面
		{{ size.x, -size.y,  size.z, 1.0f}, {-size.x, -size.y,  size.z, 1.0f}, { size.x, -size.y, -size.z, 1.0f}, {-size.x, -size.y, -size.z, 1.0f}},
		// 右側面
		{{ size.x,  size.y, -size.z, 1.0f}, { size.x,  size.y,  size.z, 1.0f}, { size.x, -size.y, -size.z, 1.0f}, { size.x, -size.y,  size.z, 1.0f}},
		// 左側面
		{{-size.x,  size.y,  size.z, 1.0f}, {-size.x,  size.y, -size.z, 1.0f}, {-size.x, -size.y,  size.z, 1.0f}, {-size.x, -size.y, -size.z, 1.0f}}
	};

	// 各面のUV座標
	Vector2 texcoords[] = {
		{0.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}
	};

	// 各面のインデックスオフセット
	int vertexOffset = 0;

	for (int i = 0; i < 6; ++i) { // 6面
		// 頂点データを追加
		for (int j = 0; j < 4; ++j) { // 各面の4頂点
			mesh->vertices.push_back({
				positions[i][j],      // 座標
				texcoords[j],         // UV
				normals[i]            // 法線
				});
		}

		// インデックスデータを追加 (2つの三角形)
		mesh->indices.push_back(vertexOffset + 0);
		mesh->indices.push_back(vertexOffset + 1);
		mesh->indices.push_back(vertexOffset + 2);

		mesh->indices.push_back(vertexOffset + 2);
		mesh->indices.push_back(vertexOffset + 1);
		mesh->indices.push_back(vertexOffset + 3);

		vertexOffset += 4; // 次の面に移動
	}
}

// 球
void Primitive::CreateSphere(float radius, int latitudeSegments, int longitudeSegments, bool isTopBased)
{
	if (latitudeSegments < 2 || longitudeSegments < 3) {
		throw std::invalid_argument("Latitude segments must be >= 2 and longitude segments must be >= 3.");
	}

	// 頂点生成
	for (int lat = 0; lat <= latitudeSegments; ++lat) {
		float theta = float(M_PI) * lat / latitudeSegments; // 緯度（0 ～ π）
		float sinTheta = sin(theta);
		float cosTheta = cos(theta);

		// UV の v 計算（基準切り替え）
		float v = isTopBased ? (1.0f - static_cast<float>(lat) / latitudeSegments)  // 上基準
			: static_cast<float>(lat) / latitudeSegments;         // 下基準

		for (int lon = 0; lon <= longitudeSegments; ++lon) {
			float phi = 2.0f * float(M_PI) * lon / longitudeSegments; // 経度（0 ～ 2π）
			float sinPhi = sin(phi);
			float cosPhi = cos(phi);

			// Y 軸基準の球体座標
			float x = radius * sinTheta * cosPhi;
			float z = radius * sinTheta * sinPhi;
			float y = radius * cosTheta;

			// UV座標
			float u = static_cast<float>(lon) / longitudeSegments;

			// 頂点追加
			mesh->vertices.push_back({
				.position = {x, y, z, 1.0f},   // 頂点座標
				.texcoord = {u, v},            // UV座標
				.normal = {sinTheta * cosPhi, cosTheta, sinTheta * sinPhi} // 法線
				});
		}
	}

	// インデックス生成
	for (int lat = 0; lat < latitudeSegments; ++lat) {
		for (int lon = 0; lon < longitudeSegments; ++lon) {
			int first = (lat * (longitudeSegments + 1)) + lon;
			int second = first + longitudeSegments + 1;

			// 三角形1（インデックス順序を反転）
			mesh->indices.push_back(first + 1);
			mesh->indices.push_back(second);
			mesh->indices.push_back(first);

			// 三角形2（インデックス順序を反転）
			mesh->indices.push_back(first + 1);
			mesh->indices.push_back(second + 1);
			mesh->indices.push_back(second);

		}
	}
}
// 円柱
void Primitive::CreateCylinder(float radius, float height, int segments) {
	if (segments < 3) {
		throw std::invalid_argument("Segments must be >= 3.");
	}
	// 既存の頂点とインデックスをクリア
	mesh->vertices.clear();
	mesh->indices.clear();

	float halfHeight = height / 2.0f;

	// 頂点データの生成
	for (int i = 0; i <= segments; ++i) {
		float theta = 2.0f * float(M_PI) * i / segments;
		float x = radius * cos(theta);
		float z = radius * sin(theta);

		// 上面
		mesh->vertices.push_back({
			.position = {x, halfHeight, z, 1.0f},    // 上面の頂点
			.texcoord = {0.5f + 0.5f * cos(theta),   // UV座標
						 0.5f + 0.5f * -sin(theta)},
			.normal = {0.0f, 1.0f, 0.0f}            // 法線（上方向）
			});

		// 下面
		mesh->vertices.push_back({
			.position = {x, -halfHeight, z, 1.0f},   // 下面の頂点
			.texcoord = {0.5f + 0.5f * cos(theta),   // UV座標
						 0.5f + 0.5f * sin(theta)},
			.normal = {0.0f, -1.0f, 0.0f}           // 法線（下方向）
			});

		// 側面（上）
		mesh->vertices.push_back({
			.position = {x, halfHeight, z, 1.0f},    // 側面上の頂点
			.texcoord = {static_cast<float>(i) / segments, 0.0f}, // UV座標
			.normal = {x / radius, 0.0f, z / radius} // 法線（側面）
			});

		// 側面（下）
		mesh->vertices.push_back({
			.position = {x, -halfHeight, z, 1.0f},   // 側面下の頂点
			.texcoord = {static_cast<float>(i) / segments, 1.0f}, // UV座標
			.normal = {x / radius, 0.0f, z / radius} // 法線（側面）
			});
	}

	// インデックスデータの生成
	// 上面のインデックス
	int centerTop = int(mesh->vertices.size()); // 上面中心
	mesh->vertices.push_back({
		.position = {0.0f, halfHeight, 0.0f, 1.0f}, // 上面中心点
		.texcoord = {0.5f, 0.5f},                  // UV中心
		.normal = {0.0f, 1.0f, 0.0f}              // 法線（上方向）
		});

	for (int i = 0; i < segments; ++i) {
		mesh->indices.push_back((i + 1) * 4);      // 次の上面頂点
		mesh->indices.push_back(i * 4);            // 現在の上面頂点
		mesh->indices.push_back(centerTop);         // 上面中心
	}

	// 下面のインデックス
	int centerBottom = int(mesh->vertices.size()); // 下面中心
	mesh->vertices.push_back({
		.position = {0.0f, -halfHeight, 0.0f, 1.0f}, // 下面中心点
		.texcoord = {0.5f, 0.5f},                   // UV中心
		.normal = {0.0f, -1.0f, 0.0f}              // 法線（下方向）
		});

	for (int i = 0; i < segments; ++i) {
		mesh->indices.push_back(centerBottom);      // 下面中心
		mesh->indices.push_back(i * 4 + 1);        // 現在の下面頂点
		mesh->indices.push_back((i + 1) * 4 + 1);  // 次の下面頂点
	}

	// 側面のインデックス
	for (int i = 0; i < segments; ++i) {
		int top1 = i * 4 + 2;          // 側面上の現在の頂点
		int top2 = (i + 1) * 4 + 2;    // 側面上の次の頂点
		int bottom1 = i * 4 + 3;       // 側面下の現在の頂点
		int bottom2 = (i + 1) * 4 + 3; // 側面下の次の頂点

		// 三角形1
		mesh->indices.push_back(bottom1);
		mesh->indices.push_back(top1);
		mesh->indices.push_back(top2);

		// 三角形2
		mesh->indices.push_back(bottom1);
		mesh->indices.push_back(top2);
		mesh->indices.push_back(bottom2);
	}
}

//　円筒
void Primitive::CreateTube(float radius, float innerRadius, float height, int segments) {
	if (segments < 3) {
		throw std::invalid_argument("Segments must be >= 3.");
	}

	float halfHeight = height / 2.0f;

	// 既存の頂点とインデックスをクリア
	mesh->vertices.clear();
	mesh->indices.clear();

	// 頂点データの生成
	for (int i = 0; i <= segments; ++i) {
		float theta = 2.0f * float(M_PI) * i / segments;
		float x_outer = radius * cos(theta);
		float z_outer = radius * sin(theta);
		float x_inner = innerRadius * cos(theta);
		float z_inner = innerRadius * sin(theta);

		// 外側の頂点
		mesh->vertices.push_back({
			.position = {x_outer, halfHeight, z_outer, 1.0f},
			.texcoord = {static_cast<float>(i) / segments, 0.0f},
			.normal = {x_outer / radius, 0.0f, z_outer / radius}
			});

		mesh->vertices.push_back({
			.position = {x_outer, -halfHeight, z_outer, 1.0f},
			.texcoord = {static_cast<float>(i) / segments, 1.0f},
			.normal = {x_outer / radius, 0.0f, z_outer / radius}
			});

		// 内側の頂点
		mesh->vertices.push_back({
			.position = {x_inner, halfHeight, z_inner, 1.0f},
			.texcoord = {static_cast<float>(i) / segments, 0.0f},
			.normal = {-x_inner / innerRadius, 0.0f, -z_inner / innerRadius} // 法線を逆に
			});

		mesh->vertices.push_back({
			.position = {x_inner, -halfHeight, z_inner, 1.0f},
			.texcoord = {static_cast<float>(i) / segments, 1.0f},
			.normal = {-x_inner / innerRadius, 0.0f, -z_inner / innerRadius} // 法線を逆に
			});
	}

	// 側面のインデックス生成
	for (int i = 0; i < segments; ++i) {
		int topOuter1 = i * 4;          // 外側上の現在の頂点
		int topOuter2 = (i + 1) * 4;    // 外側上の次の頂点
		int bottomOuter1 = i * 4 + 1;   // 外側下の現在の頂点
		int bottomOuter2 = (i + 1) * 4 + 1; // 外側下の次の頂点

		int topInner1 = i * 4 + 2;      // 内側上の現在の頂点
		int topInner2 = (i + 1) * 4 + 2; // 内側上の次の頂点
		int bottomInner1 = i * 4 + 3;   // 内側下の現在の頂点
		int bottomInner2 = (i + 1) * 4 + 3; // 内側下の次の頂点

		// 外側の三角形（反転）
		mesh->indices.push_back(topOuter2);
		mesh->indices.push_back(bottomOuter1);
		mesh->indices.push_back(topOuter1);

		mesh->indices.push_back(topOuter2);
		mesh->indices.push_back(bottomOuter2);
		mesh->indices.push_back(bottomOuter1);

		// 内側の三角形（反転）
		mesh->indices.push_back(bottomInner1);
		mesh->indices.push_back(topInner2);
		mesh->indices.push_back(topInner1);

		mesh->indices.push_back(bottomInner2);
		mesh->indices.push_back(topInner2);
		mesh->indices.push_back(bottomInner1);

		// 外側と内側の三角形（反転）
		mesh->indices.push_back(topInner1);
		mesh->indices.push_back(topOuter2);
		mesh->indices.push_back(topOuter1);

		mesh->indices.push_back(topInner2);
		mesh->indices.push_back(topOuter2);
		mesh->indices.push_back(topInner1);

		mesh->indices.push_back(bottomOuter2);
		mesh->indices.push_back(bottomInner1);
		mesh->indices.push_back(bottomOuter1);

		mesh->indices.push_back(bottomInner2);
		mesh->indices.push_back(bottomInner1);
		mesh->indices.push_back(bottomOuter2);
	}
}
// 角錐
void Primitive::CreatePyramid(float height, float radius, int segments) {
	if (segments < 3) {
		throw std::invalid_argument("Segments must be >= 3.");
	}

	float halfHeight = height / 2.0f;

	// 既存の頂点とインデックスをクリア
	mesh->vertices.clear();
	mesh->indices.clear();

	// 頂点データの生成
	// 頂点0: ピラミッドの頂点（上）
	mesh->vertices.push_back({
		.position = {0.0f, halfHeight, 0.0f, 1.0f},
		.texcoord = {0.5f, 0.5f},  // UV座標の中心を設定
		.normal = {0.0f, 1.0f, 0.0f}
		});

	// 底面の頂点
	for (int i = 0; i < segments; ++i) {
		float theta = 2.0f * float(M_PI) * i / segments;
		float x = radius * cos(theta);
		float z = radius * sin(theta);

		mesh->vertices.push_back({
			.position = {x, -halfHeight, z, 1.0f},
			.texcoord = {0.5f + 0.5f * -cos(theta), 0.5f + 0.5f * sin(theta)}, // UV座標を設定
			.normal = {0.0f, -1.0f, 0.0f}
			});
	}

	// インデックスデータの生成
	for (int i = 1; i <= segments; ++i) {
		int next = (i % segments) + 1;

		// 側面の三角形（反転）
		mesh->indices.push_back(0);   // 頂点（上）
		mesh->indices.push_back(next); // 底面の次の頂点
		mesh->indices.push_back(i);   // 底面の現在の頂点

		// 底面の三角形（反転）
		mesh->indices.push_back(next); // 次の頂点
		mesh->indices.push_back(i);    // 現在の頂点
		mesh->indices.push_back(0);    // 中心点（ここでは利用しない）
	}
}

// トーラス
void Primitive::CreateTorus(float innerRadius, float outerRadius, int tubeSegments, int radialSegments) {
	if (tubeSegments < 3 || radialSegments < 3) {
		throw std::invalid_argument("Segments must be >= 3.");
	}

	// 既存の頂点とインデックスをクリア
	mesh->vertices.clear();
	mesh->indices.clear();

	// 頂点データの生成
	for (int i = 0; i <= tubeSegments; ++i) {
		float theta = 2.0f * float(M_PI) * i / tubeSegments;
		float cosTheta = cos(theta);
		float sinTheta = sin(theta);

		for (int j = 0; j <= radialSegments; ++j) {
			float phi = 2.0f * float(M_PI) * j / radialSegments;
			float cosPhi = cos(phi);
			float sinPhi = sin(phi);

			float x = (outerRadius + innerRadius * cosPhi) * cosTheta;
			float y = (outerRadius + innerRadius * cosPhi) * sinTheta;
			float z = innerRadius * sinPhi;

			mesh->vertices.push_back({
				.position = {x, y, z, 1.0f},
				.texcoord = {static_cast<float>(i) / tubeSegments, static_cast<float>(j) / radialSegments},
				.normal = {cosPhi * cosTheta, cosPhi * sinTheta, sinPhi}
				});
		}
	}

	// インデックスデータの生成
	for (int i = 0; i < tubeSegments; ++i) {
		for (int j = 0; j < radialSegments; ++j) {
			int first = (i * (radialSegments + 1)) + j;
			int second = first + radialSegments + 1;

			mesh->indices.push_back(first);
			mesh->indices.push_back(second);
			mesh->indices.push_back(first + 1);

			mesh->indices.push_back(second);
			mesh->indices.push_back(second + 1);
			mesh->indices.push_back(first + 1);
		}
	}
}

// ばね
void Primitive::CreateSpring(float length, float width, float height, int segments, float spacing) {
	// 既存の頂点とインデックスをクリア
	mesh->vertices.clear();
	mesh->indices.clear();

	float segmentHeight = height / segments;
	float halfWidth = width / 2.0f;
	float angleStep = (2 * float(M_PI)) / segments;

	// 頂点データの生成
	for (int i = 0; i <= segments; ++i) {
		float angle = i * angleStep;
		float xOffset = halfWidth * std::cos(angle);
		float zOffset = halfWidth * std::sin(angle);

		for (float y = 0.0f; y <= length; y += segmentHeight) {
			mesh->vertices.push_back({ .position = {xOffset, y, zOffset, 1.0f}, .texcoord = {0.0f, 0.0f}, .normal = {0.0f, 0.0f, 1.0f} });
			mesh->vertices.push_back({ .position = {xOffset + spacing, y, zOffset, 1.0f}, .texcoord = {1.0f, 0.0f}, .normal = {0.0f, 0.0f, 1.0f} });
			mesh->vertices.push_back({ .position = {xOffset + spacing, y + segmentHeight, zOffset, 1.0f}, .texcoord = {1.0f, 1.0f}, .normal = {0.0f, 0.0f, 1.0f} });
			mesh->vertices.push_back({ .position = {xOffset, y + segmentHeight, zOffset, 1.0f}, .texcoord = {0.0f, 1.0f}, .normal = {0.0f, 0.0f, 1.0f} });
		}

		int baseIndex = i * 4;

		// インデックスデータの生成
		for (int j = 0; j < segments; ++j) {
			mesh->indices.push_back(baseIndex + j * 4 + 0);
			mesh->indices.push_back(baseIndex + j * 4 + 1);
			mesh->indices.push_back(baseIndex + j * 4 + 2);
			mesh->indices.push_back(baseIndex + j * 4 + 2);
			mesh->indices.push_back(baseIndex + j * 4 + 3);
			mesh->indices.push_back(baseIndex + j * 4 + 0);
		}
	}
}




void Primitive::CreateSpring(float length, float width, float height, int turns, int segments, float thickness) {
	// 既存の頂点とインデックスをクリア
	mesh->vertices.clear();
	mesh->indices.clear();

	float segmentHeight = height / segments;
	float halfWidth = width / 2.0f;
	float angleStep = (2 * float(M_PI)) / (turns * 2); // 一巻きごとに2つの頂点を生成

	// 頂点データの生成
	for (int i = 0; i <= turns * 2; ++i) {
		float angle = i * angleStep;
		float xOffset = halfWidth * std::cos(angle);
		float zOffset = halfWidth * std::sin(angle);

		for (float y = 0.0f; y <= length; y += segmentHeight) {
			for (int seg = 0; seg < segments; ++seg) {
				float yOffset = y + seg * segmentHeight;
				float radius = thickness / 2.0f;
				float xInner = xOffset - radius;
				float xOuter = xOffset + radius;
				float zInner = zOffset - radius;
				float zOuter = zOffset + radius;

				mesh->vertices.push_back({ .position = {xInner, yOffset, zInner, 1.0f}, .texcoord = {0.0f, 0.0f}, .normal = {0.0f, 0.0f, 1.0f} });
				mesh->vertices.push_back({ .position = {xOuter, yOffset, zInner, 1.0f}, .texcoord = {1.0f, 0.0f}, .normal = {0.0f, 0.0f, 1.0f} });
				mesh->vertices.push_back({ .position = {xOuter, yOffset + segmentHeight, zInner, 1.0f}, .texcoord = {1.0f, 1.0f}, .normal = {0.0f, 0.0f, 1.0f} });
				mesh->vertices.push_back({ .position = {xInner, yOffset + segmentHeight, zInner, 1.0f}, .texcoord = {0.0f, 1.0f}, .normal = {0.0f, 0.0f, 1.0f} });

				mesh->vertices.push_back({ .position = {xInner, yOffset, zOuter, 1.0f}, .texcoord = {0.0f, 0.0f}, .normal = {0.0f, 0.0f, 1.0f} });
				mesh->vertices.push_back({ .position = {xOuter, yOffset, zOuter, 1.0f}, .texcoord = {1.0f, 0.0f}, .normal = {0.0f, 0.0f, 1.0f} });
				mesh->vertices.push_back({ .position = {xOuter, yOffset + segmentHeight, zOuter, 1.0f}, .texcoord = {1.0f, 1.0f}, .normal = {0.0f, 0.0f, 1.0f} });
				mesh->vertices.push_back({ .position = {xInner, yOffset + segmentHeight, zOuter, 1.0f}, .texcoord = {0.0f, 1.0f}, .normal = {0.0f, 0.0f, 1.0f} });
			}
		}

		int baseIndex = i * segments * 8;

		// インデックスデータの生成
		for (int j = 0; j < segments; ++j) {
			for (int k = 0; k < turns * 2; ++k) {
				mesh->indices.push_back(baseIndex + j * (turns * 2) * 8 + k * 8 + 0);
				mesh->indices.push_back(baseIndex + j * (turns * 2) * 8 + k * 8 + 1);
				mesh->indices.push_back(baseIndex + j * (turns * 2) * 8 + k * 8 + 2);
				mesh->indices.push_back(baseIndex + j * (turns * 2) * 8 + k * 8 + 2);
				mesh->indices.push_back(baseIndex + j * (turns * 2) * 8 + k * 8 + 3);
				mesh->indices.push_back(baseIndex + j * (turns * 2) * 8 + k * 8 + 0);
			}
		}
	}
}

void Primitive::SetCollider()
{
	Collider::Initialize(camera_);


}

void Primitive::SetStar(ShapeParameter::Star& _star)
{
	star = _star;
}


