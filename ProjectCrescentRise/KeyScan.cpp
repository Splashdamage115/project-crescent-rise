#include "KeyScan.h"
#include "Game.h"
#include "ChatBoxText.h"

std::vector<std::pair<KeyScan::KeyCode, bool>> KeyScan::pressedKey;
double KeyScan::mouseX = 0.;
double KeyScan::mouseY = 0.;
std::vector<std::shared_ptr<mouseKeyInput>> KeyScan::mouseReleasedPair;
std::vector< std::shared_ptr<mouseKeyInput>> KeyScan::mousePressedPair;
std::vector<std::pair<KeyScan::MouseKeyCode, bool>> KeyScan::pressedMouse;
std::vector<std::shared_ptr<keyboardInput>> KeyScan::keyboardPressedPair;
std::vector<std::shared_ptr<keyboardInput>> KeyScan::keyboardReleasedPair;
bool KeyScan::typingActive = false;
KeyScan::KeyCode KeyScan::m_lastKeyStroke = KeyScan::KeyCode::ESC;
float KeyScan::m_typeWaitTime = 0.3f;


void KeyScan::init()
{
    KeyCode lastNum = static_cast<KeyCode>(KeyCode::LALT);

    for (int i = 0; i < static_cast<int>(lastNum); i++)
    {
        KeyCode currentNum = static_cast<KeyCode>(i);
        pressedKey.emplace_back(currentNum, false);
    }


    std::shared_ptr<keyboardInput> ki = std::make_shared<keyboardInput>();
    ki->active = true;
    ki->keyCode = KeyCode::SLASH;
    ki->function = ([]() { KeyScan::typingActive = true; ChatBoxText::typing = true; });
    append(ki, false);
}

void KeyScan::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key >= GLFW_KEY_A && key <= GLFW_KEY_Z)
    {
        KeyCode code = static_cast<KeyCode>(key - GLFW_KEY_A);

        for (unsigned int i = 0; i < pressedKey.size(); i++)
        {
            if (pressedKey.at(i).first == code)
            {
                if (action == GLFW_PRESS)
                {
                    pressedKey.at(i).second = true;
                    keyboardPressed(code);
                }
                else if (action == GLFW_RELEASE)
                {
                    pressedKey.at(i).second = false;
                    keyboardReleased(code);
                    m_typeWaitTime = 0.f;
                }
            }

        }
    }
    else if (key >= GLFW_KEY_0 && key <= GLFW_KEY_9)
    {
        KeyCode code = static_cast<KeyCode>(key - GLFW_KEY_0 + 26);

        for (unsigned int i = 26; i < pressedKey.size(); i++)
        {
            if (pressedKey.at(i).first == code)
            {
                if (action == GLFW_PRESS)
                {
                    pressedKey.at(i).second = true;
                    keyboardPressed(code);
                }
                else if (action == GLFW_RELEASE)
                {
                    pressedKey.at(i).second = false;
                    m_typeWaitTime = 0.f;
                    keyboardReleased(code);
                }
            }

        }
    }
    else
    {
        for (unsigned int i = 0; i < pressedKey.size(); i++)
        {
            if (pressedKey.at(i).first == KeyCode::ESC && key == GLFW_KEY_ESCAPE)
            {
                if (action == GLFW_PRESS)
                {
                    pressedKey.at(i).second = true;
                    keyboardPressed(KeyCode::ESC);
                }
                else if (action == GLFW_RELEASE)
                {
                    pressedKey.at(i).second = false;
                    keyboardReleased(KeyCode::ESC);
                }
            }
            else if (pressedKey.at(i).first == KeyCode::TAB && key == GLFW_KEY_TAB)
            {
                if (action == GLFW_PRESS)
                {
                    keyboardPressed(KeyCode::TAB);
                    pressedKey.at(i).second = true;
                }
                else if (action == GLFW_RELEASE)
                {
                    keyboardReleased(KeyCode::TAB);
                    pressedKey.at(i).second = false;
                }
            }
            else if (pressedKey.at(i).first == KeyCode::LALT && key == GLFW_KEY_LEFT_ALT)
            {
                if (action == GLFW_PRESS)
                {
                    keyboardPressed(KeyCode::LALT);
                    pressedKey.at(i).second = true;
                }
                else if (action == GLFW_RELEASE)
                {
                    keyboardReleased(KeyCode::LALT);
                    pressedKey.at(i).second = false;
                }
            }
            else if (pressedKey.at(i).first == KeyCode::LCTRL && key == GLFW_KEY_LEFT_CONTROL)
            {
                if (action == GLFW_PRESS)
                {
                    keyboardPressed(KeyCode::LCTRL);
                    pressedKey.at(i).second = true;
                }
                else if (action == GLFW_RELEASE)
                {
                    keyboardReleased(KeyCode::LCTRL);
                    pressedKey.at(i).second = false;
                }
            }
            else if (pressedKey.at(i).first == KeyCode::ENTER && key == GLFW_KEY_ENTER)
            {
                // WE SPECIFICALLY SET THE TYPING
                if (action == GLFW_PRESS)
                {
                    pressedKey.at(i).second = true;
                }
                else if (action == GLFW_RELEASE)
                {
                    pressedKey.at(i).second = false;
                    typingActive = !typingActive;

                }
            }
            else if (pressedKey.at(i).first == KeyCode::CAPS_LOCK && key == GLFW_KEY_CAPS_LOCK)
            {
                if (action == GLFW_PRESS)
                {
                    keyboardPressed(KeyCode::CAPS_LOCK);
                    pressedKey.at(i).second = true;
                }
                else if (action == GLFW_RELEASE)
                {
                    keyboardReleased(KeyCode::CAPS_LOCK);
                    pressedKey.at(i).second = false;
                }
            }
            else if (pressedKey.at(i).first == KeyCode::BACKSPACE && key == GLFW_KEY_BACKSPACE)
            {
                if (action == GLFW_PRESS)
                {
                    pressedKey.at(i).second = true;
                }
                else if (action == GLFW_RELEASE)
                {
                    pressedKey.at(i).second = false;
                    m_typeWaitTime = 0.f;
                }
            }
            else if (pressedKey.at(i).first == KeyCode::SPACEBAR && key == GLFW_KEY_SPACE)
            {
                if (action == GLFW_PRESS)
                {
                    keyboardPressed(KeyCode::SPACEBAR);
                    pressedKey.at(i).second = true;
                }
                else if (action == GLFW_RELEASE)
                {
                    keyboardReleased(KeyCode::SPACEBAR);
                    pressedKey.at(i).second = false;
                    m_typeWaitTime = 0.f;
                }
            }
            else if (pressedKey.at(i).first == KeyCode::SLASH && key == GLFW_KEY_SLASH)
            {
                if (action == GLFW_PRESS)
                {
                    keyboardPressed(KeyCode::SLASH);
                    pressedKey.at(i).second = true;
                }
                else if (action == GLFW_RELEASE)
                {
                    keyboardReleased(KeyCode::SLASH);
                    pressedKey.at(i).second = false;
                    m_typeWaitTime = 0.f;
                }
            }
            else if (pressedKey.at(i).first == KeyCode::LSHIFT && key == GLFW_KEY_LEFT_SHIFT)
            {
                if (action == GLFW_PRESS)
                {
                    keyboardPressed(KeyCode::LSHIFT);
                    pressedKey.at(i).second = true;
                }
                else if (action == GLFW_RELEASE)
                {
                    keyboardReleased(KeyCode::LSHIFT);
                    pressedKey.at(i).second = false;
                }
            }
        }
    }
}

