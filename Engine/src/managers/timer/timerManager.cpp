#include "timerManager.h"

TimerManager::~TimerManager()
{
    // No need to join or delete threads, as we're not using threads anymore.
    timers.clear();
}

void TimerManager::CreateTimer(const std::string& name, float time, bool bLoop, const std::function<void()>& func)
{
    timers[name] = {time,time, bLoop, func };
}

void TimerManager::Tick(float deltaTime)
{
    for (auto it = timers.begin(); it != timers.end();)
    {
        auto& timerData = it->second;
        timerData.currentTime -= deltaTime;

        if (timerData.currentTime <= 0)
        {
			timerData.func();

			if (timerData.bLoop)
			{
				timerData.currentTime = timerData.time;
			}
			else
			{
				it = timers.erase(it);
				continue;
			}
        }

        ++it;
    }
}