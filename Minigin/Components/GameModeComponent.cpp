#include "MiniginPCH.h"
#include "GameModeComponent.h"
#include "..\InputManager.h"
#include "..\Scenegraph\GameObject.h"
#include "PacmanComponent.h"


GameModeComponent::GameModeComponent()
{
}

GameModeComponent::~GameModeComponent()
{
}

void GameModeComponent::Update(float deltaTime)
{
	UNREFERENCED_PARAMETER(deltaTime);

	if (*m_pGameMode == GameMode::Singleplayer || *m_pGameMode == GameMode::Ghost)
	{
		if ((*m_pPacman)->GetComponent<PacmanComponent>()->IsDead())
		{
			(*m_pGameMode) = GameMode::Menu;
			std::cout << "Game Over!\n";
		}
	}
	else
	{
		if ((*m_pPacman)->GetComponent<PacmanComponent>()->IsDead() && (*m_pMsPacman)->GetComponent<PacmanComponent>()->IsDead())
		{
			(*m_pGameMode) = GameMode::Menu;
			std::cout << "Game Over!\n";
		}
	}
}

void GameModeComponent::SetGameModeReference(std::shared_ptr<GameMode> pGameMode)
{
	m_pGameMode = pGameMode;
}

void GameModeComponent::SetPacManReference(std::shared_ptr<GameObject*> pPacman)
{
	m_pPacman = pPacman;
}

void GameModeComponent::SetMsPacManReference(std::shared_ptr<GameObject*> pMsPacman)
{
	m_pMsPacman = pMsPacman;
}