#include "MiniginPCH.h"
#include "MenuComponent.h"
#include "..\InputManager.h"


MenuComponent::MenuComponent()
{
}

MenuComponent::~MenuComponent()
{
}

void MenuComponent::Update(float deltaTime)
{
	UNREFERENCED_PARAMETER(deltaTime);

	auto& input = InputManager::GetInstance();
	if (input.IsPressed(ControllerButton::ButtonY, 0))
	{
		(*m_pGameMode) = GameMode::Singleplayer;
		std::cout << "Singleplayer selected!\n";
	}
	if (input.IsPressed(ControllerButton::ButtonB, 0))
	{
		(*m_pGameMode) = GameMode::MsPacman;
		std::cout << "Pacman & MsPacman selected!\n";
	}
	if (input.IsPressed(ControllerButton::ButtonA, 0))
	{
		(*m_pGameMode) = GameMode::Ghost;
		std::cout << "Pacman & Ghost selected!\n";
	}
}

void MenuComponent::SetGameModeReference(std::shared_ptr<GameMode> pGameMode)
{
	m_pGameMode = pGameMode;
}
