#pragma once
#include <vector>

class InputManager
{
public:
    char PollKey(const std::vector<char>& keysToCheck) const;

    bool IsEscPressed()        const;
    bool IsRestartKeyPressed() const;

    bool IsKeyPressed(int virtualKey)  const;
    bool IsKeyJustPressed(int virtualKey) const;
};