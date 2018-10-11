#pragma once
#include <XInput.h>
#include "Singleton.h"

enum class ControllerButton
{
	ButtonA,
	ButtonB,
	ButtonX,
	ButtonY
};

class InputManager final : public Singleton<InputManager>
{
public:
	bool ProcessInput();
	bool IsPressed(ControllerButton button, int playerId) const;
private:
	XINPUT_STATE currentStatePlayer0{};
	XINPUT_STATE currentStatePlayer1{};
};
