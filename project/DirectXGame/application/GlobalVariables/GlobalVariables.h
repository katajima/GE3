#pragma once
#include"DirectXGame/engine/math/MathFanctions.h"
#include <iostream>
#include <fstream>
#include <variant>
#include <string>
#include <map>
#include<json.hpp>

/// <summary>
/// グローバル変数
/// </summary>
class GlobalVariables {
private:
	//項目
	using Item = std::variant<int32_t,uint32_t, float, Vector2, Vector3,Vector4 ,bool>;

	//グループ
	using Group = std::map<std::string, Item>;

public:
	static GlobalVariables* GetInstance();

	/// <summary>
	/// グループの作成
	/// </summary>
	/// <param name="groupName">グループ名</param>
	void CreateGroup(const std::string& groupName);

	//
	// 値のセット(int)
	void SetValue(const std::string& groupName, const std::string& key, int32_t value);
	// 値のセット(uint)
	void SetValue(const std::string& groupName, const std::string& key, uint32_t value);
	// 値のセット(float)
	void SetValue(const std::string& groupName, const std::string& key, float value);
	// 値のセット(Vector2)
	void SetValue(const std::string& groupName, const std::string& key, const Vector2& value);
	// 値のセット(Vector3)
	void SetValue(const std::string& groupName, const std::string& key, const Vector3& value);
	// 値のセット(Vector4)
	void SetValue(const std::string& groupName, const std::string& key, const Vector4& value);
	// 値のセット(bool)
	void SetValue(const std::string& groupName, const std::string& key, bool value);

	// 値の取得
	int32_t GetIntValue(const std::string& groupName, const std::string& key) const;
	uint32_t GetUintValue(const std::string& groupName, const std::string& key) const;
	float GetFloatValue(const std::string& groupName, const std::string& key) const;
	Vector2 GetVector2Value(const std::string& groupName, const std::string& key) const;
	Vector3 GetVector3Value(const std::string& groupName, const std::string& key) const;
	Vector4 GetVector4Value(const std::string& groupName, const std::string& key) const;
	bool GetBoolValue(const std::string& groupName, const std::string& key) const;



	
	/// <summary>
	/// ファイルに書き出し
	/// </summary>
	/// <param name="groupName"></param>
	void saveFile(const std::string& groupName);
	
	/// <summary>
	/// ディレクトリの全ファイル読み込み
	/// </summary>
	void LoadFiles();

	/// <summary>
	/// ファイルから読み込む
	/// </summary>
	/// <param name="groupName"></param>
	void LoadFile(const std::string& groupName);

	// 項目の追加(int)
	void AddItem(const std::string& groupName, const std::string& key, int32_t value);
	// 項目の追加(uint)
	void AddItem(const std::string& groupName, const std::string& key, uint32_t value);
	// 項目の追加(float)
	void AddItem(const std::string& groupName, const std::string& key, float value);
	// 項目の追加(Vector2)
	void AddItem(const std::string& groupName, const std::string& key, const Vector2& value);
	// 項目の追加(Vector3)
	void AddItem(const std::string& groupName, const std::string& key, const Vector3& value);
	// 項目の追加(Vector4)
	void AddItem(const std::string& groupName, const std::string& key, const Vector4& value);
	// 項目の追加(bool)
	void AddItem(const std::string& groupName, const std::string& key, bool value);
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

private:
	GlobalVariables() = default;
	~GlobalVariables() = default;
	GlobalVariables(const GlobalVariables &global) = default;
	GlobalVariables& operator=(const GlobalVariables& global) = default;




	// 全データ
	std::map<std::string, Group> datas_;
	std::map<std::string, std::vector<std::string>> groupKeys_; // 順序を保持するためのリスト

	//グローバル変数の保存先
	const std::string kDirectoryPath = "resources/GlobalVariables/";

	

	using json = nlohmann::json;
};
