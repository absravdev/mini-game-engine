#include "InputManager.h"
#include <windows.h>

char InputManager::PollKey(const std::vector<char>& keysToCheck) const
{
    if (keysToCheck.empty())
        return '\0';

    for (char letter : keysToCheck)
    {
        if (IsKeyJustPressed((int)letter))
            return letter;
    }

    return '\0';
}

bool InputManager::IsEscPressed() const
{
    return (GetAsyncKeyState(VK_ESCAPE) & 0x8000) != 0;
}

bool InputManager::IsRestartKeyPressed() const
{
    return (GetAsyncKeyState('R') & 0x8000) != 0;
}

bool InputManager::IsKeyPressed(int vk) const
{
    return (GetAsyncKeyState(vk) & 0x8000) != 0;
}

bool InputManager::IsKeyJustPressed(int vk) const
{
    return (GetAsyncKeyState(vk) & 0x0001) != 0;
}