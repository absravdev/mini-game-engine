#pragma once

struct FallingLetter
{
    char  c = 'A';
    int   x = 0;
    float y = 0.f;
    float vy = 2.f;

    int   shieldHits = 0;
    bool  alive = true;
};