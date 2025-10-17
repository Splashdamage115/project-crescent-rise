#include "KeyScan.h"

std::vector<std::pair<KeyScan::KeyCode, bool>> KeyScan::pressedKey;
double KeyScan::mouseX = 0.;
double KeyScan::mouseY = 0.;
std::vector<std::shared_ptr<mouseKeyInput>> KeyScan::mouseReleasedPair;
std::vector< std::shared_ptr<mouseKeyInput>> KeyScan::mousePressedPair;
std::vector<std::pair<KeyScan::MouseKeyCode, bool>> KeyScan::pressedMouse;


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
