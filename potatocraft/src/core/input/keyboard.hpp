#pragma once

namespace potatocraft {
    
    class Keyboard
    {
    public:
        Keyboard();

        enum keyStates
        {
            RELEASED,
            PRESSED
        };

        enum toggleableKeys
        {
            ESCAPE = 256,
            F3 = 292,
            F4 = 293
        };

        bool isKeyPressed(int key);
        void updateKeyState(int key, int action);
        bool isKeyReleased(int key);
        bool isToggleableKey(int key);
        bool isKeyToggled(int key);

    private:
        int m_lastKey;
        std::unordered_map<int, int> m_keyStates;
    };

}