#pragma once
#include <vector>

class DrawManager;
class InputManager;

class IGame
{
public:
    virtual ~IGame() = default;

    virtual void Reset() = 0;

    virtual void Update(float dt, const InputManager& input) = 0;

    virtual void Draw(DrawManager& drawManager) = 0;

    virtual bool IsGameOver()  const = 0;
    virtual int  GetScore()    const = 0;
    virtual int  GetBestScore() const = 0;
};