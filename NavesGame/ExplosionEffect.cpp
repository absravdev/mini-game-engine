#include "ExplosionEffect.h"

ExplosionEffect::ExplosionEffect(int x, int y, float duration)
    : ttl(duration)
{
    posX = (float)x;
    posY = (float)y;
    displayChar = '*';
    tag = ActorTag::Explosion;
}

void ExplosionEffect::BeginPlay()
{
}

void ExplosionEffect::Tick(float dt)
{
    ttl -= dt;
    if (ttl <= 0.f)
        alive = false;
}
