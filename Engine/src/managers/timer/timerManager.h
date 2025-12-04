#pragma once
#include <iostream>
#include <unordered_map>
#include <functional>
#include <chrono>

class TimerManager
{
public:
    ~TimerManager();

    void CreateTimer(const std::string& name, float time, bool bLoop, const std::function<void()>& func);
    void Tick(float deltaTime);

private:
    struct TimerData
    {
        float time;
        float currentTime;
        bool bLoop;
        std::function<void()> func;
    };

    std::unordered_map<std::string, TimerData> timers;
};