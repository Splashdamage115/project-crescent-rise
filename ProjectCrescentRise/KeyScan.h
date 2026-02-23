#pragma once

/*


                      How to use:



                      Assign to Callback



                      Example implementation:


    std::shared_ptr<keyboardInput> ki = std::make_shared<keyboardInput>();
    ki->active = true;
    ki->keyCode = KeyCode::SLASH;
    ki->function = ([]() { KeyScan::typingActive = true; ChatBoxText::typing = true; });
    append(ki, false);



*/

#include "Library.h"
#include <functional>

struct mouseKeyInput;
struct keyboardInput;

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
        ZERO,
        ONE,
        TWO,
        THREE,
        FOUR,
        FIVE,
        SIX,
        SEVEN,
        EIGHT,
        NINE,
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

    static void init();

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

    static void append(std::shared_ptr<keyboardInput> t_keyboard, bool released)
    {
        if (released)
            keyboardReleasedPair.push_back(t_keyboard);
        else
            keyboardPressedPair.push_back(t_keyboard);
    }

    static void mouseReleased(MouseKeyCode& t_key);
    static void mousePressed(MouseKeyCode& t_key);

    static void keyboardReleased(KeyCode t_key);
    static void keyboardPressed(KeyCode t_key);

    static double mouseX;
    static double mouseY;

    static bool HandleTyping(std::string& t_string);

    static bool typingActive;
private:
    static std::vector<std::pair<KeyCode, bool>> pressedKey;
    static std::vector<std::pair<MouseKeyCode, bool>> pressedMouse;

    static std::vector<std::shared_ptr<mouseKeyInput>> mouseReleasedPair;
    static std::vector< std::shared_ptr<mouseKeyInput>> mousePressedPair;

    static std::vector<std::shared_ptr<keyboardInput>> keyboardPressedPair;
    static std::vector<std::shared_ptr<keyboardInput>> keyboardReleasedPair;

    static KeyCode m_lastKeyStroke;
    static float m_typeWaitTime;
};

struct mouseKeyInput
{
    bool active = false;
    KeyScan::MouseKeyCode keyCode;
    std::function<void(void)> function;
};

struct keyboardInput
{
    bool active = false;
    KeyScan::KeyCode keyCode;
    std::function<void(void)> function;
};
