#include "MiniginPCH.h"
#include "GhostComponent.h"
#include "DrawComponent.h"
#include "TransformComponent.h"
#include "GridComponent.h"
#include "PacmanComponent.h"
#include "..\Scenegraph\GameObject.h"
#include "..\ResourceManager.h"
#include "..\InputManager.h"
#include <string>

GhostComponent::GhostComponent()
{}

GhostComponent::~GhostComponent()
{}

void GhostComponent::Update(float deltaTime)
{

	UpdateDirection();
	HandleMovement(deltaTime);
	HandleScared(deltaTime);
	HandleEatingPacman();

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

void GhostComponent::UpdateTexture()
{
	switch (m_CurrentState)
	{
	case GhostStates::Roaming:
		switch (m_Direction)
		{
		case GhostMovementDirection::Right:
			if (movementFrame)
			{
				SetActiveTexture(m_Texture_R_01);
			}
			else
			{
				SetActiveTexture(m_Texture_R_02);
			}
			break;
		case GhostMovementDirection::Down:
			if (movementFrame)
			{
				SetActiveTexture(m_Texture_D_01);
			}
			else
			{
				SetActiveTexture(m_Texture_D_02);
			}
			break;
		case GhostMovementDirection::Left:
			if (movementFrame)
			{
				SetActiveTexture(m_Texture_L_01);
			}
			else
			{
				SetActiveTexture(m_Texture_L_02);
			}
			break;
		case GhostMovementDirection::Up:
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
	case GhostStates::Scared:
		if (movementFrame)
		{
			SetActiveTexture(m_Texture_Scared_01);
		}
		else
		{
			SetActiveTexture(m_Texture_Scared_02);
		}
		movementFrame = !movementFrame;
		break;
	case GhostStates::Dead:
		
		//Handle Death timer

		if (true)
		{
			SetState(GhostStates::Spawning);
		}

		switch (m_Direction)
		{
		case GhostMovementDirection::Right:
			SetActiveTexture(m_Texture_R_Eyes);
			break;
		case GhostMovementDirection::Down:
			SetActiveTexture(m_Texture_D_Eyes);
			break;
		case GhostMovementDirection::Left:
			SetActiveTexture(m_Texture_L_Eyes);
			break;
		case GhostMovementDirection::Up:
			SetActiveTexture(m_Texture_U_Eyes);
			break;
		default:
			break;
		}

		break;
	case GhostStates::Spawning:
		//Do nothing
		break;
	default:
		break;
	}
}

void GhostComponent::CheckTeleportation()
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

void GhostComponent::UpdateDirection()
{
	auto& input = InputManager::GetInstance();

	if (input.IsPressed(ControllerButton::ButtonB, m_PlayerId) && m_Direction != GhostMovementDirection::Right && m_DistTraversedFromCurrentToGoal <= 0.25f)
	{
		ghostLocation testingGoalLoc;
		testingGoalLoc.x = m_CurrentLoc.x + 1;
		testingGoalLoc.y = m_CurrentLoc.y;

		if (!((*m_pGrid)[testingGoalLoc.x][testingGoalLoc.y]->GetComponent<GridComponent>()->GetIsWall()))
		{
			SetDirection(GhostMovementDirection::Right);
			m_DistTraversedFromCurrentToGoal = 0;
		}
	}
	else if (input.IsPressed(ControllerButton::ButtonA, m_PlayerId) && m_Direction != GhostMovementDirection::Down && m_DistTraversedFromCurrentToGoal <= 0.25f)
	{
		ghostLocation testingGoalLoc;
		testingGoalLoc.x = m_CurrentLoc.x ;
		testingGoalLoc.y = m_CurrentLoc.y + 1;

		if (!((*m_pGrid)[testingGoalLoc.x][testingGoalLoc.y]->GetComponent<GridComponent>()->GetIsWall()))
		{
			SetDirection(GhostMovementDirection::Down);
			m_DistTraversedFromCurrentToGoal = 0;
		}
	}
	else if (input.IsPressed(ControllerButton::ButtonX, m_PlayerId) && m_Direction != GhostMovementDirection::Left && m_DistTraversedFromCurrentToGoal <= 0.25f)
	{
		ghostLocation testingGoalLoc;
		testingGoalLoc.x = m_CurrentLoc.x - 1;
		testingGoalLoc.y = m_CurrentLoc.y;

		if (!((*m_pGrid)[testingGoalLoc.x][testingGoalLoc.y]->GetComponent<GridComponent>()->GetIsWall()))
		{
			SetDirection(GhostMovementDirection::Left);
			m_DistTraversedFromCurrentToGoal = 0;
		}
	}
	else if (input.IsPressed(ControllerButton::ButtonY, m_PlayerId) && m_Direction != GhostMovementDirection::Up && m_DistTraversedFromCurrentToGoal <= 0.25f)
	{
		ghostLocation testingGoalLoc;
		testingGoalLoc.x = m_CurrentLoc.x;
		testingGoalLoc.y = m_CurrentLoc.y - 1;

		if (!((*m_pGrid)[testingGoalLoc.x][testingGoalLoc.y]->GetComponent<GridComponent>()->GetIsWall()))
		{
			SetDirection(GhostMovementDirection::Up);
			m_DistTraversedFromCurrentToGoal = 0;
		}
	}
}

void GhostComponent::HandleMovement(float deltaTime)
{

	CheckTeleportation();

	switch (m_Direction)
	{
	case GhostMovementDirection::Right:
		m_GoalLoc.x = m_CurrentLoc.x + 1;
		m_GoalLoc.y = m_CurrentLoc.y;
		break;
	case GhostMovementDirection::Down:
		m_GoalLoc.x = m_CurrentLoc.x;
		m_GoalLoc.y = m_CurrentLoc.y + 1;
		break;
	case GhostMovementDirection::Left:
		m_GoalLoc.x = m_CurrentLoc.x - 1;
		m_GoalLoc.y = m_CurrentLoc.y;
		break;
	case GhostMovementDirection::Up:
		m_GoalLoc.x = m_CurrentLoc.x;
		m_GoalLoc.y = m_CurrentLoc.y - 1;
		break;
	default:
		break;
	}

	if (m_CurrentState == GhostStates::Roaming || m_CurrentState == GhostStates::Scared)
	{
		//If this should be AI controlled
		if (m_PlayerId != 0 && m_PlayerId != 1)
		{

			//If I'm about to run into a wall
			if ((*m_pGrid)[m_GoalLoc.x][m_GoalLoc.y]->GetComponent<GridComponent>()->GetIsWall())
			{
				int number = rand() % 2;

				//Change direction!!
				switch (m_Direction)
				{
				case GhostMovementDirection::Right:
					//Up & Down are possible -> pick random
					if (!((*m_pGrid)[m_CurrentLoc.x][m_CurrentLoc.y - 1]->GetComponent<GridComponent>()->GetIsWall())
						&&
						!((*m_pGrid)[m_CurrentLoc.x][m_CurrentLoc.y + 1]->GetComponent<GridComponent>()->GetIsWall()))
					{
						if (number == 0)
						{
							SetDirection(GhostMovementDirection::Up);
						}
						else
						{
							SetDirection(GhostMovementDirection::Down);
						}
					}
					//Else, do the only possible one
					else if (!((*m_pGrid)[m_CurrentLoc.x][m_CurrentLoc.y - 1]->GetComponent<GridComponent>()->GetIsWall()))
					{
						SetDirection(GhostMovementDirection::Up);
					}
					else
					{
						SetDirection(GhostMovementDirection::Down);
					}
					break;
				case GhostMovementDirection::Down:
					//Up & Down are possible -> pick random
					if (!((*m_pGrid)[m_CurrentLoc.x - 1][m_CurrentLoc.y]->GetComponent<GridComponent>()->GetIsWall())
						&&
						!((*m_pGrid)[m_CurrentLoc.x + 1][m_CurrentLoc.y]->GetComponent<GridComponent>()->GetIsWall()))
					{
						if (number == 0)
						{
							SetDirection(GhostMovementDirection::Left);
						}
						else
						{
							SetDirection(GhostMovementDirection::Right);
						}
					}
					//Else, do the only possible one
					else if (!((*m_pGrid)[m_CurrentLoc.x - 1][m_CurrentLoc.y]->GetComponent<GridComponent>()->GetIsWall()))
					{
						SetDirection(GhostMovementDirection::Left);
					}
					else
					{
						SetDirection(GhostMovementDirection::Right);
					}
					break;
				case GhostMovementDirection::Left:
					//Up & Down are possible -> pick random
					if (!((*m_pGrid)[m_CurrentLoc.x][m_CurrentLoc.y - 1]->GetComponent<GridComponent>()->GetIsWall())
						&&
						!((*m_pGrid)[m_CurrentLoc.x][m_CurrentLoc.y + 1]->GetComponent<GridComponent>()->GetIsWall()))
					{
						if (number == 0)
						{
							SetDirection(GhostMovementDirection::Up);
						}
						else
						{
							SetDirection(GhostMovementDirection::Down);
						}
					}
					//Else, do the only possible one
					else if (!((*m_pGrid)[m_CurrentLoc.x][m_CurrentLoc.y - 1]->GetComponent<GridComponent>()->GetIsWall()))
					{
						SetDirection(GhostMovementDirection::Up);
					}
					else
					{
						SetDirection(GhostMovementDirection::Down);
					}
					break;
				case GhostMovementDirection::Up:
					//Up & Down are possible -> pick random
					if (!((*m_pGrid)[m_CurrentLoc.x - 1][m_CurrentLoc.y]->GetComponent<GridComponent>()->GetIsWall())
						&&
						!((*m_pGrid)[m_CurrentLoc.x + 1][m_CurrentLoc.y]->GetComponent<GridComponent>()->GetIsWall()))
					{
						if (number == 0)
						{
							SetDirection(GhostMovementDirection::Left);
						}
						else
						{
							SetDirection(GhostMovementDirection::Right);
						}
					}
					//Else, do the only possible one
					else if (!((*m_pGrid)[m_CurrentLoc.x - 1][m_CurrentLoc.y]->GetComponent<GridComponent>()->GetIsWall()))
					{
						SetDirection(GhostMovementDirection::Left);
					}
					else
					{
						SetDirection(GhostMovementDirection::Right);
					}
					break;
				default:
					break;
				}
				return;
			}
		}
		else
		{
			//It's player-controlled: don't randomly choose a direction to go when hitting a wall
			if ((*m_pGrid)[m_GoalLoc.x][m_GoalLoc.y]->GetComponent<GridComponent>()->GetIsWall())
			{
				m_DistTraversedFromCurrentToGoal = 0;
			}
		}
	}
		
	auto gridTopLeftPos = (*m_pGrid)[m_CurrentLoc.x][m_CurrentLoc.y]->GetComponent<TransformComponent>()->GetPosition();
	glm::vec3 currentPos = gridTopLeftPos;
	currentPos.x -= 8;
	currentPos.y -= 8;


	switch (m_CurrentState)
	{
	case GhostStates::Roaming:

		m_DistTraversedFromCurrentToGoal += m_DistancePerSecond * deltaTime;

		switch (m_Direction)
		{
		case GhostMovementDirection::Right:

			GetGameObject()->GetComponent<TransformComponent>()->SetPosition(
				currentPos.x + m_DistTraversedFromCurrentToGoal * m_DistBetweenLocations,
				currentPos.y,
				0.0f);

			if (m_DistTraversedFromCurrentToGoal >= 1.0f)
			{
				m_CurrentLoc.x++;
				m_GoalLoc.x++;
				m_DistTraversedFromCurrentToGoal -= 1.0f;
			}
			break;
		case GhostMovementDirection::Down:

			GetGameObject()->GetComponent<TransformComponent>()->SetPosition(
				currentPos.x,
				currentPos.y + m_DistTraversedFromCurrentToGoal * m_DistBetweenLocations,
				0.0f);

			if (m_DistTraversedFromCurrentToGoal >= 1.0f)
			{
				m_CurrentLoc.y++;
				m_GoalLoc.y++;
				m_DistTraversedFromCurrentToGoal -= 1.0f;
			}

			break;
		case GhostMovementDirection::Left:

			GetGameObject()->GetComponent<TransformComponent>()->SetPosition(
				currentPos.x - m_DistTraversedFromCurrentToGoal * m_DistBetweenLocations,
				currentPos.y,
				0.0f);

			if (m_DistTraversedFromCurrentToGoal >= 1.0f)
			{
				m_CurrentLoc.x--;
				m_GoalLoc.x--;
				m_DistTraversedFromCurrentToGoal -= 1.0f;
			}

			break;
		case GhostMovementDirection::Up:

			GetGameObject()->GetComponent<TransformComponent>()->SetPosition(
				currentPos.x,
				currentPos.y - m_DistTraversedFromCurrentToGoal * m_DistBetweenLocations,
				0.0f);

			if (m_DistTraversedFromCurrentToGoal >= 1.0f)
			{
				m_CurrentLoc.y--;
				m_GoalLoc.y--;
				m_DistTraversedFromCurrentToGoal -= 1.0f;
			}
			break;
		default:
			break;
		}
		break;
	case GhostStates::Scared:

		m_DistTraversedFromCurrentToGoal += m_DistancePerSecond * deltaTime;

		switch (m_Direction)
		{
		case GhostMovementDirection::Right:

			GetGameObject()->GetComponent<TransformComponent>()->SetPosition(
				currentPos.x + m_DistTraversedFromCurrentToGoal * m_DistBetweenLocations,
				currentPos.y,
				0.0f);

			if (m_DistTraversedFromCurrentToGoal >= 1.0f)
			{
				m_CurrentLoc.x++;
				m_GoalLoc.x++;
				m_DistTraversedFromCurrentToGoal -= 1.0f;
			}
			break;
		case GhostMovementDirection::Down:

			GetGameObject()->GetComponent<TransformComponent>()->SetPosition(
				currentPos.x,
				currentPos.y + m_DistTraversedFromCurrentToGoal * m_DistBetweenLocations,
				0.0f);

			if (m_DistTraversedFromCurrentToGoal >= 1.0f)
			{
				m_CurrentLoc.y++;
				m_GoalLoc.y++;
				m_DistTraversedFromCurrentToGoal -= 1.0f;
			}

			break;
		case GhostMovementDirection::Left:

			GetGameObject()->GetComponent<TransformComponent>()->SetPosition(
				currentPos.x - m_DistTraversedFromCurrentToGoal * m_DistBetweenLocations,
				currentPos.y,
				0.0f);

			if (m_DistTraversedFromCurrentToGoal >= 1.0f)
			{
				m_CurrentLoc.x--;
				m_GoalLoc.x--;
				m_DistTraversedFromCurrentToGoal -= 1.0f;
			}

			break;
		case GhostMovementDirection::Up:

			GetGameObject()->GetComponent<TransformComponent>()->SetPosition(
				currentPos.x,
				currentPos.y - m_DistTraversedFromCurrentToGoal * m_DistBetweenLocations,
				0.0f);

			if (m_DistTraversedFromCurrentToGoal >= 1.0f)
			{
				m_CurrentLoc.y--;
				m_GoalLoc.y--;
				m_DistTraversedFromCurrentToGoal -= 1.0f;
			}
			break;
		default:
			break;
		}
		break;
	case GhostStates::Dead:
		//Nothing!
		break;
	case GhostStates::Spawning:
		HandleRespawning(deltaTime);
		break;
	default:
		break;
	}
}

void  GhostComponent::HandleRespawning(float deltaTime)
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
		SetState(GhostStates::Roaming);
		SetDirection(GhostMovementDirection::Left);
		m_CurrentRespawnTime = 0.0f;
	}
}

