#pragma once
#include "Objecto.h"

enum class ActorTag
{
    None,
    Player,
    PlayerBullet,
    Enemy,
    Explosion
};

class Actor : public Objeto
{
protected:
    char  displayChar = ' ';
    bool  alive = true;
    ActorTag tag = ActorTag::None;

public:
    virtual ~Actor() = default;

    virtual void BeginPlay() = 0;
    virtual void Tick(float dt) = 0;
    virtual void OnCollision(Actor* other) {}

    char     GetDisplayChar() const { return displayChar; }
    bool     IsAlive()        const { return alive; }
    ActorTag GetTag()         const { return tag; }

    void Destroy() { alive = false; }
};
