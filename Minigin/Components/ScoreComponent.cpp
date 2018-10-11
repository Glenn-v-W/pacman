#include "MiniginPCH.h"
#include "ScoreComponent.h"
#include "..\Scenegraph\GameObject.h"
#include "PacmanComponent.h"
#include "TextComponent.h"


ScoreComponent::ScoreComponent()
{
}

ScoreComponent::~ScoreComponent()
{
}

void ScoreComponent::Update(float deltaTime)
{
	UNREFERENCED_PARAMETER(deltaTime);
	m_Score = (*m_pPacman)->GetComponent<PacmanComponent>()->GetScore();

	std::string text = "Player ";
	text += std::to_string(m_PlayerId+1);
	text += ": ";
	text += std::to_string(m_Score);
	GetGameObject()->GetComponent<TextComponent>()->SetText(text);
}

void ScoreComponent::SetPlayerId(int playerId)
{
	m_PlayerId = playerId;
}

void ScoreComponent::SetPacManReference(std::shared_ptr<GameObject*> pPacman)
{
	m_pPacman = pPacman;
}
