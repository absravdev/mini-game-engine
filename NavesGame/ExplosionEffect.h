#pragma once
#include "Actor.h"

class ExplosionEffect : public Actor
{
    float ttl = 0.25f;

public:
    ExplosionEffect(int x, int y, float duration);

    void BeginPlay() override;
    void Tick(float dt) override;
};
