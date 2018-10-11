#include "MiniginPCH.h"
#include "InputManager.h"
#include <SDL.h>


bool InputManager::ProcessInput()
{
	ZeroMemory(&currentStatePlayer0, sizeof(XINPUT_STATE));
	XInputGetState(0, &currentStatePlayer0);
	ZeroMemory(&currentStatePlayer1, sizeof(XINPUT_STATE));
	XInputGetState(1, &currentStatePlayer1);

	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
		if (e.type == SDL_KEYDOWN) {
			
		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {
			
		}
	}

	return true;
}

bool InputManager::IsPressed(ControllerButton button, int playerId) const
{
	const Uint8 *pKeysStates = SDL_GetKeyboardState(nullptr);
	if (playerId == 0)
	{
		switch (button)
		{
		case ControllerButton::ButtonA:
			if (pKeysStates[SDL_SCANCODE_S] || pKeysStates[SDL_SCANCODE_3])
			{
				return true;
			}
			return currentStatePlayer0.Gamepad.wButtons & XINPUT_GAMEPAD_A;
		case ControllerButton::ButtonB:
			if (pKeysStates[SDL_SCANCODE_D] || pKeysStates[SDL_SCANCODE_2])
			{
				return true;
			}
			return currentStatePlayer0.Gamepad.wButtons & XINPUT_GAMEPAD_B;
		case ControllerButton::ButtonX:
			if (pKeysStates[SDL_SCANCODE_A])
			{
				return true;
			}
			return currentStatePlayer0.Gamepad.wButtons & XINPUT_GAMEPAD_X;
		case ControllerButton::ButtonY:
			if (pKeysStates[SDL_SCANCODE_W] || pKeysStates[SDL_SCANCODE_1])
			{
				return true;
			}
			return currentStatePlayer0.Gamepad.wButtons & XINPUT_GAMEPAD_Y;
		default: return false;
		}
	}
	else if (playerId == 1)
	{
		switch (button)
		{
		case ControllerButton::ButtonA:
			if (pKeysStates[SDL_SCANCODE_DOWN])
			{
				return true;
			}
			return currentStatePlayer1.Gamepad.wButtons & XINPUT_GAMEPAD_A;
		case ControllerButton::ButtonB:
			if (pKeysStates[SDL_SCANCODE_RIGHT])
			{
				return true;
			}
			return currentStatePlayer1.Gamepad.wButtons & XINPUT_GAMEPAD_B;
		case ControllerButton::ButtonX:
			if (pKeysStates[SDL_SCANCODE_LEFT])
			{
				return true;
			}
			return currentStatePlayer1.Gamepad.wButtons & XINPUT_GAMEPAD_X;
		case ControllerButton::ButtonY:
			if (pKeysStates[SDL_SCANCODE_UP])
			{
				return true;
			}
			return currentStatePlayer1.Gamepad.wButtons & XINPUT_GAMEPAD_Y;
		default: return false;
		}
	}
	else
	{
		return false;
	}
}

