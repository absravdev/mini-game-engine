#pragma once
#include <vector>
#include <string>

class NavesGame;

struct EnemySpawnInfo
{
    std::string type;
    char  displayChar = 'G';
    float speed = 3.f;
    int   hp = 1;
    int   points = 10;
    int   yPos = 0;
};

struct WaveConfig
{
    float triggerTime = 0.f;
    std::vector<EnemySpawnInfo> enemies;
};

class WaveManager
{
    std::vector<WaveConfig> waves;
    int   nextWaveIndex = 0;
    float elapsed = 0.f;
    int   cycle = 0;
    float cycleTimeOffset = 0.f;

    static constexpr float SPEED_MULTIPLIER_PER_CYCLE = 0.25f;
    static constexpr int   HP_BONUS_PER_CYCLE = 1;
    static constexpr int   POINTS_BONUS_PER_CYCLE = 5;

public:
    bool LoadFromXML(const std::string& filepath);
    void Reset();
    void Update(float dt, NavesGame* game, int screenWidth);

    int  GetTotalWaves()    const { return (int)waves.size(); }
    int  GetCurrentWave()   const { return nextWaveIndex; }
    int  GetCycle()         const { return cycle; }
};