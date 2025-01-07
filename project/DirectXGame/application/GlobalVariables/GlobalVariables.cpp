#include "GlobalVariables.h"
#include "Windows.h"
#include "imgui.h"

GlobalVariables* GlobalVariables::GetInstance() {
	static GlobalVariables instance;
	return &instance;
}

void GlobalVariables::CreateGroup(const std::string& groupName) {
	// 指定名のオブジェクトがなければ追加する
	datas_[groupName];
}

void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, int32_t value) {
	// グループの参照を取得
	Group& group = datas_[groupName];
	// 新しい項目のデータを参照
	Item newItem{};
	newItem = value;
	// 設定した項目をstd::mapに追加
	group[key] = newItem;
}

void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, uint32_t value)
{
	// グループの参照を取得
	Group& group = datas_[groupName];
	// 新しい項目のデータを参照
	Item newItem{};
	newItem = value;
	// 設定した項目をstd::mapに追加
	group[key] = newItem;
}

void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, float value) {
	// グループの参照を取得
	Group& group = datas_[groupName];
	// 新しい項目のデータを参照
	Item newItem{};
	newItem = value;
	// 設定した項目をstd::mapに追加
	group[key] = newItem;
}

void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, const Vector2& value)
{
	// グループの参照を取得
	Group& group = datas_[groupName];
	// 新しい項目のデータを参照
	Item newItem{};
	newItem = value;
	// 設定した項目をstd::mapに追加
	group[key] = newItem;
}

void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, const Vector3& value) {
	// グループの参照を取得
	Group& group = datas_[groupName];
	// 新しい項目のデータを参照
	Item newItem{};
	newItem = value;
	// 設定した項目をstd::mapに追加
	group[key] = newItem;
}

void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, const Vector4& value)
{
	// グループの参照を取得
	Group& group = datas_[groupName];
	// 新しい項目のデータを参照
	Item newItem{};
	newItem = value;
	// 設定した項目をstd::mapに追加
	group[key] = newItem;
}

void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, bool value) {
	// グループの参照を取得
	Group& group = datas_[groupName];
	// 新しい項目のデータを参照
	Item newItem{};
	newItem = value;
	// 設定した項目をstd::mapに追加
	group[key] = newItem;
}

int32_t GlobalVariables::GetIntValue(const std::string& groupName, const std::string& key) const {

	// 指定グループが存在する
	assert(datas_.find(groupName) != datas_.end());
	// グループの参照を取得
	const Group& group = datas_.at(groupName);

	// 指定グループに指定のキーが存在する
	assert(group.find(key) != group.end());

	// 指定グループから指定のキーを取得
	return std::get<int32_t>(group.at(key));
}

uint32_t GlobalVariables::GetUintValue(const std::string& groupName, const std::string& key) const
{
	// 指定グループが存在する
	assert(datas_.find(groupName) != datas_.end());
	// グループの参照を取得
	const Group& group = datas_.at(groupName);

	// 指定グループに指定のキーが存在する
	assert(group.find(key) != group.end());

	// 指定グループから指定のキーを取得
	return std::get<uint32_t>(group.at(key));
}

float GlobalVariables::GetFloatValue(const std::string& groupName, const std::string& key) const {
	// 指定グループが存在する
	assert(datas_.find(groupName) != datas_.end());
	// グループの参照を取得
	const Group& group = datas_.at(groupName);

	// 指定グループに指定のキーが存在する
	assert(group.find(key) != group.end());

	// 指定グループから指定のキーを取得
	return std::get<float>(group.at(key));
}

Vector2 GlobalVariables::GetVector2Value(const std::string& groupName, const std::string& key) const
{
	// 指定グループが存在する
	assert(datas_.find(groupName) != datas_.end());
	// グループの参照を取得
	const Group& group = datas_.at(groupName);

	// 指定グループに指定のキーが存在する
	assert(group.find(key) != group.end());

	// 指定グループから指定のキーを取得
	return std::get<Vector2>(group.at(key));
}

Vector3 GlobalVariables::GetVector3Value(const std::string& groupName, const std::string& key) const {

	// 指定グループが存在する
	assert(datas_.find(groupName) != datas_.end());
	// グループの参照を取得
	const Group& group = datas_.at(groupName);

	// 指定グループに指定のキーが存在する
	assert(group.find(key) != group.end());

	// 指定グループから指定のキーを取得
	return std::get<Vector3>(group.at(key));
}

