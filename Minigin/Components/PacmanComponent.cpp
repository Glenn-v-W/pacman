#include "MiniginPCH.h"
#include "PacmanComponent.h"
#include "DrawComponent.h"
#include "TransformComponent.h"
#include "GridComponent.h"
#include "GhostComponent.h"
#include "..\Scenegraph\GameObject.h"
#include "..\ResourceManager.h"
#include "..\InputManager.h"

PacmanComponent::PacmanComponent()
{}

PacmanComponent::~PacmanComponent()
{}

void PacmanComponent::Update(float deltaTime)
{
	if (!m_IsDead)
	{
		HandleEatingGhost();

		UpdateDirection();
		HandleMovement(deltaTime);

		//Handle textures
		m_TimeSinceLastAnimChange += deltaTime;

		//Change anims once every [animSpeed] seconds
		float animSpeed = 0.1f;
		if (m_TimeSinceLastAnimChange >= animSpeed)
		{
			UpdateTexture();
			m_TimeSinceLastAnimChange = 0;
		}
	}
}

void PacmanComponent::UpdateTexture()
{
	switch (m_CurrentState)
	{
	case PacmanStates::Moving:
		switch (m_Direction)
		{
		case MovementDirection::Right:
			if (movementFrame)
			{
				SetActiveTexture(m_Texture_R_01);
			}
			else
			{
				SetActiveTexture(m_Texture_R_02);
			}
			break;
		case MovementDirection::Down:
			if (movementFrame)
			{
				SetActiveTexture(m_Texture_D_01);
			}
			else
			{
				SetActiveTexture(m_Texture_D_02);
			}
			break;
		case MovementDirection::Left:
			if (movementFrame)
			{
				SetActiveTexture(m_Texture_L_01);
			}
			else
			{
				SetActiveTexture(m_Texture_L_02);
			}
			break;
		case MovementDirection::Up:
			if (movementFrame)
			{
				SetActiveTexture(m_Texture_U_01);
			}
			else
			{
				SetActiveTexture(m_Texture_U_02);
			}
			break;
		default:
			break;
		}
		movementFrame = !movementFrame;
		break;
	case PacmanStates::Stationary:
		//Keep the current animation! -> Do nothing
		break;
	case PacmanStates::Dead:
		if (m_DeathFrame > m_LastDeathFrame)
		{
			m_DeathFrame++;

			if (m_DeathFrame > m_LastDeathFrame + 4)
			{

				m_CurrentLoc.x = m_RespawnLoc.x;
				m_CurrentLoc.y = m_RespawnLoc.y;
				
				if (m_CurrentLoc.x <= 0)
				{
					m_CurrentLoc.x = 0;
				}
				if (m_CurrentLoc.x >= 28)
				{
					m_CurrentLoc.x = 27;
				}
				if (m_CurrentLoc.y <= 0)
				{
					m_CurrentLoc.y = 0;
				}
				if (m_CurrentLoc.y >= 30)
				{
					m_CurrentLoc.y = 30;
				}
				auto gridTopLeftPos = (*m_pGrid)[m_CurrentLoc.x][m_CurrentLoc.y]->GetComponent<TransformComponent>()->GetPosition();
				glm::vec3 currentPos = gridTopLeftPos;
				currentPos.x -= 8;
				currentPos.y -= 8;


				GetGameObject()->GetComponent<TransformComponent>()->SetPosition(
					currentPos.x,
					currentPos.y,
					0.0f);
				SetActiveTexture(m_DeathTextures[0]);

				m_DeathFrame = 0;
				--m_Lives;
				if (m_Lives > 0)
				{
					SetState(PacmanStates::Spawning);
				}
				else
				{
					//Player is dead; game over for this player!
					m_IsDead = true;
					GetGameObject()->GetComponent<DrawComponent>()->SetTexture("..\\Data\\Sprites\\Pacman\\Death\\Empty.png");
				}
			}
		}
		else
		{
			SetActiveTexture(m_DeathTextures[m_DeathFrame]);
			m_DeathFrame++;
		}
		break;
	case PacmanStates::Spawning:
		SetActiveTexture(m_DeathTextures[0]);
		break;
	default:
		break;
	}
}

