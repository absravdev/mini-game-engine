#pragma once
#include <list>
#include <string>
#include "IGame.h"
#include "Actor.h"
#include "WaveManager.h"

class PlayerShip;

class NavesGame : public IGame
{
    int height = 0;
    int width = 0;

    int  score = 0;
    int  bestScore = 0;
    bool gameOver = false;

    std::list<Actor*> actors;
    std::list<Actor*> pendingAdd;

    PlayerShip* player = nullptr;
    WaveManager waveManager;

    std::string xmlPath;
    std::string recordFilePath;

    static constexpr float PLAYER_SPEED = 12.f;
    static constexpr float PLAYER_SHOOT_INTERVAL = 0.25f;
    static constexpr char  PLAYER_CHAR = '>';

public:
    NavesGame(int h, int w, const std::string& xmlFile = "waves.xml",
        const std::string& recordFile = "naves_record.txt");
    ~NavesGame();

    void Reset()                                     override;
    void Update(float dt, const InputManager& input) override;
    void Draw(DrawManager& drawManager)              override;
    bool IsGameOver()  const override { return gameOver; }
    int  GetScore()    const override { return score; }
    int  GetBestScore() const override { return bestScore; }

    void AddActor(Actor* actor);
    void DestroyActor(Actor* actor);
    void AddScore(int pts);
    void SetGameOver();

    int GetMinY() const;
    int GetMaxY() const;
    int GetMinX() const;
    int GetMaxX() const;

private:
    void DestroyAllActors();
    void FlushPendingActors();
    void CheckCollisions();
    void RemoveDeadActors();
    void DrawGameHUD(DrawManager& dm);

    void LoadRecord();
    void SaveRecord();
};
