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
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q,
        R,
        S,
        T,
        U,
        V,
        W,
        X,
        Y,
        Z,
        ESC,
        TAB,
        LSHIFT,
        LCTRL,
        LALT
    };

    static void init()
    {
        KeyCode lastNum = static_cast<KeyCode>(KeyCode::LALT);

        for (int i = 0; i < static_cast<int>(lastNum); i++)
        {
            KeyCode currentNum = static_cast<KeyCode>(i);
            pressedKey.emplace_back(currentNum, false);
        }
    }

    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (key >= GLFW_KEY_A && key <= GLFW_KEY_LAST)
        {
            KeyCode code = static_cast<KeyCode>(key - GLFW_KEY_A);

            for (int i = 0; i < pressedKey.size(); i++)
            {
                if (pressedKey.at(i).first == code)
                {
                    if (action == GLFW_PRESS)
                        pressedKey.at(i).second = true;
                    else if (action == GLFW_RELEASE)
                        pressedKey.at(i).second = false;
                }
                else if (pressedKey.at(i).first == KeyCode::ESC)
                {
                    if (action == GLFW_PRESS)
                        pressedKey.at(i).second = true;
                    else if (action == GLFW_RELEASE)
                        pressedKey.at(i).second = false;
                }
                else if (pressedKey.at(i).first == KeyCode::TAB)
                {
                    if (action == GLFW_PRESS)
                        pressedKey.at(i).second = true;
                    else if (action == GLFW_RELEASE)
                        pressedKey.at(i).second = false;
                }
                else if (pressedKey.at(i).first == KeyCode::LALT)
                {
                    if (action == GLFW_PRESS)
                        pressedKey.at(i).second = true;
                    else if (action == GLFW_RELEASE)
                        pressedKey.at(i).second = false;
                }
                else if (pressedKey.at(i).first == KeyCode::LCTRL)
                {
                    if (action == GLFW_PRESS)
                        pressedKey.at(i).second = true;
                    else if (action == GLFW_RELEASE)
                        pressedKey.at(i).second = false;
                }
                else if (pressedKey.at(i).first == KeyCode::LSHIFT)
                {
                    if (action == GLFW_PRESS)
                        pressedKey.at(i).second = true;
                    else if (action == GLFW_RELEASE)
                        pressedKey.at(i).second = false;
                }
            }
        }
    }

    static void cursorCallback(GLFWwindow* window, double xpos, double ypos)
    {
        mouseX = xpos;
        mouseY = ypos;
    }

    static bool isKeyDown(KeyCode t_key)
    {
        for (int i = 0; i < pressedKey.size(); i++)
        {
            if (pressedKey.at(i).first == t_key)
            {
                return pressedKey.at(i).second;
            }
        }
    }

    static double mouseX;
    static double mouseY;

private:
    static std::vector<std::pair<KeyCode, bool>> pressedKey;
};

