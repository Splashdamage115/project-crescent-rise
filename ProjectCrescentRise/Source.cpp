#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include "Game.h"
#include <cstdlib>
#include <ctime>

int main()
{
    if (!glfwInit())
        return -1;

    srand(static_cast<unsigned int>(time(nullptr)));

    Game game;
    game.initGame();
    game.playGame();

    glfwTerminate();
    system("pause");
    return 0;
}