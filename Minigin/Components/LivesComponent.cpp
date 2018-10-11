#include "MiniginPCH.h"
#include "LivesComponent.h"
#include "..\Scenegraph\GameObject.h"
#include "PacmanComponent.h"
#include "DrawComponent.h"
#include "..\ResourceManager.h"


LivesComponent::LivesComponent()
{
}

LivesComponent::~LivesComponent()
{
}

void LivesComponent::Update(float deltaTime)
{
	UNREFERENCED_PARAMETER(deltaTime);
	m_Lives = (*m_pPacman)->GetComponent<PacmanComponent>()->GetLives();

	if (m_Lives == 3)
	{
		GetGameObject()->GetComponent<DrawComponent>()->SetTexture(m_Texture_3);
	}
	else if (m_Lives == 2)
	{
		GetGameObject()->GetComponent<DrawComponent>()->SetTexture(m_Texture_2);
	}
	else if (m_Lives == 1)
	{
		GetGameObject()->GetComponent<DrawComponent>()->SetTexture(m_Texture_1);
	}
	else
	{
		GetGameObject()->GetComponent<DrawComponent>()->SetTexture(m_Texture_0);
	}
}

void LivesComponent::SetIsMsPacman()
{
	m_IsMsPacman = true;
}

void LivesComponent::SetPacManReference(std::shared_ptr<GameObject*> pPacman)
{
	m_pPacman = pPacman;
}

void LivesComponent::SetTextures()
{
	if (!m_IsMsPacman)
	{
		m_Texture_3 = ResourceManager::GetInstance().LoadTexture("..\\Data\\Sprites\\Pacman\\Life\\3.png");
		m_Texture_2 = ResourceManager::GetInstance().LoadTexture("..\\Data\\Sprites\\Pacman\\Life\\2.png");
		m_Texture_1 = ResourceManager::GetInstance().LoadTexture("..\\Data\\Sprites\\Pacman\\Life\\1.png");
		m_Texture_0 = ResourceManager::GetInstance().LoadTexture("..\\Data\\Sprites\\Pacman\\Life\\0.png");
	}
	else
	{
		m_Texture_3 = ResourceManager::GetInstance().LoadTexture("..\\Data\\Sprites\\MsPacman\\Life\\3.png");
		m_Texture_2 = ResourceManager::GetInstance().LoadTexture("..\\Data\\Sprites\\MsPacman\\Life\\2.png");
		m_Texture_1 = ResourceManager::GetInstance().LoadTexture("..\\Data\\Sprites\\MsPacman\\Life\\1.png");
		m_Texture_0 = ResourceManager::GetInstance().LoadTexture("..\\Data\\Sprites\\MsPacman\\Life\\0.png");
	}
}