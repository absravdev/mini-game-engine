#include "PlayerShip.h"
#include "NavesGame.h"
#include "Bullet.h"
#include "InputManager.h"
#include <windows.h>

PlayerShip::PlayerShip(NavesGame* game, float speed, float shootInterval, char ch)
    : game(game), speed(speed), shootInterval(shootInterval)
{
    displayChar = ch;
    tag = ActorTag::Player;
}

void PlayerShip::BeginPlay()
{
    shootTimer = 0.f;
}

void PlayerShip::Tick(float dt)
{
    HandleMovement(dt);
    HandleShooting(dt);
}

void PlayerShip::OnCollision(Actor* other)
{
    if (other->GetTag() == ActorTag::Enemy)
    {
        game->SetGameOver();
    }
}

void PlayerShip::HandleMovement(float dt)
{
    if (!currentInput) return;

    int minY = game->GetMinY();
    int maxY = game->GetMaxY();

    if (currentInput->IsKeyPressed(VK_UP) || currentInput->IsKeyPressed('W'))
    {
        posY -= speed * dt;
        if (posY < (float)minY) posY = (float)minY;
    }
    if (currentInput->IsKeyPressed(VK_DOWN) || currentInput->IsKeyPressed('S'))
    {
        posY += speed * dt;
        if (posY > (float)maxY) posY = (float)maxY;
    }
}

void PlayerShip::HandleShooting(float dt)
{
    shootTimer += dt;
    if (shootTimer >= shootInterval)
    {
        shootTimer -= shootInterval;

        Bullet* b = new Bullet(30.f, (char)250);
        b->SetPosition(posX + 2.f, posY);
        game->AddActor(b);
    }
}
