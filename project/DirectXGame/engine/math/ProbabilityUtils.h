#pragma once
#include <cmath>
#include <math.h>
#include<assert.h>
#include<random>
#include <cstdint>
#include <cstdlib> // for rand() and srand() 
#include <ctime> // for time()
#include <iostream>
#include <vector>
#include <stdexcept>
#include <numeric>

// ランダムエンジンの初期化
static std::random_device seedGenerator;


// 階乗（Factorial）の計算関数
static uint32_t Factorial(uint32_t n) {
    if (n > 0) {
        return n * Factorial(n - 1);
    }
    return 1;
}

// 順列（Permutation）の計算関数
static uint32_t Permutation(uint32_t n, uint32_t k) {
    return Factorial(n) / Factorial(n - k);
}

// 組み合わせ（Combination）の計算関数
static uint32_t Combination(uint32_t n, uint32_t k) {
    return Permutation(n, k) / Factorial(k);
}

// 確率(従属事象)
// 確率(n), 回数(k)
// 1/n + k
static float DependentProbability(uint32_t n, uint32_t k) {
	float num = 0;
	for (uint32_t i = 1; i <= k; i++) {
		num += 1.0f / static_cast<float>(n);
	}
	return num;
}

// 確率(独立事象)
// 確率(n), 回数(k)
// 1/n + k
static float IndependentProbability(uint32_t n, uint32_t k) {
	float num = 0;
	num = powf(1.0f / static_cast<float>(n), static_cast<float>(k));
	return num;
}




// サイコロ(diceNum面)を振る関数 
static int RollDice(uint32_t diceNum) {
    return rand() % diceNum + 1; // 1からdiceNumのランダムな数を返す 
}

// バーンの期待値を計算する関数
// diceNum: サイコロの面の数, totalRolls: ロールの回数
static float ExpectedValue(uint32_t diceNum, int totalRolls) {
    // サイコロ1回分の期待値を計算
    float singleRollExpectedValue = (static_cast<float>(diceNum) + 1) / 2.0f;

    // 合計ロール数を掛けて全体の期待値を計算
    return singleRollExpectedValue * static_cast<float>(totalRolls);
}

// 出目の確率を持つサイコロを振る関数
// probabilities: 各目に対応する確率のベクトル（例: {0.1, 0.2, 0.7}）
// 戻り値: サイコロの出目（1から始まるインデックス）
static int RollBiasedDice(const std::vector<float>& probabilities) {
	// 確率ベクトルのチェック
	if (probabilities.empty()) {
		throw std::invalid_argument("The probabilities vector cannot be empty.");
	}

	// 確率の合計が1かどうかをチェック
	float sum = std::accumulate(probabilities.begin(), probabilities.end(), 0.0f);
	if (sum < 0.999f || sum > 1.001f) { // 許容範囲でチェック
		throw std::invalid_argument("The probabilities must sum to 1.");
	}

	// ランダムエンジンの初期化
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dist(0.0f, 1.0f);

	// 乱数生成
	float randomValue = dist(gen);

	// 確率に基づいて結果を決定
	float cumulativeProbability = 0.0f;
	for (size_t i = 0; i < probabilities.size(); ++i) {
		cumulativeProbability += probabilities[i];
		if (randomValue <= cumulativeProbability) {
			return static_cast<int>(i + 1); // サイコロの目は1から始まる
		}
	}

	// 理論上ここに到達することはないが、安全のため
	return static_cast<int>(probabilities.size());
}

// バーンの期待値を計算する関数
// probabilities: 各目の確率を格納したベクトル
// totalRolls: サイコロを振る回数
static float ExpectedValue(const std::vector<float>& probabilities, int totalRolls) {
	// 確率ベクトルのチェック
	if (probabilities.empty()) {
		throw std::invalid_argument("The probabilities vector cannot be empty.");
	}

	// 確率の合計が1かどうかをチェック
	float sum = std::accumulate(probabilities.begin(), probabilities.end(), 0.0f);
	if (sum < 0.999f || sum > 1.001f) { // 許容範囲でチェック
		throw std::invalid_argument("The probabilities must sum to 1.");
	}

	// 確率を基に単回ロールの期待値を計算
	float singleRollExpectedValue = 0.0f;
	for (size_t i = 0; i < probabilities.size(); ++i) {
		singleRollExpectedValue += (i + 1) * probabilities[i]; // i+1は出目に対応
	}

	// 全体の期待値を計算
	return singleRollExpectedValue * static_cast<float>(totalRolls);
}