void KeyScan::mouseClickCallback(GLFWwindow* window, int button, int action, int mods)
{
    MouseKeyCode m = static_cast<MouseKeyCode>(button);

    if (action == GLFW_PRESS)
    {
        mousePressed(m);
    }
    else if (action == GLFW_RELEASE)
    {
        mouseReleased(m);
    }
}

void KeyScan::mouseReleased(MouseKeyCode& t_key)
{
    for (auto& func : mouseReleasedPair)
    {
        if (func == nullptr) continue;

        if (func->active)
            if (func->keyCode == t_key)
                func->function();
    }
    for (auto& i : pressedMouse)
    {
        if (i.first == t_key)
            i.second = false;
    }
}

void KeyScan::mousePressed(MouseKeyCode& t_key)
{
    for (auto& func : mousePressedPair)
    {
        if (func == nullptr) continue;

        if (func->active)
            if (func->keyCode == t_key)
                func->function();
    }
    for (auto& i : pressedMouse)
    {
        if (i.first == t_key)
            i.second = true;
    }
}

void KeyScan::keyboardReleased(KeyCode t_key)
{
    for (auto& func : keyboardReleasedPair)
    {
        if (func == nullptr) continue;

        if (func->active)
            if (func->keyCode == t_key)
                func->function();
    }
}

void KeyScan::keyboardPressed(KeyCode t_key)
{
    for (auto& func : keyboardPressedPair)
    {
        if (func == nullptr) continue;

        if (func->active)
            if (func->keyCode == t_key)
                func->function();
    }
}

