#pragma once

#include "Library.h"

class KeyScan
{
public:

    enum class KeyCode
    {
        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I
    };

    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_E && action == GLFW_PRESS)
        {

        }
    }
private:
};

