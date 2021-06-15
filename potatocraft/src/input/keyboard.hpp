#include <unordered_map>

class Keyboard
{
private:
    std::unordered_map<int, bool> keyStates; // bool ? pressed : released

public:
    bool isKeyPressed(int key);
    bool registerKey(int key);
};