#pragma once

enum class EffectType
{
    Trail,
    Explosion
};

struct Effect
{
    EffectType type = EffectType::Trail;
    int x = 0;
    int y = 0;
    float ttl = 0.25f;
};