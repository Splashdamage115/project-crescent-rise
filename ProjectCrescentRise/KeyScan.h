#pragma once

#include "Library.h"
#include <functional>

struct mouseKeyInput;

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
        ENTER,
        CAPS_LOCK,
        BACKSPACE,
        SPACEBAR,
        SLASH,
        LALT
    };

    enum class MouseKeyCode
    {
        LeftMouse,
        RightMouse,
        MiddleMouse
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

    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

    static void mouseClickCallback(GLFWwindow* window, int button, int action, int mods);

    static void cursorCallback(GLFWwindow* window, double xpos, double ypos)
    {
        mouseX = xpos;
        mouseY = ypos;
    }

    static bool isKeyDown(KeyCode t_key)
    {
        if (t_key == KeyCode::CAPS_LOCK)
        {
            for (unsigned int i = 0; i < pressedKey.size(); i++)
            {
                if (pressedKey.at(i).first == t_key)
                {
                    if (pressedKey.at(i).second)
                        return true;
                    else
                        return false;
                }
            }
        }
        if (typingActive) return false;
        for (unsigned int i = 0; i < pressedKey.size(); i++)
        {
            if (pressedKey.at(i).first == t_key)
            {
                if (pressedKey.at(i).second)
                    return true;
                else
                    return false;
            }
        }
        return false;
    }

    static void append(std::shared_ptr<mouseKeyInput> t_mski, bool released)
    {
        if(released)
            mouseReleasedPair.push_back(t_mski);
        else
            mousePressedPair.push_back(t_mski);
    }

    static void mouseReleased(MouseKeyCode& t_key);
    static void mousePressed(MouseKeyCode& t_key);

    static double mouseX;
    static double mouseY;

    static bool HandleTyping(std::string& t_string);

    static bool typingActive;
private:
    static std::vector<std::pair<KeyCode, bool>> pressedKey;
    static std::vector<std::pair<MouseKeyCode, bool>> pressedMouse;
    static std::vector<std::shared_ptr<mouseKeyInput>> mouseReleasedPair;
    static std::vector< std::shared_ptr<mouseKeyInput>> mousePressedPair;
    static KeyCode m_lastKeyStroke;
    static float m_typeWaitTime;
};

struct mouseKeyInput
{
    bool active = false;
    KeyScan::MouseKeyCode keyCode;
    std::function<void(void)> function;
};