Vector4 GlobalVariables::GetVector4Value(const std::string& groupName, const std::string& key) const
{
	// 指定グループが存在する
	assert(datas_.find(groupName) != datas_.end());
	// グループの参照を取得
	const Group& group = datas_.at(groupName);

	// 指定グループに指定のキーが存在する
	assert(group.find(key) != group.end());

	// 指定グループから指定のキーを取得
	return std::get<Vector4>(group.at(key));
}

bool GlobalVariables::GetBoolValue(const std::string& groupName, const std::string& key) const {

	// 指定グループが存在する
	assert(datas_.find(groupName) != datas_.end());
	// グループの参照を取得
	const Group& group = datas_.at(groupName);

	// 指定グループに指定のキーが存在する
	assert(group.find(key) != group.end());

	// 指定グループから指定のキーを取得
	return std::get<bool>(group.at(key));
}

void GlobalVariables::saveFile(const std::string& groupName) {

	// グループを検索
	std::map<std::string, Group>::iterator itGroup = datas_.find(groupName);
	// 未登録チェック
	assert(itGroup != datas_.end());

	json root;
	root = json::object();

	// jsonオブジェクト登録
	root[groupName] = json::object();

	// 各項目について
	for (std::map<std::string, Item>::iterator itItem = itGroup->second.begin(); itItem != itGroup->second.end(); ++itItem) {
		// 項目名を取得
		const std::string& itemName = itItem->first;
		// 項目の参照を取得
		Item& item = itItem->second;

		// int32_t型の値を保持していれば
		if (std::holds_alternative<int32_t>(item)) {
			root[groupName][itemName] = std::get<int32_t>(item);
		}
		else if (std::holds_alternative<uint32_t>(item)) {
			root[groupName][itemName] = std::get<uint32_t>(item);
		}
		else if (std::holds_alternative<float>(item)) {
			root[groupName][itemName] = std::get<float>(item);
		}
		else if (std::holds_alternative<Vector2>(item)) {
			Vector2 value = std::get<Vector2>(item);
			root[groupName][itemName] = json::array({ value.x, value.y });
		}
		else if (std::holds_alternative<Vector3>(item)) {
			Vector3 value = std::get<Vector3>(item);
			root[groupName][itemName] = json::array({ value.x, value.y, value.z });
		}
		else if (std::holds_alternative<Vector4>(item)) {
			Vector4 value = std::get<Vector4>(item);
			root[groupName][itemName] = json::array({ value.x, value.y, value.z,value.w});
		}
		else if (std::holds_alternative<bool>(item)) {
			root[groupName][itemName] = std::get<bool>(item);
		}

		// ディレクトリがなければ作成する
		std::filesystem::path dir(kDirectoryPath);
		if (!std::filesystem::exists(kDirectoryPath)) {
			std::filesystem::create_directory(kDirectoryPath);
		}

		// 書き込むJSONファイルのフルパスを合成する
		std::string filePath = kDirectoryPath + groupName + ".json";

		// 書き込み用ファイルストリーム
		std::ofstream ofs;
		// ファイルを書き込み用に開く
		ofs.open(filePath);
		// ファイルオープン失敗
		if (ofs.fail()) {
			std::string message = "Failed open data file for write.";
			MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
			assert(0);
			return;
		}

		// ファイルにjson文字列を書き込む(インデント幅4)
		ofs << std::setw(4) << root << std::endl;
		// ファイルを閉じる
		ofs.close();
	}
}

void GlobalVariables::LoadFiles() {

	std::filesystem::path dir(kDirectoryPath);
	// ディレクトリがなければスキップする
	if (!std::filesystem::exists(kDirectoryPath)) {
		return;
	}

	std::filesystem::directory_iterator dir_it(kDirectoryPath);
	for (const std::filesystem::directory_entry& entry : dir_it) {
		// ファイルパスを取得
		const std::filesystem::path& filePath = entry.path();

		// ファイル拡張子を取得
		std::string extension = filePath.extension().string();
		// 　.jsonファイル以外はスキップ
		if (extension.compare(".json") != 0) {
			continue;
		}

		// ファイル読み込み
		LoadFile(filePath.stem().string());
	}
}

