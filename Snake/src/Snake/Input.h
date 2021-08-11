#pragma once
#include <iostream>

enum class Key
{
    A = 4,
    B = 5,
    C = 6,
    D = 7,
    E = 8,
    F = 9,
    G = 10,
    H = 11,
    I = 12,
    J = 13,
    K = 14,
    L = 15,
    M = 16,
    N = 17,
    O = 18,
    P = 19,
    Q = 20,
    R = 21,
    S = 22,
    T = 23,
    U = 24,
    V = 25,
    W = 26,
    X = 27,
    Y = 28,
    Z = 29,
    NUM_1 = 30,
    NUM_2 = 31,
    NUM_3 = 32,
    NUM_4 = 33,
    NUM_5 = 34,
    NUM_6 = 35,
    NUM_7 = 36,
    NUM_8 = 37,
    NUM_9 = 38,
    NUM_0 = 39,
    RETURN = 40,
    ESCAPE = 41,
    BACKSPACE = 42,
    TAB = 43,
    SPACE = 44,
    MINUS = 45,
    EQUALS = 46,
    LEFTBRACKET = 47,
    RIGHTBRACKET = 48,
    BACKSLASH = 49,
    NONUSHASH = 50,
    SEMICOLON = 51,
    APOSTROPHE = 52,
    GRAVE = 53,
    COMMA = 54,
    PERIOD = 55,
    SLASH = 56,
    CAPSLOCK = 57,
    F1 = 58,
    F2 = 59,
    F3 = 60,
    F4 = 61,
    F5 = 62,
    F6 = 63,
    F7 = 64,
    F8 = 65,
    F9 = 66,
    F10 = 67,
    F11 = 68,
    F12 = 69,
    PRINTSCREEN = 70,
    SCROLLLOCK = 71,
    PAUSE = 72,
    INSERT = 73,
    HOME = 74,
    PAGEUP = 75,
    _DELETE = 76,
    END = 77,
    PAGEDOWN = 78,
    RIGHT = 79,
    LEFT = 80,
    DOWN = 81,
    UP = 82,
    NUMLOCKCLEAR = 83,
    KP_DIVIDE = 84,
    KP_MULTIPLY = 85,
    KP_MINUS = 86,
    KP_PLUS = 87,
    KP_ENTER = 88,
    KP_1 = 89,
    KP_2 = 90,
    KP_3 = 91,
    KP_4 = 92,
    KP_5 = 93,
    KP_6 = 94,
    KP_7 = 95,
    KP_8 = 96,
    KP_9 = 97,
    KP_0 = 98,
    KP_PERIOD = 99,
    LCTRL = 224,
    LSHIFT = 225,
    LALT = 226,
    LGUI = 227,
    RCTRL = 228,
    RSHIFT = 229,
    RALT = 230,
    RGUI = 231,
};

enum class MouseButton
{
    LEFT = 1,
    MIDDLE = 2,
    RIGHT = 3,
};

static class Input
{
    friend class Window;
private:
    static void Update();

public:
    static bool AnyKeyPressed();
    static bool AnyKeyState();
    static bool KeyState(Key key);
    static bool KeyPressed(Key key);
    static bool KeyReleased(Key key);

public:
    static bool AnyMousePressed();
    static bool AnyMouseState();
    static bool MouseState(MouseButton button);
    static bool MousePressed(MouseButton button);
    static bool MouseReleased(MouseButton button);

public:
    static int32_t MouseX();
    static int32_t MouseY();
    static void MouseXY(int32_t& x, int32_t& y);
}Input;