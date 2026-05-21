#pragma once
#include "Actor.h"
#include <string>

class NavesGame;

class EnemyShip : public Actor
{
    float speed = 3.f;
    int   hp = 1;
    int   points = 10;
    NavesGame* game = nullptr;
    std::string enemyType;

public:
    EnemyShip(NavesGame* game, const std::string& type, float speed, int hp, int points, char ch);

    void BeginPlay() override;
    void Tick(float dt) override;
    void OnCollision(Actor* other) override;

    const std::string& GetEnemyType() const { return enemyType; }
    int GetPoints() const { return points; }
};
