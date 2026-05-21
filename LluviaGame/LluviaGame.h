#pragma once
#include <vector>
#include "IGame.h"
#include "FallingLetter.h"
#include "Effect.h"
#include "DifficultyManager.h"

class LluviaGame : public IGame
{
    int height = 0;
    int width = 0;

    int  score = 0;
    int  bestScore = 0;
    bool gameOver = false;

    float spawnAccumulator = 0.f;
    DifficultyManager difficulty;

    std::vector<FallingLetter> letters;
    std::vector<Effect>        effects;

    static constexpr int   SHIELD_PROBABILITY = 25;
    static constexpr float TRAIL_TTL = 0.35f;
    static constexpr float EXPLOSION_TTL = 0.20f;
    static constexpr int   POINTS_PER_LETTER = 1;

public:
    LluviaGame(int h, int w);

    void Reset()                                     override;
    void Update(float dt, const InputManager& input) override;
    void Draw(DrawManager& drawManager)              override;
    bool IsGameOver()  const override { return gameOver; }
    int  GetScore()    const override { return score; }
    int  GetBestScore() const override { return bestScore; }

private:
    void SpawnLetters(float dt);
    void UpdateLetters(float dt);
    void HandleInput(char key);
    void UpdateEffects(float dt);

    void DamageShieldOrExplode(size_t idx);
    void ExplodeLetterAtIndex(size_t idx);

    bool ReachedBottom(const FallingLetter& l) const;
    char RandomLetter() const;
    bool Chance(int percent) const;

    void DrawGameHUD(DrawManager& dm);
};