void PacmanComponent::HandleEatingGhost()
{
	auto pos = GetGameObject()->GetComponent<TransformComponent>()->GetPosition();
	for (size_t i = 0; i < m_pGhosts.size(); i++)
	{
		if ((*m_pGhosts[i])->GetComponent<GhostComponent>()->IsScared())
		{
			auto enemyPos = (*m_pGhosts[i])->GetComponent<TransformComponent>()->GetPosition();


			float distance = float(sqrt(pow(enemyPos.x - pos.x, 2) +	pow(enemyPos.y - pos.y, 2) * 1.0));

			if (distance < eatDistance)
			{
				(*m_pGhosts[i])->GetComponent<GhostComponent>()->Kill();
				m_Score += m_ScorePerKill;
			}
		}
	}
}

void PacmanComponent::TriggerCurrentTile()
{
	if (m_CurrentLoc.x <= 0)
	{
		m_CurrentLoc.x = 0;
	}
	if (m_CurrentLoc.x >= 28)
	{
		m_CurrentLoc.x = 27;
	}
	if (m_CurrentLoc.y <= 0)
	{
		m_CurrentLoc.y = 0;
	}
	if (m_CurrentLoc.y >= 30)
	{
		m_CurrentLoc.y = 30;
	}
	int result = (*m_pGrid)[m_CurrentLoc.x][m_CurrentLoc.y]->GetComponent<GridComponent>()->TriggerTile();
	if (result == 1)
	{
		m_Score += m_ScorePerPill;
	}
	else if (result == 2)
	{
		m_Score += m_ScorePerLargePill;
		//Scare all ghosts.
		for (size_t i = 0; i < m_pGhosts.size(); i++)
		{
			(*m_pGhosts[i])->GetComponent<GhostComponent>()->Scare();
		}
	}
}

void PacmanComponent::CheckTeleportation()
{
	for (int i = 0; i < int(m_Teleports.size()); i++)
	{
		if (m_GoalLoc.x == m_Teleports[i].first.x && m_GoalLoc.y == m_Teleports[i].first.y)
		{
			m_CurrentLoc.x = m_Teleports[i].second.x;
			m_CurrentLoc.y = m_Teleports[i].second.y;
		}
	}
}

void PacmanComponent::UpdateDirection()
{
	if (m_CurrentState != PacmanStates::Spawning && m_CurrentState != PacmanStates::Dead)
	{
		auto& input = InputManager::GetInstance();

		if (input.IsPressed(ControllerButton::ButtonB, m_PlayerId) && m_Direction != MovementDirection::Right && m_DistTraversedFromCurrentToGoal <= 0.25f)
		{
			location testingGoalLoc;
			testingGoalLoc.x = m_CurrentLoc.x + 1;
			testingGoalLoc.y = m_CurrentLoc.y;

			if (testingGoalLoc.x <= 0)
			{
				testingGoalLoc.x = 0;
			}
			if (testingGoalLoc.x >= 28)
			{
				testingGoalLoc.x = 27;
			}
			if (testingGoalLoc.y <= 0)
			{
				testingGoalLoc.y = 0;
			}
			if (testingGoalLoc.y >= 30)
			{
				testingGoalLoc.y = 30;
			}
			if (!((*m_pGrid)[testingGoalLoc.x][testingGoalLoc.y]->GetComponent<GridComponent>()->GetIsWall()))
			{
				SetDirection(MovementDirection::Right);
				SetState(PacmanStates::Moving);
				m_DistTraversedFromCurrentToGoal = 0;
			}
		}
		else if (input.IsPressed(ControllerButton::ButtonA, m_PlayerId) && m_Direction != MovementDirection::Down && m_DistTraversedFromCurrentToGoal <= 0.25f)
		{
			location testingGoalLoc;
			testingGoalLoc.x = m_CurrentLoc.x;
			testingGoalLoc.y = m_CurrentLoc.y + 1;

			if (testingGoalLoc.x <= 0)
			{
				testingGoalLoc.x = 0;
			}
			if (testingGoalLoc.x >= 28)
			{
				testingGoalLoc.x = 27;
			}
			if (testingGoalLoc.y <= 0)
			{
				testingGoalLoc.y = 0;
			}
			if (testingGoalLoc.y >= 30)
			{
				testingGoalLoc.y = 30;
			}
			if (!((*m_pGrid)[testingGoalLoc.x][testingGoalLoc.y]->GetComponent<GridComponent>()->GetIsWall()))
			{
				SetDirection(MovementDirection::Down);
				SetState(PacmanStates::Moving);
				m_DistTraversedFromCurrentToGoal = 0;
			}
		}
		else if (input.IsPressed(ControllerButton::ButtonX, m_PlayerId) && m_Direction != MovementDirection::Left && m_DistTraversedFromCurrentToGoal <= 0.25f)
		{
			location testingGoalLoc;
			testingGoalLoc.x = m_CurrentLoc.x - 1;
			testingGoalLoc.y = m_CurrentLoc.y;

			if (testingGoalLoc.x <= 0)
			{
				testingGoalLoc.x = 0;
			}
			if (testingGoalLoc.x >= 28)
			{
				testingGoalLoc.x = 27;
			}
			if (testingGoalLoc.y <= 0)
			{
				testingGoalLoc.y = 0;
			}
			if (testingGoalLoc.y >= 30)
			{
				testingGoalLoc.y = 30;
			}
			if (!((*m_pGrid)[testingGoalLoc.x][testingGoalLoc.y]->GetComponent<GridComponent>()->GetIsWall()))
			{
				SetDirection(MovementDirection::Left);
				SetState(PacmanStates::Moving);
				m_DistTraversedFromCurrentToGoal = 0;
			}
		}
		else if (input.IsPressed(ControllerButton::ButtonY, m_PlayerId) && m_Direction != MovementDirection::Up && m_DistTraversedFromCurrentToGoal <= 0.25f)
		{
			location testingGoalLoc;
			testingGoalLoc.x = m_CurrentLoc.x;
			testingGoalLoc.y = m_CurrentLoc.y - 1;

			if (testingGoalLoc.x <= 0)
			{
				testingGoalLoc.x = 0;
			}
			if (testingGoalLoc.x >= 28)
			{
				testingGoalLoc.x = 27;
			}
			if (testingGoalLoc.y <= 0)
			{
				testingGoalLoc.y = 0;
			}
			if (testingGoalLoc.y >= 30)
			{
				testingGoalLoc.y = 30;
			}
			if (!((*m_pGrid)[testingGoalLoc.x][testingGoalLoc.y]->GetComponent<GridComponent>()->GetIsWall()))
			{
				SetDirection(MovementDirection::Up);
				SetState(PacmanStates::Moving);
				m_DistTraversedFromCurrentToGoal = 0;
			}
		}
	}
}

