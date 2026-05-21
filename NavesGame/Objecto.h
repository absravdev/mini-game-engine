#pragma once

class Objeto
{
protected:
    float posX = 0.f;
    float posY = 0.f;

public:
    virtual ~Objeto() = default;

    float GetX() const { return posX; }
    float GetY() const { return posY; }

    void SetPosition(float x, float y) { posX = x; posY = y; }
    void SetX(float x) { posX = x; }
    void SetY(float y) { posY = y; }

    int GetIntX() const { return (int)posX; }
    int GetIntY() const { return (int)posY; }
};
