#pragma once
#include "Actor.h"

class Bullet : public Actor
{
    float speed = 30.f;

public:
    Bullet(float speed, char ch);

    void BeginPlay() override;
    void Tick(float dt) override;
    void OnCollision(Actor* other) override;
};