void GhostComponent::SetTextures()
{
	std::string filePath = "..\\Data\\Sprites\\Ghosts\\";
	std::string specificGhostFilePath = filePath;

	switch (m_GhostId)
	{
	case 0:
		specificGhostFilePath += "Cyan\\";
		break;
	case 1:
		specificGhostFilePath += "Orange\\";
		break;
	case 2:
		specificGhostFilePath += "Pink\\";
		break;
	case 3:
		specificGhostFilePath += "Red\\";
		break;
	default:
		break;
	}

	//Facing Right
	std::string tempFilePath = specificGhostFilePath;
	tempFilePath += "R_01.png";
	m_Texture_R_01 = ResourceManager::GetInstance().LoadTexture(tempFilePath);

	tempFilePath = specificGhostFilePath;
	tempFilePath += "R_02.png";
	m_Texture_R_02 = ResourceManager::GetInstance().LoadTexture(tempFilePath);

	//Facing Down
	tempFilePath = specificGhostFilePath;
	tempFilePath += "D_01.png";
	m_Texture_D_01 = ResourceManager::GetInstance().LoadTexture(tempFilePath);

	tempFilePath = specificGhostFilePath;
	tempFilePath += "D_02.png";
	m_Texture_D_02 = ResourceManager::GetInstance().LoadTexture(tempFilePath);

	//Facing Left
	tempFilePath = specificGhostFilePath;
	tempFilePath += "L_01.png";
	m_Texture_L_01 = ResourceManager::GetInstance().LoadTexture(tempFilePath);

	tempFilePath = specificGhostFilePath;
	tempFilePath += "L_02.png";
	m_Texture_L_02 = ResourceManager::GetInstance().LoadTexture(tempFilePath);

	//Facing Up
	tempFilePath = specificGhostFilePath;
	tempFilePath += "U_01.png";
	m_Texture_U_01 = ResourceManager::GetInstance().LoadTexture(tempFilePath);

	tempFilePath = specificGhostFilePath;
	tempFilePath += "U_02.png";
	m_Texture_U_02 = ResourceManager::GetInstance().LoadTexture(tempFilePath);

	//Scared
	tempFilePath = filePath;
	tempFilePath += "Scared\\Blue_01.png";
	m_Texture_Scared_01 = ResourceManager::GetInstance().LoadTexture(tempFilePath);

	tempFilePath = filePath;
	tempFilePath += "Scared\\Blue_02.png";
	m_Texture_Scared_02 = ResourceManager::GetInstance().LoadTexture(tempFilePath);

	//Eyes
	filePath += "Respawning\\";

	tempFilePath = filePath;
	tempFilePath += "R.png";
	m_Texture_R_Eyes = ResourceManager::GetInstance().LoadTexture(tempFilePath);

	tempFilePath = filePath;
	tempFilePath += "D.png";
	m_Texture_D_Eyes = ResourceManager::GetInstance().LoadTexture(tempFilePath);

	tempFilePath = filePath;
	tempFilePath += "L.png";
	m_Texture_L_Eyes = ResourceManager::GetInstance().LoadTexture(tempFilePath);

	tempFilePath = filePath;
	tempFilePath += "U.png";
	m_Texture_U_Eyes = ResourceManager::GetInstance().LoadTexture(tempFilePath);
}

