#include "EnemyShip.h"
#include "NavesGame.h"
#include "ExplosionEffect.h"

EnemyShip::EnemyShip(NavesGame* game, const std::string& type,
    float speed, int hp, int points, char ch)
    : game(game), enemyType(type), speed(speed), hp(hp), points(points)
{
    displayChar = ch;
    tag = ActorTag::Enemy;
}

void EnemyShip::BeginPlay()
{
}

void EnemyShip::Tick(float dt)
{
    posX -= speed * dt;

    if (GetIntX() <= game->GetMinX() + 2)
    {
        alive = false;
        game->SetGameOver();
    }
}

void EnemyShip::OnCollision(Actor* other)
{
    if (other->GetTag() == ActorTag::PlayerBullet)
    {
        hp--;
        if (hp <= 0)
        {
            alive = false;
            game->AddScore(points);

            for (int dy = -1; dy <= 1; dy++)
                for (int dx = -1; dx <= 1; dx++)
                {
                    ExplosionEffect* exp = new ExplosionEffect(GetIntX() + dx, GetIntY() + dy, 0.25f);
                    game->AddActor(exp);
                }
        }
    }
}