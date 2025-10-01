#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include "Game.h"

int main()
{
    if (!glfwInit())
        return -1;

    Game game;
    game.initGame();
    game.playGame();

    glfwTerminate();
    return 0;
}