bool KeyScan::HandleTyping(std::string& t_string)
{
    float keyStrokeTime = 0.2f;
    if (!typingActive) return false;
    for (unsigned int i = 0; i < pressedKey.size(); i++)
    {
        if (i >= 26)
        {
            if (pressedKey.at(i).first == KeyCode::SPACEBAR)
            {
                if (m_lastKeyStroke == pressedKey.at(i).first)
                {
                    if (pressedKey.at(i).second)
                    {
                        m_typeWaitTime -= Game::deltaTime;
                        if (m_typeWaitTime >= 0.f)
                            break;
                    }
                }
                if (pressedKey.at(i).second) {
                    t_string += ' ';
                    m_typeWaitTime = keyStrokeTime;
                    m_lastKeyStroke = pressedKey.at(i).first;
                }
            }
            if (pressedKey.at(i).first == KeyCode::BACKSPACE)
            {
                if (m_lastKeyStroke == pressedKey.at(i).first)
                {
                    if (pressedKey.at(i).second)
                    {
                        m_typeWaitTime -= Game::deltaTime;
                        if (m_typeWaitTime >= 0.f)
                            break;
                    }
                }
                if (pressedKey.at(i).second)
                {
                    if (t_string.size() == 0) continue;
                    std::string t = t_string;
                    t_string.clear();
                    for (int l = 0; l < t.size() - 1; l++)
                    {
                        t_string += t.at(l);
                    }
                    m_typeWaitTime = keyStrokeTime;
                    m_lastKeyStroke = pressedKey.at(i).first;
                }
            }
            if (pressedKey.at(i).first == KeyCode::SLASH)
            {
                if (m_lastKeyStroke == pressedKey.at(i).first)
                {
                    if (pressedKey.at(i).second)
                    {
                        m_typeWaitTime -= Game::deltaTime;
                        if (m_typeWaitTime >= 0.f)
                            break;
                    }
                }
                if (pressedKey.at(i).second) {
                    t_string += '/';
                    m_typeWaitTime = keyStrokeTime;
                    m_lastKeyStroke = pressedKey.at(i).first;
                }
            }
            if (pressedKey.at(i).first == KeyCode::ESC)
            {
                if (pressedKey.at(i).second) 
                {
                    t_string.clear();
                    typingActive = false;
                    return true;
                }
            }
            if (pressedKey.at(i).first == KeyCode::ENTER)
            {
                if (pressedKey.at(i).second)
                {
                    return true;
                }
            }
            if (m_lastKeyStroke == pressedKey.at(i).first)
            {
                if (pressedKey.at(i).second)
                {
                    m_typeWaitTime -= Game::deltaTime;
                    if (m_typeWaitTime >= 0.f)
                        break;
                }
            }
            else if (pressedKey.at(i).first == KeyCode::ZERO)
            {
                if (pressedKey.at(i).second)
                {
                    t_string += '0';
                    m_typeWaitTime = keyStrokeTime;
                    m_lastKeyStroke = pressedKey.at(i).first;
                }
            }
            else if (pressedKey.at(i).first == KeyCode::ONE)
            {
                if (pressedKey.at(i).second)
                {
                    t_string += '1';
                    m_typeWaitTime = keyStrokeTime;
                    m_lastKeyStroke = pressedKey.at(i).first;
                }
            }
            else if (pressedKey.at(i).first == KeyCode::TWO)
            {
                if (pressedKey.at(i).second)
                {
                    t_string += '2';
                    m_typeWaitTime = keyStrokeTime;
                    m_lastKeyStroke = pressedKey.at(i).first;
                }
            }
            else if (pressedKey.at(i).first == KeyCode::THREE)
            {
                if (pressedKey.at(i).second)
                {
                    t_string += '3';
                    m_typeWaitTime = keyStrokeTime;
                    m_lastKeyStroke = pressedKey.at(i).first;
                }
            }
            else if (pressedKey.at(i).first == KeyCode::FOUR)
            {
                if (pressedKey.at(i).second)
                {
                    t_string += '4';
                    m_typeWaitTime = keyStrokeTime;
                    m_lastKeyStroke = pressedKey.at(i).first;
                }
            }
            else if (pressedKey.at(i).first == KeyCode::FIVE)
            {
                if (pressedKey.at(i).second)
                {
                    t_string += '5';
                    m_typeWaitTime = keyStrokeTime;
                    m_lastKeyStroke = pressedKey.at(i).first;
                }
            }
            else if (pressedKey.at(i).first == KeyCode::SIX)
            {
                if (pressedKey.at(i).second)
                {
                    t_string += '6';
                    m_typeWaitTime = keyStrokeTime;
                    m_lastKeyStroke = pressedKey.at(i).first;
                }
            }
            else if (pressedKey.at(i).first == KeyCode::SEVEN)
            {
                if (pressedKey.at(i).second)
                {
                    t_string += '7';
                    m_typeWaitTime = keyStrokeTime;
                    m_lastKeyStroke = pressedKey.at(i).first;
                }
            }
            else if (pressedKey.at(i).first == KeyCode::EIGHT)
            {
                if (pressedKey.at(i).second)
                {
                    t_string += '8';
                    m_typeWaitTime = keyStrokeTime;
                    m_lastKeyStroke = pressedKey.at(i).first;
                }
            }
            else if (pressedKey.at(i).first == KeyCode::NINE)
            {
                if (pressedKey.at(i).second)
                {
                    t_string += '9';
                    m_typeWaitTime = keyStrokeTime;
                    m_lastKeyStroke = pressedKey.at(i).first;
                }
            }
            continue;
        }
        if (m_lastKeyStroke == pressedKey.at(i).first)
        {
            if (pressedKey.at(i).second)
            {
                m_typeWaitTime -= Game::deltaTime;
                if (m_typeWaitTime >= 0.f)
                    break;
            }
        }
        else if (pressedKey.at(i).second)
        {
            int loc = i + static_cast<int>((isKeyDown(KeyCode::LSHIFT)) ? 'A' : 'A');
            char typed = static_cast<char>(loc);
            t_string += typed;
            m_typeWaitTime = keyStrokeTime;
            m_lastKeyStroke = pressedKey.at(i).first;
            break;
        }
    }
    return false;
}