void GhostComponent::SetActiveTexture(std::shared_ptr<Texture2D> activeTexture)
{
	GetGameObject()->GetComponent<DrawComponent>()->SetTexture(activeTexture);
}

void GhostComponent::SetState(GhostStates state)
{
	m_CurrentState = state;
}

void GhostComponent::SetDirection(GhostMovementDirection dir)
{
	m_Direction = dir;
}

void GhostComponent::SetPlayerId(int playerId)
{
	m_PlayerId = playerId;
}

void GhostComponent::SetGhostId(int ghostId)
{
	m_GhostId = ghostId;
}

void GhostComponent::SetRespawnLoc(int x, int y)
{
	m_RespawnLoc.x = x;
	m_RespawnLoc.y = y;
}

void GhostComponent::SetGridReference(std::shared_ptr<std::vector<std::vector<GameObject*>>> pGrid)
{
	m_pGrid = pGrid;
}

void GhostComponent::AddTeleportLocation(int xFrom, int yFrom, int xTo, int yTo)
{
	std::pair<ghostLocation, ghostLocation> pair;
	pair.first.x = xFrom;
	pair.first.y = yFrom;
	pair.second.x = xTo;
	pair.second.y = yTo;
	m_Teleports.push_back(pair);
}

void GhostComponent::AddPacmanRef(std::shared_ptr<GameObject*> pacman)
{
	m_pPacmans.push_back(pacman);
}

