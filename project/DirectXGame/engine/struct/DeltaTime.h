#pragma once
#include <chrono>
#include <algorithm>

class DeltaTime
{
public:
    DeltaTime()
        : m_deltaTime(0.0f), m_timeScale(1.0f), m_lastTime(std::chrono::high_resolution_clock::now()) {}

    // 更新処理 (フレームごとに呼び出し)
    void Update()
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsed = currentTime - m_lastTime;
        m_deltaTime = elapsed.count() * m_timeScale;  // 時間スケールを適用
        m_lastTime = currentTime;
    }

    // 経過時間を取得 (秒単位)
    float GetSeconds() const { return m_deltaTime; }

    // 経過時間を取得 (ミリ秒単位)
    float GetMilliseconds() const { return m_deltaTime * 1000.0f; }

    // 時間スケールを設定 (通常 1.0f、遅くするには < 1.0f、早くするには > 1.0f)
    void SetTimeScale(float scale) { m_timeScale = (std::max)(0.0f, scale); }

    // 現在の時間スケールを取得
    float GetTimeScale() const { return m_timeScale; }

private:
    float m_deltaTime;  // フレーム間の経過時間（秒単位、float）
    float m_timeScale;  // 時間の倍率 (1.0f = 通常速度, 0.5f = 50%速度, 2.0f = 2倍速)
    std::chrono::high_resolution_clock::time_point m_lastTime;  // 前回の時間記録
};