void GlobalVariables::LoadFile(const std::string& groupName) {

	// 書き込むJSONファイルのフルパスを合成する
	std::string filePath = kDirectoryPath + groupName + ".json";
	// 読み込み用ファイルストリーム
	std::ifstream ifs;
	// ファイル読み込み用に開く
	ifs.open(filePath);

	// ファイルオープン失敗？
	//   ファイルオープン失敗
	if (ifs.fail()) {
		std::string message = "Failed open data file for write.";
		MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
		assert(0);
		return;
	}

	json root;

	// json文字列からjsonデータ構造に展開
	ifs >> root;
	// ファイルを閉じる
	ifs.close();

	// グループを検索
	json::iterator itGroup = root.find(groupName);

	// 　未登録チェック
	assert(itGroup != root.end());

	// 各アイテムについて
	for (json::iterator itItem = itGroup->begin(); itItem != itGroup->end(); ++itItem) {
		// アイテム名を取得
		const std::string& itemName = itItem.key();

		// int32_t型の値を保持していれば
		if (itItem->is_number_integer()) {
			// intの値を登録
			int32_t value = itItem->get<int32_t>();
			SetValue(groupName, itemName, value);
		}
		// int32_t型の値を保持していれば
		if (itItem->is_number_integer()) {
			// intの値を登録
			uint32_t value = itItem->get<uint32_t>();
			SetValue(groupName, itemName, value);
		}
		if (itItem->is_number_float()) {
			// intの値を登録
			double value = itItem->get<double>();
			SetValue(groupName, itemName, static_cast<float>(value));
		}
		if (itItem->is_array() && itItem->size() == 3) {
			// intの値を登録
			Vector2 value = { itItem->at(0), itItem->at(1) };
			SetValue(groupName, itemName, value);
		}
		if (itItem->is_array() && itItem->size() == 3) {
			// intの値を登録
			Vector3 value = { itItem->at(0), itItem->at(1), itItem->at(2) };
			SetValue(groupName, itemName, value);
		}
		if (itItem->is_array() && itItem->size() == 4) {
			// intの値を登録
			Vector4 value = { itItem->at(0), itItem->at(1), itItem->at(2),itItem->at(3) };
			SetValue(groupName, itemName, value);
		}
		if (itItem->is_boolean()) {
			// intの値を登録
			bool value = itItem->get<bool>();
			SetValue(groupName, itemName, value);
		}
	}
}

void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, int32_t value) {

	std::map<std::string, Group>::iterator itGroup = datas_.find(groupName);
	// 項目が未登録なら
	Group& group = itGroup->second;
	if (group.find(key) == group.end()) {
		SetValue(groupName, key, value);
	}
}

void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, uint32_t value)
{
	std::map<std::string, Group>::iterator itGroup = datas_.find(groupName);
	// 項目が未登録なら
	Group& group = itGroup->second;
	if (group.find(key) == group.end()) {
		SetValue(groupName, key, value);
	}
}

void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, float value) {

	std::map<std::string, Group>::iterator itGroup = datas_.find(groupName);
	// 項目が未登録なら
	Group& group = itGroup->second;
	if (group.find(key) == group.end()) {
		SetValue(groupName, key, value);
	}
}

void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, const Vector2& value)
{
	std::map<std::string, Group>::iterator itGroup = datas_.find(groupName);
	// 項目が未登録なら
	Group& group = itGroup->second;
	if (group.find(key) == group.end()) {
		SetValue(groupName, key, value);
	}
}

void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, const Vector3& value) {

	std::map<std::string, Group>::iterator itGroup = datas_.find(groupName);
	// 項目が未登録なら
	Group& group = itGroup->second;
	if (group.find(key) == group.end()) {
		SetValue(groupName, key, value);
	}
}

void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, const Vector4& value)
{
	std::map<std::string, Group>::iterator itGroup = datas_.find(groupName);
	// 項目が未登録なら
	Group& group = itGroup->second;
	if (group.find(key) == group.end()) {
		SetValue(groupName, key, value);
	}
}

void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, bool value) {

	std::map<std::string, Group>::iterator itGroup = datas_.find(groupName);
	// 項目が未登録なら
	Group& group = itGroup->second;
	if (group.find(key) == group.end()) {
		SetValue(groupName, key, value);
	}
}