void PacmanComponent::HandleMovement(float deltaTime)
{

	CheckTeleportation();

	switch (m_Direction)
	{
	case MovementDirection::Right:
		m_GoalLoc.x = m_CurrentLoc.x + 1;
		m_GoalLoc.y = m_CurrentLoc.y;
		break;
	case MovementDirection::Down:
		m_GoalLoc.x = m_CurrentLoc.x;
		m_GoalLoc.y = m_CurrentLoc.y + 1;
		break;
	case MovementDirection::Left:
		m_GoalLoc.x = m_CurrentLoc.x - 1;
		m_GoalLoc.y = m_CurrentLoc.y;
		break;
	case MovementDirection::Up:
		m_GoalLoc.x = m_CurrentLoc.x;
		m_GoalLoc.y = m_CurrentLoc.y - 1;
		break;
	default:
		break;
	}


	if (m_CurrentState == PacmanStates::Moving)
	{
		if (m_GoalLoc.x <= 0)
		{
			m_GoalLoc.x = 0;
		}
		if (m_GoalLoc.x >= 28)
		{
			m_GoalLoc.x = 27;
		}
		if (m_GoalLoc.y <= 0)
		{
			m_GoalLoc.y = 0;
		}
		if (m_GoalLoc.y >= 30)
		{
			m_GoalLoc.y = 30;
		}
		if ((*m_pGrid)[m_GoalLoc.x][m_GoalLoc.y]->GetComponent<GridComponent>()->GetIsWall())
		{
			SetState(PacmanStates::Stationary);
		}
	}
		
	if (m_CurrentLoc.x <= 0)
	{
		m_CurrentLoc.x = 0;
	}
	if (m_CurrentLoc.x >= 28)
	{
		m_CurrentLoc.x = 27;
	}
	if (m_CurrentLoc.y <= 0)
	{
		m_CurrentLoc.y = 0;
	}
	if (m_CurrentLoc.y >= 30)
	{
		m_CurrentLoc.y = 30;
	}
	auto gridTopLeftPos = (*m_pGrid)[m_CurrentLoc.x][m_CurrentLoc.y]->GetComponent<TransformComponent>()->GetPosition();
	
	glm::vec3 currentPos = gridTopLeftPos;
	currentPos.x -= 8;
	currentPos.y -= 8;


	switch (m_CurrentState)
	{
	case PacmanStates::Moving:

		m_DistTraversedFromCurrentToGoal += m_DistancePerSecond * deltaTime;

		switch (m_Direction)
		{
		case MovementDirection::Right:

			GetGameObject()->GetComponent<TransformComponent>()->SetPosition(
				currentPos.x + m_DistTraversedFromCurrentToGoal * m_DistBetweenLocations,
				currentPos.y,
				0.0f);

			if (m_DistTraversedFromCurrentToGoal >= 1.0f)
			{
				m_CurrentLoc.x++;
				TriggerCurrentTile();
				m_GoalLoc.x++;
				m_DistTraversedFromCurrentToGoal -= 1.0f;
			}
			break;
		case MovementDirection::Down:

			GetGameObject()->GetComponent<TransformComponent>()->SetPosition(
				currentPos.x,
				currentPos.y + m_DistTraversedFromCurrentToGoal * m_DistBetweenLocations,
				0.0f);

			if (m_DistTraversedFromCurrentToGoal >= 1.0f)
			{
				m_CurrentLoc.y++;
				TriggerCurrentTile();
				m_GoalLoc.y++;
				m_DistTraversedFromCurrentToGoal -= 1.0f;
			}

			break;
		case MovementDirection::Left:

			GetGameObject()->GetComponent<TransformComponent>()->SetPosition(
				currentPos.x - m_DistTraversedFromCurrentToGoal * m_DistBetweenLocations,
				currentPos.y,
				0.0f);

			if (m_DistTraversedFromCurrentToGoal >= 1.0f)
			{
				m_CurrentLoc.x--;
				TriggerCurrentTile();
				m_GoalLoc.x--;
				m_DistTraversedFromCurrentToGoal -= 1.0f;
			}

			break;
		case MovementDirection::Up:

			GetGameObject()->GetComponent<TransformComponent>()->SetPosition(
				currentPos.x,
				currentPos.y - m_DistTraversedFromCurrentToGoal * m_DistBetweenLocations,
				0.0f);

			if (m_DistTraversedFromCurrentToGoal >= 1.0f)
			{
				m_CurrentLoc.y--;
				TriggerCurrentTile();
				m_GoalLoc.y--;
				m_DistTraversedFromCurrentToGoal -= 1.0f;
			}

			break;
		default:
			break;
		}


		break;
	case PacmanStates::Stationary:
		//Nothing!
		break;
	case PacmanStates::Dead:
		//Nothing!
		break;
	case PacmanStates::Spawning:
		HandleRespawning(deltaTime);
		break;
	default:
		break;
	}
}

