#pragma once
#include "Actor.h"

class InputManager;
class NavesGame;

class PlayerShip : public Actor
{
    float speed = 10.f;
    float shootInterval = 0.3f;
    float shootTimer = 0.f;

    NavesGame* game = nullptr;
    const InputManager* currentInput = nullptr;

public:
    PlayerShip(NavesGame* game, float speed, float shootInterval, char ch);

    void BeginPlay() override;
    void Tick(float dt) override;
    void OnCollision(Actor* other) override;

    void SetInput(const InputManager* input) { currentInput = input; }

private:
    void HandleMovement(float dt);
    void HandleShooting(float dt);
};
