#include "GameEngine.h"
#include "LluviaGame.h"

int main()
{
    const int fps = 100;
    const int height = 20;
    const int width = 80;

    LluviaGame game(height, width);

    GameEngine engine(fps, height, width);

    engine.RegisterGame('L', "Lluvia de Letras", &game);

    engine.Start();

    return 0;
}