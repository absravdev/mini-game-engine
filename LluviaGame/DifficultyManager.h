#pragma once

class DifficultyManager
{
    float elapsed = 0.f;

    static constexpr float BASE_FALL_SPEED = 2.0f;
    static constexpr float FALL_SPEED_INCREMENT = 0.08f;

    static constexpr float BASE_SPAWN_RATE = 0.6f;
    static constexpr float SPAWN_RATE_INCREMENT = 0.03f;

    static constexpr int   MIN_MAX_LETTERS = 2;
    static constexpr int   MAX_MAX_LETTERS = 12;
    static constexpr float LETTER_CAP_INTERVAL = 15.0f;

public:
    void  Reset() { elapsed = 0.f; }
    void  Update(float dt) { elapsed += dt; }
    float GetElapsed() const { return elapsed; }

    float GetFallSpeed()              const;
    float GetSpawnRate()              const;
    int   GetMaxSimultaneousLetters() const;
};