void GhostComponent::Scare()
{
	SetState(GhostStates::Scared);
}

bool GhostComponent::IsScared()
{
	if (m_CurrentState == GhostStates::Scared)
	{
		return true;
	}
	return false;
}

void GhostComponent::Kill()
{
	m_CurrentState = GhostStates::Dead;
}

void GhostComponent::HandleScared(float deltaTime)
{
	if (m_CurrentState == GhostStates::Scared)
	{
		m_CurrentScaredTime += deltaTime;
		if (m_CurrentScaredTime >= m_TotalScaredTime)
		{
			m_CurrentState = GhostStates::Roaming;
			m_CurrentScaredTime = 0;
		}
	}
}

void GhostComponent::HandleEatingPacman()
{
	if (m_CurrentState == GhostStates::Roaming)
	{
		auto pos = GetGameObject()->GetComponent<TransformComponent>()->GetPosition();
		for (size_t i = 0; i < m_pPacmans.size(); i++)
		{
			auto enemyPos = (*m_pPacmans[i])->GetComponent<TransformComponent>()->GetPosition();


			float distance = float(sqrt(pow(enemyPos.x - pos.x, 2) + pow(enemyPos.y - pos.y, 2) * 1.0));

			if (distance < eatDistance)
			{
				(*m_pPacmans[i])->GetComponent<PacmanComponent>()->Kill();
			}
		}
	}
}