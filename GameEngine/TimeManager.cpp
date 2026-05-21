#include "TimeManager.h"

float TimeManager::GetElapsedTime(void) const
{
    return m_elapsedTime;
}

bool TimeManager::shouldExecuteNextFrame(void)
{
    clock_t currentTimeSinceGameStarted = clock();

    bool isNextFrame = (currentTimeSinceGameStarted - previousFrameTimeSinceGameStarted)
        >= (CLOCKS_PER_SEC * desiredSecondsPerFrame);

    if (isNextFrame)
    {
        m_elapsedTime = (float)(currentTimeSinceGameStarted - previousFrameTimeSinceGameStarted)
            / (float)CLOCKS_PER_SEC;
        previousFrameTimeSinceGameStarted = currentTimeSinceGameStarted;
    }

    return isNextFrame;
}