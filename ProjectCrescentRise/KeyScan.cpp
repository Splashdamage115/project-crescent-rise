#include "KeyScan.h"
#include "Game.h"

std::vector<std::pair<KeyScan::KeyCode, bool>> KeyScan::pressedKey;
double KeyScan::mouseX = 0.;
double KeyScan::mouseY = 0.;
std::vector<std::shared_ptr<mouseKeyInput>> KeyScan::mouseReleasedPair;
std::vector< std::shared_ptr<mouseKeyInput>> KeyScan::mousePressedPair;
std::vector<std::pair<KeyScan::MouseKeyCode, bool>> KeyScan::pressedMouse;
bool KeyScan::typingActive = false;
KeyScan::KeyCode KeyScan::m_lastKeyStroke = KeyScan::KeyCode::ESC;
float KeyScan::m_typeWaitTime = 0.3f;


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

bool KeyScan::HandleTyping(std::string& t_string)
{
    float keyStrokeTime = 0.2f;
    if (!typingActive) return;
    for (unsigned int i = 0; i < pressedKey.size(); i++)
    {
        if (i > 26)
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
                }
            }
            if (pressedKey.at(i).first == KeyCode::ENTER)
            {
                if (pressedKey.at(i).second)
                {
                    typingActive = false;
                    return true;
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
        if (pressedKey.at(i).second)
        {
            int loc = i + static_cast<int>((isKeyDown(KeyCode::CAPS_LOCK)) ? 'a' : 'A');
            char typed = static_cast<char>(loc);
            t_string += typed;
            m_typeWaitTime = keyStrokeTime;
            m_lastKeyStroke = pressedKey.at(i).first;
            break;
        }
    }
    return false;
}
