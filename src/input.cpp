#include "input.hpp"

namespace Lulu::Keyboard
{
    bool keyPressedThisFrame(Input *input, KeyCodeID code)
    {
        Key key = input->keys[code];
        return key.isDown && key.halfTransitionCount == 1 || key.halfTransitionCount > 2;
    }

    bool keyReleasedThisFrame(Input *input, KeyCodeID code)
    {
        Key key = input->keys[code];
        return !key.isDown && key.halfTransitionCount == 1 || key.halfTransitionCount > 2;
    }

    bool keyIsDown(Input *input, KeyCodeID code)
    {
        return input->keys[code].isDown;
    }
}