void  PacmanComponent::HandleRespawning(float deltaTime)
{
	if (m_CurrentRespawnTime == 0)
	{
		//Set current pos to respawn pos
		m_CurrentLoc.x = m_RespawnLoc.x;
		m_CurrentLoc.y = m_RespawnLoc.y;

	}
	m_CurrentRespawnTime += deltaTime;
	if (m_CurrentRespawnTime >= m_TotalRespawnTime)
	{
		SetState(PacmanStates::Moving);
		SetDirection(MovementDirection::Left);
		m_CurrentRespawnTime = 0.0f;
	}
}

void PacmanComponent::SetIsMsPacman()
{
	m_IsMsPacman = true;
}

void PacmanComponent::SetTextures()
{
	if (!m_IsMsPacman)
	{
		//Facing Right
		m_Texture_R_01 = ResourceManager::GetInstance().LoadTexture("..\\Data\\Sprites\\Pacman\\Movement\\R_01.png");
		m_Texture_R_02 = ResourceManager::GetInstance().LoadTexture("..\\Data\\Sprites\\Pacman\\Movement\\R_02.png");
		//Facing Down
		m_Texture_D_01 = ResourceManager::GetInstance().LoadTexture("..\\Data\\Sprites\\Pacman\\Movement\\D_01.png");
		m_Texture_D_02 = ResourceManager::GetInstance().LoadTexture("..\\Data\\Sprites\\Pacman\\Movement\\D_02.png");
		//Facing Left
		m_Texture_L_01 = ResourceManager::GetInstance().LoadTexture("..\\Data\\Sprites\\Pacman\\Movement\\L_01.png");
		m_Texture_L_02 = ResourceManager::GetInstance().LoadTexture("..\\Data\\Sprites\\Pacman\\Movement\\L_02.png");
		//Facing Up
		m_Texture_U_01 = ResourceManager::GetInstance().LoadTexture("..\\Data\\Sprites\\Pacman\\Movement\\U_01.png");
		m_Texture_U_02 = ResourceManager::GetInstance().LoadTexture("..\\Data\\Sprites\\Pacman\\Movement\\U_02.png");
		//Death
		for (int i = 1; i <= m_LastDeathFrame + 1; i++)
		{
			std::string filePath = "..\\Data\\Sprites\\Pacman\\Death\\";
			if (i<10)
			{
				filePath += "0";
			}
			filePath += std::to_string(int(i));
			filePath += ".png";
			auto texture = ResourceManager::GetInstance().LoadTexture(filePath);
			m_DeathTextures.push_back(texture);
		}
	}
	else
	{
		//Facing Right
		m_Texture_R_01 = ResourceManager::GetInstance().LoadTexture("..\\Data\\Sprites\\MsPacman\\Movement\\R_01.png");
		m_Texture_R_02 = ResourceManager::GetInstance().LoadTexture("..\\Data\\Sprites\\MsPacman\\Movement\\R_02.png");
		//Facing Down
		m_Texture_D_01 = ResourceManager::GetInstance().LoadTexture("..\\Data\\Sprites\\MsPacman\\Movement\\D_01.png");
		m_Texture_D_02 = ResourceManager::GetInstance().LoadTexture("..\\Data\\Sprites\\MsPacman\\Movement\\D_02.png");
		//Facing Left
		m_Texture_L_01 = ResourceManager::GetInstance().LoadTexture("..\\Data\\Sprites\\MsPacman\\Movement\\L_01.png");
		m_Texture_L_02 = ResourceManager::GetInstance().LoadTexture("..\\Data\\Sprites\\MsPacman\\Movement\\L_02.png");
		//Facing Up
		m_Texture_U_01 = ResourceManager::GetInstance().LoadTexture("..\\Data\\Sprites\\MsPacman\\Movement\\U_01.png");
		m_Texture_U_02 = ResourceManager::GetInstance().LoadTexture("..\\Data\\Sprites\\MsPacman\\Movement\\U_02.png");
		//Death
		for (int i = 1; i <= m_LastDeathFrame + 1; i++)
		{
			std::string filePath = "..\\Data\\Sprites\\MsPacman\\Death\\";
			if (i<10)
			{
				filePath += "0";
			}
			filePath += std::to_string(int(i));
			filePath += ".png";
			auto texture = ResourceManager::GetInstance().LoadTexture(filePath);
			m_DeathTextures.push_back(texture);
		}
	}
}

