#include "LluviaGame.h"
#include "DrawManager.h"
#include "InputManager.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>

LluviaGame::LluviaGame(int h, int w) : height(h), width(w)
{
    static bool seeded = false;
    if (!seeded)
    {
        std::srand((unsigned int)std::time(nullptr));
        seeded = true;
    }
    Reset();
}

void LluviaGame::Reset()
{
    score = 0;
    gameOver = false;
    spawnAccumulator = 0.f;
    letters.clear();
    effects.clear();
    difficulty.Reset();
}

void LluviaGame::Update(float dt, const InputManager& input)
{
    if (gameOver) return;

    difficulty.Update(dt);
    SpawnLetters(dt);
    UpdateLetters(dt);

    std::vector<char> activeKeys;
    for (const auto& l : letters)
        if (l.alive) activeKeys.push_back(l.c);

    char key = input.PollKey(activeKeys);
    if (key != '\0')
        HandleInput(key);

    UpdateEffects(dt);

    for (const auto& l : letters)
    {
        if (l.alive && ReachedBottom(l))
        {
            gameOver = true;
            bestScore = std::max(bestScore, score);
            break;
        }
    }
}

void LluviaGame::DrawGameHUD(DrawManager& dm)
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

void LluviaGame::Draw(DrawManager& dm)
{
    const int w = dm.GetWidth();
    const int h = dm.GetHeight();

    DrawGameHUD(dm);

    for (const auto& l : letters)
    {
        if (!l.alive) continue;

        int y = (int)l.y;
        int x = l.x;

        if (x <= 0 || x >= w - 1) continue;
        if (y <= 0 || y >= h - 1) continue;

        int top = PLAYFIELD_TOP;
        for (int r = y - 1; r >= top; --r)
            dm.SetCharInPosition('.', r, x);

        char ch = l.c;

        if (l.shieldHits > 0)
        {
            if (x - 1 > 0 && x + 1 < w - 1)
            {
                dm.SetCharInPosition('[', y, x - 1);
                dm.SetCharInPosition(ch, y, x);
                dm.SetCharInPosition(']', y, x + 1);
            }
            else
            {
                dm.SetCharInPosition(ch, y, x);
            }
        }
        else
        {
            dm.SetCharInPosition(ch, y, x);
        }
    }

    for (const auto& e : effects)
    {
        if (e.x <= 0 || e.x >= w - 1) continue;
        if (e.y <= 0 || e.y >= h - 1) continue;

        char ch = (e.type == EffectType::Trail) ? '.' : '*';
        dm.SetCharInPosition(ch, e.y, e.x);
    }
}

void LluviaGame::SpawnLetters(float dt)
{
    const float spawnRate = difficulty.GetSpawnRate();
    spawnAccumulator += spawnRate * dt;

    const int maxLetters = difficulty.GetMaxSimultaneousLetters();

    while (spawnAccumulator >= 1.f && (int)letters.size() < maxLetters)
    {
        spawnAccumulator -= 1.f;

        FallingLetter l;
        l.c = RandomLetter();
        l.x = 3 + (std::rand() % (width - 6));
        l.y = 3.f;
        l.vy = difficulty.GetFallSpeed();
        l.shieldHits = Chance(SHIELD_PROBABILITY) ? 1 : 0;

        letters.push_back(l);
    }
}

void LluviaGame::UpdateLetters(float dt)
{
    for (auto& l : letters)
    {
        if (!l.alive) continue;

        const int oldY = (int)l.y;
        l.vy = difficulty.GetFallSpeed();
        l.y += l.vy * dt;
        const int newY = (int)l.y;

        for (int y = oldY; y < newY; ++y)
        {
            if (y <= 0 || y >= height - 1) continue;
            Effect e;
            e.type = EffectType::Trail;
            e.x = l.x;
            e.y = y;
            e.ttl = TRAIL_TTL;
            effects.push_back(e);
        }
    }

    for (int i = (int)letters.size() - 1; i >= 0; --i)
        if (!letters[i].alive)
            letters.erase(letters.begin() + i);
}

void LluviaGame::HandleInput(char key)
{
    if (key >= 'a' && key <= 'z') key = (char)(key - 32);

    for (size_t i = 0; i < letters.size(); ++i)
    {
        if (!letters[i].alive) continue;
        if (letters[i].c == key)
            DamageShieldOrExplode(i);
    }
}

void LluviaGame::DamageShieldOrExplode(size_t idx)
{
    if (!letters[idx].alive) return;

    if (letters[idx].shieldHits > 0)
    {
        letters[idx].shieldHits -= 1;
        return;
    }

    ExplodeLetterAtIndex(idx);
    score += POINTS_PER_LETTER;
    bestScore = std::max(bestScore, score);
}

void LluviaGame::ExplodeLetterAtIndex(size_t idx)
{
    const int centerY = (int)letters[idx].y;

    for (int dy = -1; dy <= 1; ++dy)
        for (int dx = -1; dx <= 1; ++dx)
        {
            const int ex = letters[idx].x + dx;
            const int ey = centerY + dy;

            if (ex <= 0 || ex >= width - 1) continue;
            if (ey <= 0 || ey >= height - 1) continue;

            Effect e;
            e.type = EffectType::Explosion;
            e.x = ex;
            e.y = ey;
            e.ttl = EXPLOSION_TTL;
            effects.push_back(e);
        }

    letters[idx].alive = false;
}

void LluviaGame::UpdateEffects(float dt)
{
    for (auto& e : effects)
        e.ttl -= dt;

    for (int i = (int)effects.size() - 1; i >= 0; --i)
        if (effects[i].ttl <= 0.f)
            effects.erase(effects.begin() + i);
}

bool LluviaGame::ReachedBottom(const FallingLetter& l) const
{
    return (int)l.y >= height - 2;
}

char LluviaGame::RandomLetter() const
{
    return (char)('A' + (std::rand() % 26));
}

bool LluviaGame::Chance(int percent) const
{
    if (percent <= 0)   return false;
    if (percent >= 100) return true;
    return (std::rand() % 100) < percent;
}