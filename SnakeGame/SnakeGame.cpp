#include "SnakeGame.h"
#include "DrawManager.h"
#include "InputManager.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <windows.h>

SnakeGame::SnakeGame(int h, int w) : height(h), width(w)
{
    static bool seeded = false;
    if (!seeded)
    {
        std::srand((unsigned int)std::time(nullptr));
        seeded = true;
    }
    Reset();
}

int SnakeGame::MinX() const { return 1; }
int SnakeGame::MaxX() const { return width - 2; }
int SnakeGame::MinY() const { return PLAYFIELD_TOP; }
int SnakeGame::MaxY() const { return height - 2; }

void SnakeGame::Reset()
{
    score = 0;
    gameOver = false;
    dir = Direction::Right;
    nextDir = Direction::Right;
    moveTimer = 0.f;

    body.clear();

    int cx = width / 2;
    int cy = (MinY() + MaxY()) / 2;

    for (int i = 0; i < INITIAL_LENGTH; i++)
    {
        Cell c;
        c.x = cx - i;
        c.y = cy;
        body.push_back(c);
    }

    SpawnFood();
}

void SnakeGame::Update(float dt, const InputManager& input)
{
    if (gameOver) return;

    ReadDirection(input);

    moveTimer += dt;
    if (moveTimer >= MOVE_INTERVAL)
    {
        moveTimer -= MOVE_INTERVAL;
        dir = nextDir;
        MoveSnake();
    }
}

void SnakeGame::ReadDirection(const InputManager& input)
{
    if ((input.IsKeyPressed(VK_UP) || input.IsKeyPressed('W')) && dir != Direction::Down)
        nextDir = Direction::Up;
    else if ((input.IsKeyPressed(VK_DOWN) || input.IsKeyPressed('S')) && dir != Direction::Up)
        nextDir = Direction::Down;
    else if ((input.IsKeyPressed(VK_LEFT) || input.IsKeyPressed('A')) && dir != Direction::Right)
        nextDir = Direction::Left;
    else if ((input.IsKeyPressed(VK_RIGHT) || input.IsKeyPressed('D')) && dir != Direction::Left)
        nextDir = Direction::Right;
}

void SnakeGame::MoveSnake()
{
    Cell oldTail = body.back();

    std::vector<Cell>::iterator it = body.end() - 1;
    while (it != body.begin())
    {
        std::vector<Cell>::iterator prev = it - 1;
        it->x = prev->x;
        it->y = prev->y;
        --it;
    }

    std::vector<Cell>::iterator head = body.begin();
    switch (dir)
    {
    case Direction::Up:    head->y -= 1; break;
    case Direction::Down:  head->y += 1; break;
    case Direction::Left:  head->x -= 1; break;
    case Direction::Right: head->x += 1; break;
    }

    if (head->x < MinX() || head->x > MaxX() ||
        head->y < MinY() || head->y > MaxY())
    {
        gameOver = true;
        if (score > bestScore) bestScore = score;
        return;
    }

    std::vector<Cell>::iterator bodyIt = body.begin() + 1;
    while (bodyIt != body.end())
    {
        if (head->x == bodyIt->x && head->y == bodyIt->y)
        {
            gameOver = true;
            if (score > bestScore) bestScore = score;
            return;
        }
        ++bodyIt;
    }

    if (head->x == food.x && head->y == food.y)
    {
        score += POINTS_PER_FOOD;
        if (score > bestScore) bestScore = score;

        body.push_back(oldTail);

        SpawnFood();
    }
}

void SnakeGame::SpawnFood()
{
    Cell c;
    int attempts = 0;

    do
    {
        c.x = MinX() + (std::rand() % (MaxX() - MinX() + 1));
        c.y = MinY() + (std::rand() % (MaxY() - MinY() + 1));
        attempts++;
    } while (IsOccupiedByBody(c) && attempts < 1000);

    food = c;
}

bool SnakeGame::IsOccupiedByBody(const Cell& c) const
{
    std::vector<Cell>::const_iterator it = body.begin();
    while (it != body.end())
    {
        if (it->x == c.x && it->y == c.y)
            return true;
        ++it;
    }
    return false;
}

void SnakeGame::DrawGameHUD(DrawManager& dm)
{
    int w = dm.GetWidth();

    for (int i = 1; i < w - 1; ++i)
        dm.SetCharInPosition(' ', HUD_ROW_TEXT, i);

    char left[50], right[50];
    sprintf_s(left, sizeof(left), "PUNTUACION: %d", score);
    sprintf_s(right, sizeof(right), "RECORD: %d", bestScore);

    int col = 3;
    for (int i = 0; left[i] != '\0'; i++)
    {
        if (col >= w - 2) break;
        dm.SetCharInPosition(left[i], HUD_ROW_TEXT, col++);
    }

    int rightLen = 0;
    for (int i = 0; right[i] != '\0'; i++) rightLen++;

    int rightStart = (w - 2) - rightLen;
    if (rightStart < 3) rightStart = 3;

    col = rightStart;
    for (int i = 0; right[i] != '\0'; i++)
    {
        if (col >= w - 2) break;
        dm.SetCharInPosition(right[i], HUD_ROW_TEXT, col++);
    }

    if (gameOver)
    {
        char msg[] = "|GAME OVER: (R) RESET / (M) MENU / (ESC) SALIR|";
        int msgLen = 0;
        for (int i = 0; msg[i] != '\0'; i++) msgLen++;

        int start = (w / 2) - msgLen / 2;
        if (start < 3) start = 3;
        col = start;
        for (int i = 0; msg[i] != '\0'; i++)
        {
            if (col >= w - 2) break;
            dm.SetCharInPosition(msg[i], HUD_ROW_TEXT, col++);
        }
    }
}

void SnakeGame::Draw(DrawManager& dm)
{
    DrawGameHUD(dm);

    dm.SetCharInPosition(CHAR_FOOD, food.y, food.x);

    std::vector<Cell>::iterator it = body.begin();
    while (it != body.end())
    {
        if (it->x >= 1 && it->x < width - 1 &&
            it->y >= PLAYFIELD_TOP && it->y < height - 1)
        {
            dm.SetCharInPosition(CHAR_BODY, it->y, it->x);
        }
        ++it;
    }
}