void PacmanComponent::SetActiveTexture(std::shared_ptr<Texture2D> activeTexture)
{
	GetGameObject()->GetComponent<DrawComponent>()->SetTexture(activeTexture);
}

void PacmanComponent::SetState(PacmanStates state)
{
	m_CurrentState = state;
}

void PacmanComponent::SetDirection(MovementDirection dir)
{
	m_Direction = dir;
}

void PacmanComponent::SetPlayerId(int playerId)
{
	m_PlayerId = playerId;
}

void PacmanComponent::SetRespawnLoc(int x, int y)
{
	m_RespawnLoc.x = x;
	m_RespawnLoc.y = y;
}

void PacmanComponent::SetGridReference(std::shared_ptr<std::vector<std::vector<GameObject*>>> pGrid)
{
	m_pGrid = pGrid;
}

void PacmanComponent::AddTeleportLocation(int xFrom, int yFrom, int xTo, int yTo)
{
	std::pair<location, location> pair;
	pair.first.x = xFrom;
	pair.first.y = yFrom;
	pair.second.x = xTo;
	pair.second.y = yTo;
	m_Teleports.push_back(pair);
}

void PacmanComponent::AddGhostRef(std::shared_ptr<GameObject*> ghost)
{
	m_pGhosts.push_back(ghost);
}

void PacmanComponent::Kill()
{
	if (m_CurrentState == PacmanStates::Moving || m_CurrentState == PacmanStates::Stationary)
	{
		m_CurrentState = PacmanStates::Dead;
	}
}

bool PacmanComponent::IsDead()
{
	return m_IsDead;
}

int PacmanComponent::GetScore()
{
	return m_Score;
}

int PacmanComponent::GetLives()
{
	return m_Lives;
}
