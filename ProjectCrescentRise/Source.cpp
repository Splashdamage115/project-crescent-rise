#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include "GameLoop.h"

int main()
{
    if (!glfwInit())
        return -1;

    GameLoop game;
    game.playGame();

    glfwTerminate();
    return 0;
}