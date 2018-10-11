#include "MiniginPCH.h"
#include "GridComponent.h"
#include "DrawComponent.h"
#include "..\Scenegraph\GameObject.h"
#include <string>
#include <iostream>

GridComponent::GridComponent()
{}

GridComponent::~GridComponent()
{}

void GridComponent::Update(float deltaTime)
{
	UNREFERENCED_PARAMETER(deltaTime);
	if (firstUpdate)
	{
		SetTexture();
		firstUpdate = false;
	}
}

void GridComponent::SetTileType(TileType tileType)
{
	m_TileType = tileType;

	m_TileFilePath = "..\\Data\\Sprites\\Grid\\GridTiles_";
	if (int(m_TileType)<10)
	{
		m_TileFilePath += "0";
	}
	m_TileFilePath += std::to_string(int(m_TileType));
	m_TileFilePath += ".png";

	//std::cout << g_TileFilePath << "\n";

	SetIsWall();
}

bool GridComponent::GetIsWall()
{
	return m_IsWall;
}

int GridComponent::TriggerTile()
{
	auto tileType = m_TileType;
	if (m_TileType == TileType::GridTiles_46 || m_TileType == TileType::GridTiles_47 || m_TileType == TileType::GridTiles_48)
	{
		SetTileType(TileType::GridTiles_45);
		SetTexture();

		if (tileType == TileType::GridTiles_48)
		{
			return 2; //Powerup that scares ghosts
		}
		return 1; //Pills for score
	}
	return 0; //Nothing happened!
}

void GridComponent::SetTexture()
{
	GetGameObject()->GetComponent<DrawComponent>()->SetTexture(m_TileFilePath);
}

void GridComponent::SetIsWall()
{
	if (m_TileType == TileType::GridTiles_45 || m_TileType == TileType::GridTiles_46 || m_TileType == TileType::GridTiles_47 || m_TileType == TileType::GridTiles_48)
	{
		m_IsWall = false;
	}
	else
	{
		m_IsWall = true;
	}
}