//void GlobalVariables::Update() {
//	if (!ImGui::Begin("Global Variables", nullptr, ImGuiWindowFlags_MenuBar)) {
//		ImGui::End();
//		return;
//	}
//	if (ImGui::BeginMenuBar()) {
//		for (auto& [groupName, group] : datas_) {
//			if (ImGui::BeginMenu(groupName.c_str())) {
//
//				// グループ内の項目を vector にコピーしてソート
//				std::vector<std::pair<std::string, Item>> items(group.begin(), group.end());
//
//				// 番号順にソートする
//				std::sort(items.begin(), items.end(), [](const auto& a, const auto& b) {
//					int numA = std::stoi(a.first.substr(10)); // "Translate " の長さは 10
//					int numB = std::stoi(b.first.substr(10));
//					return numA < numB;
//					});
//
//				// ソート済みの項目を表示
//				for (auto& [itemName, item] : items) {
//					if (std::holds_alternative<Vector3>(item)) {
//						Vector3* ptr = std::get_if<Vector3>(&item);
//						ImGui::DragFloat3(itemName.c_str(), reinterpret_cast<float*>(ptr), 0.1f);
//					}
//				}
//
//				ImGui::Text("\n");
//				if (ImGui::Button("Save")) {
//					saveFile(groupName);
//					std::string message = std::format("{}.json saved.", groupName);
//					MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
//				}
//
//				// メニューを閉じる
//				ImGui::EndMenu();
//			}
//		}
//		ImGui::EndMenuBar();
//	}
//	ImGui::End();
//}



void GlobalVariables::Update() {
#ifdef _DEBUG
	if (!ImGui::Begin("Global Variables", nullptr, ImGuiWindowFlags_MenuBar)) {
		ImGui::End();
		return;
	}
	if (!ImGui::BeginMenuBar())
		return;

	for (std::map<std::string, Group>::iterator itGroup_ = datas_.begin(); itGroup_ != datas_.end(); ++itGroup_) {
		// グループ名を取得
		const std::string& groupName = itGroup_->first;
		// グループの参照を取得
		Group& group = itGroup_->second;



		if (!ImGui::BeginMenu(groupName.c_str()))
			continue;
		for (std::map<std::string, Item>::iterator itItem = group.begin(); itItem != group.end(); ++itItem) {
			// 項目名を取得
			const std::string& itemName = itItem->first;
			// 項目の参照を取得
			Item& item = itItem->second;



			if (std::holds_alternative<int32_t>(item)) {
				int32_t* ptr = std::get_if<int32_t>(&item);
				ImGui::DragInt(itemName.c_str(), ptr, 0.01f);
			}
			else if (std::holds_alternative<uint32_t>(item)) {
				
			}
			else if (std::holds_alternative<float>(item)) {
				float* ptr = std::get_if<float>(&item);
				ImGui::DragFloat(itemName.c_str(), ptr, 0.01f);
			}
			else if (std::holds_alternative<Vector2>(item)) {
				Vector2* ptr = std::get_if<Vector2>(&item);
				ImGui::DragFloat2(itemName.c_str(), reinterpret_cast<float*>(ptr), 0.1f);
			}
			else if (std::holds_alternative<Vector3>(item)) {
				Vector3* ptr = std::get_if<Vector3>(&item);
				ImGui::DragFloat3(itemName.c_str(), reinterpret_cast<float*>(ptr), 0.1f);
			}
			else if (std::holds_alternative<Vector4>(item)) {
				Vector4* ptr = std::get_if<Vector4>(&item);
				ImGui::DragFloat4(itemName.c_str(), reinterpret_cast<float*>(ptr), 0.1f);
			}
			else if (std::holds_alternative<bool>(item)) {
				bool* ptr = std::get_if<bool>(&item);
				ImGui::Checkbox(itemName.c_str(), ptr);
			}
		}

		// 改行
		ImGui::Text("\n");
		if (ImGui::Button("Save")) {
			saveFile(groupName);
			std::string message = std::format("{}.json saved.", groupName);
			MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
		}

		ImGui::EndMenu();
	}

	ImGui::EndMenuBar();
	ImGui::End();
#endif
}



