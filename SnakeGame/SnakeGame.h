#pragma once
#include <vector>
#include "IGame.h"

enum class Direction { Up, Down, Left, Right };

struct Cell
{
    int x = 0;
    int y = 0;

    bool operator==(const Cell& o) const { return x == o.x && y == o.y; }
};

class SnakeGame : public IGame
{
    int height = 0;
    int width = 0;

    int  score = 0;
    int  bestScore = 0;
    bool gameOver = false;

    std::vector<Cell> body;

    Direction dir = Direction::Right;
    Direction nextDir = Direction::Right;

    Cell food;

    float moveTimer = 0.f;

    static constexpr float MOVE_INTERVAL = 0.15f;
    static constexpr int   INITIAL_LENGTH = 4;
    static constexpr int   POINTS_PER_FOOD = 1;

    static constexpr char CHAR_BODY = (char)219;
    static constexpr char CHAR_FOOD = '@';

public:
    SnakeGame(int h, int w);

    void Reset()                                     override;
    void Update(float dt, const InputManager& input) override;
    void Draw(DrawManager& drawManager)              override;
    bool IsGameOver()   const override { return gameOver; }
    int  GetScore()     const override { return score; }
    int  GetBestScore() const override { return bestScore; }

private:
    void ReadDirection(const InputManager& input);
    void MoveSnake();
    void SpawnFood();
    bool IsOccupiedByBody(const Cell& c) const;
    void DrawGameHUD(DrawManager& dm);

    int MinX() const;
    int MaxX() const;
    int MinY() const;
    int MaxY() const;
};