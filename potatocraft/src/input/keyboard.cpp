#include <src/input/keyboard.hpp>

bool Keyboard::isKeyPressed(int key) {
    return m_keyStates[key] == 1;
}

bool Keyboard::isKeyReleased(int key) {
    return m_keyStates[key] == m_lastKey;
}

bool Keyboard::isToggleableKey(int key) {
    bool isToggleable = false;
    switch (key)
    {
    case toggleableKeys::ESCAPE:
        isToggleable = true;
        break;
    case toggleableKeys::F3:
        isToggleable = true;
        break;
    case toggleableKeys::F4:
        isToggleable = true;
        break;
    default:
        break;
    }
    return isToggleable;
}

bool Keyboard::isKeyToggled(int key) {
    return isToggleableKey(key) && isKeyPressed(key);
}

void Keyboard::updateKeyState(int key, int action) {
    if (action == keyStates::PRESSED) {
        m_lastKey = key;
        if (isKeyToggled(key)){
            m_keyStates[key] = keyStates::RELEASED;
        } else {
            m_keyStates[key] = keyStates::PRESSED;
        }
    } else {
        if (!isToggleableKey(key)){
            m_keyStates[key] = keyStates::RELEASED;
        }
    }
}

Keyboard::Keyboard():m_lastKey(-1) {}