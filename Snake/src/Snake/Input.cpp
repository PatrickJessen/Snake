#include "Input.h"
#include "SDL.h"
#include "SDL_events.h"

#define NOT_PRESSED 0
#define IS_PRESSED 1
#define FIRST_PRESS 2
#define IS_RELEASED 3


static uint8_t s_keyPressedMap[255] = { NOT_PRESSED };
static uint8_t s_keyReleasedMap[255] = { NOT_PRESSED };
static uint8_t s_mousebuttonPressedMap[255] = { NOT_PRESSED };
static uint8_t s_mousebuttonReleasedMap[255] = { NOT_PRESSED };

static int32_t s_xMouse = 0;
static int32_t s_yMouse = 0;

const static uint8_t* s_keyState;
static uint8_t s_mouseState[3];

static bool S_InputPressed(bool(*inputState)(uint8_t input), uint8_t(*pressedMap), uint8_t input)
{
	if ((inputState(input)) && (pressedMap[input] == NOT_PRESSED))
		pressedMap[input] = FIRST_PRESS;
	if (pressedMap[input] == FIRST_PRESS)
	{
		pressedMap[input] = IS_PRESSED;
		return true;
	}
	if (inputState(input) == false)
		pressedMap[input] = NOT_PRESSED;
	return false;
}
static bool S_InputReleased(bool(*inputState)(uint8_t input), uint8_t(*releasedMap), uint8_t input)
{
	if ((inputState(input)) && (releasedMap[input] == NOT_PRESSED))
		releasedMap[input] = IS_PRESSED;
	if ((inputState(input) == false) && (releasedMap[input] == IS_PRESSED))
		releasedMap[input] = IS_RELEASED;
	if (releasedMap[input] == IS_RELEASED)
	{
		releasedMap[input] = NOT_PRESSED;
		return true;
	}
	return false;
}

static bool S_AnyKeyState(uint8_t key)
{
	return (bool)s_keyState[key];
}
static bool S_AnyMouseState(uint8_t button)
{
	return (bool)s_mouseState[button];
}

void Input::Update()
{
	s_keyState = SDL_GetKeyboardState(NULL);
	for (uint8_t i = 0; i < 3; i++)
		s_mouseState[i] = SDL_GetMouseState(&s_xMouse, &s_yMouse) & SDL_BUTTON(i);
}

bool Input::AnyKeyPressed()
{
	for (int i = 0; i < 255; i++)
		if (KeyPressed((Key)i))
			return true;
	return false;
}

bool Input::AnyKeyState()
{
	for (int i = 0; i < 255; i++)
		if (KeyState((Key)i))
			return true;
	return false;
}

bool Input::KeyState(Key key)
{
	return S_AnyKeyState((uint8_t)key);
}

bool Input::KeyPressed(Key key)
{
	return S_InputPressed(S_AnyKeyState, s_keyPressedMap, (uint8_t)key);
}

bool Input::KeyReleased(Key key)
{
	return S_InputReleased(S_AnyKeyState, s_keyReleasedMap, (uint8_t)key);
}

int32_t Input::MouseX()
{
	return s_xMouse;
}
int32_t Input::MouseY()
{
	return s_yMouse;
}
void Input::MouseXY(int32_t& x, int32_t& y)
{
	x = s_xMouse;
	y = s_yMouse;
}
bool Input::AnyMousePressed()
{
	for (int i = 0; i < 3; i++)
		if (MousePressed((MouseButton)i))
			return true;
	return false;
}
bool Input::AnyMouseState()
{
	for (int i = 0; i < 3; i++)
		if (MouseState((MouseButton)i))
			return true;
	return false;
}
bool Input::MouseState(MouseButton button)
{
	return S_AnyMouseState((uint8_t)button);
}
bool Input::MousePressed(MouseButton button)
{
	return S_InputPressed(S_AnyMouseState, s_mousebuttonPressedMap, (uint8_t)button);
}
bool Input::MouseReleased(MouseButton button)
{
	return S_InputReleased(S_AnyMouseState, s_mousebuttonReleasedMap, (uint8_t)button);
}