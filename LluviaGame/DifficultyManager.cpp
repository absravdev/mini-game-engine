#include "DifficultyManager.h"

float DifficultyManager::GetFallSpeed() const
{
    return BASE_FALL_SPEED + elapsed * FALL_SPEED_INCREMENT;
}

float DifficultyManager::GetSpawnRate() const
{
    return BASE_SPAWN_RATE + elapsed * SPAWN_RATE_INCREMENT;
}

int DifficultyManager::GetMaxSimultaneousLetters() const
{
    int extra = static_cast<int>(elapsed / LETTER_CAP_INTERVAL);
    int value = MIN_MAX_LETTERS + extra;

    if (value < MIN_MAX_LETTERS) value = MIN_MAX_LETTERS;
    if (value > MAX_MAX_LETTERS) value = MAX_MAX_LETTERS;

    return value;
}