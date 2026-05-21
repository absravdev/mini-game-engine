#include "Bullet.h"

Bullet::Bullet(float speed, char ch)
    : speed(speed)
{
    displayChar = ch;
    tag = ActorTag::PlayerBullet;
}

void Bullet::BeginPlay()
{
}

void Bullet::Tick(float dt)
{
    posX += speed * dt;
}

void Bullet::OnCollision(Actor* other)
{
    if (other->GetTag() == ActorTag::Enemy)
    {
        alive = false;
    }
}
