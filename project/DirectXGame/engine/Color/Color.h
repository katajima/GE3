#pragma once
#include <iostream>
#include <unordered_map>

struct Color {
    float r, g, b, a;

    // デフォルトコンストラクタ（白に初期化）
};

// 色の名前を列挙型で定義
enum class ColorName {
    WHITE,          // 白
    BLACK,          // 黒
    RED,            // 赤
    GREEN,          // 緑
    BLUE,           // 青
    Silver,         // 銀
    Gray,           // 灰色
    Yellow,         // 黄色
    Orange,         // オレンジ
    Magenta,        // 紫
};

// 色名とRGBA値のマッピング
const std::unordered_map<ColorName, Color> colorMap = {
    {ColorName::WHITE,      Color(1.0f, 1.0f, 1.0f, 1.0f)},// 白
    {ColorName::BLACK,      Color(0.0f, 0.0f, 0.0f, 1.0f)},// 黒
    {ColorName::RED,        Color(1.0f, 0.0f, 0.0f, 1.0f)},// 赤
    {ColorName::GREEN,      Color(0.0f, 1.0f, 0.0f, 1.0f)},// 緑
    {ColorName::BLUE,       Color(0.0f, 0.0f, 1.0f, 1.0f)},// 青
    {ColorName::Silver,     Color(0.753f, 0.753f, 0.753f,1.0f)},// 銀
    {ColorName::Gray,       Color(0.502f, 0.502f, 0.502f,1.0f)},// 灰色
    {ColorName::Yellow,     Color(1.0f, 1.0f, 0.0f,1.0f)},// 黄色
    {ColorName::Orange,     Color(1.0f, 0.549f, 0.0f,1.0f)},// オレンジ
    {ColorName::Magenta,     Color(1.0f, 0.0f, 1.0f,1.0f)},// 紫
    {ColorName::Silver,     Color(0.753f, 0.753f, 0.753f,1.0f)},// 銀
};


// ColorNameからColorを取得する関数
static Color GetColorSet(ColorName name) {
    auto it = colorMap.find(name);
    if (it != colorMap.end()) {
        return it->second;
    }
    return Color();  // デフォルトの白
}

