#include "WaveManager.h"
#include "NavesGame.h"
#include "EnemyShip.h"
#include <tinyxml2.h>

bool WaveManager::LoadFromXML(const std::string& filepath)
{
    waves.clear();

    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError err = doc.LoadFile(filepath.c_str());
    if (err != tinyxml2::XML_SUCCESS)
        return false;

    tinyxml2::XMLElement* root = doc.FirstChildElement("Waves");
    if (!root)
        return false;

    tinyxml2::XMLElement* waveElem = root->FirstChildElement("Wave");
    while (waveElem)
    {
        WaveConfig wc;
        waveElem->QueryFloatAttribute("time", &wc.triggerTime);

        tinyxml2::XMLElement* enemyElem = waveElem->FirstChildElement("Enemy");
        while (enemyElem)
        {
            EnemySpawnInfo info;

            const char* typeAttr = enemyElem->Attribute("type");
            if (typeAttr) info.type = typeAttr;

            const char* charAttr = enemyElem->Attribute("char");
            if (charAttr && charAttr[0] != '\0') info.displayChar = charAttr[0];

            enemyElem->QueryFloatAttribute("speed", &info.speed);
            enemyElem->QueryIntAttribute("hp", &info.hp);
            enemyElem->QueryIntAttribute("points", &info.points);
            enemyElem->QueryIntAttribute("y", &info.yPos);

            int count = 1;
            enemyElem->QueryIntAttribute("count", &count);

            int spacing = 2;
            enemyElem->QueryIntAttribute("spacing", &spacing);

            for (int i = 0; i < count; i++)
            {
                EnemySpawnInfo copy = info;
                copy.yPos = info.yPos + i * spacing;
                wc.enemies.push_back(copy);
            }

            enemyElem = enemyElem->NextSiblingElement("Enemy");
        }

        waves.push_back(wc);
        waveElem = waveElem->NextSiblingElement("Wave");
    }

    return !waves.empty();
}

void WaveManager::Reset()
{
    nextWaveIndex = 0;
    elapsed = 0.f;
    cycle = 0;
    cycleTimeOffset = 0.f;
}

void WaveManager::Update(float dt, NavesGame* game, int screenWidth)
{
    if (waves.empty()) return;

    elapsed += dt;

    while (nextWaveIndex < (int)waves.size() &&
        elapsed >= cycleTimeOffset + waves[nextWaveIndex].triggerTime)
    {
        const WaveConfig& wc = waves[nextWaveIndex];

        for (const auto& info : wc.enemies)
        {
            float scaledSpeed = info.speed + (SPEED_MULTIPLIER_PER_CYCLE * cycle * info.speed);
            int   scaledHP = info.hp + (HP_BONUS_PER_CYCLE * cycle);
            int   scaledPts = info.points + (POINTS_BONUS_PER_CYCLE * cycle);

            EnemyShip* enemy = new EnemyShip(
                game,
                info.type,
                scaledSpeed,
                scaledHP,
                scaledPts,
                info.displayChar
            );

            float spawnX = (float)(screenWidth - 2);
            float spawnY = (float)(game->GetMinY() + info.yPos);

            if (spawnY > (float)game->GetMaxY())
                spawnY = (float)game->GetMaxY();
            if (spawnY < (float)game->GetMinY())
                spawnY = (float)game->GetMinY();

            enemy->SetPosition(spawnX, spawnY);
            game->AddActor(enemy);
        }

        nextWaveIndex++;
    }

    // olejades acabes, nova ronda més difícil
    if (nextWaveIndex >= (int)waves.size())
    {
        cycle++;
        cycleTimeOffset = elapsed + 3.f;  // pausa de 3s
        nextWaveIndex = 0;
    }
}