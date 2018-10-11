#pragma once
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec3.hpp>
#pragma warning(pop)
#include "BaseComponent.h"
#include <vector>
#include "..\Texture2D.h"

struct location
{
	int x;
	int y;
};

enum class PacmanStates : int
{
	Moving = 0,
	Stationary = 1,
	Dead = 2,
	Spawning = 3
};

enum class MovementDirection : int
{
	Right = 0,
	Down = 1,
	Left = 2,
	Up = 3
};

class PacmanComponent : public BaseComponent
{
public:
	PacmanComponent();
	virtual ~PacmanComponent();
	void Initialize() {};
	void Update(float deltaTime);
	void Draw() {};

	void SetIsMsPacman();
	void SetTextures();
	void SetPlayerId(int playerId);
	void SetRespawnLoc(int x, int y);
	void SetGridReference(std::shared_ptr<std::vector<std::vector<GameObject*>>> pGrid);
	void AddTeleportLocation(int xFrom, int yFrom, int xTo, int yTo);
	void AddGhostRef(std::shared_ptr<GameObject*> ghost);
	
	void Kill();
	bool IsDead();
	int GetScore();
	int GetLives();

private:

	//FUNCTIONS:
	void HandleEatingGhost(); //If a ghost is scared, and close enough, eat it!
	void TriggerCurrentTile(); //Trigger the tile pacman is currently on -> eat a pill if it's there
	void CheckTeleportation(); //Check if pacman is on a teleportation square, if so, use it!
	void UpdateDirection(); //Update direction depending on input
	void SetDirection(MovementDirection dir); //Helper for setting direction
	void HandleMovement(float deltaTime); //Handles Pacman's movement
	void HandleRespawning(float deltaTime); //Handles respawning
	void UpdateTexture(); //Update texture of DrawComponent to the one it should be
	void SetActiveTexture(std::shared_ptr<Texture2D> activeTexture); //Helper for updating textures
	void SetState(PacmanStates state); //Helper for setting pacman's state

	//VARIABLES

	//Ghosts Refs
	std::vector<std::shared_ptr<GameObject*>> m_pGhosts;
	float eatDistance = 8.0f;

	//Player ID
	int m_PlayerId = 0;
	bool m_IsMsPacman = false;

	//States
	PacmanStates m_CurrentState = PacmanStates::Spawning;

	//Life related
	int m_Lives = 3;
	bool m_IsDead = false;

	//Score Related
	int m_Score = 0;
	int m_ScorePerPill = 10;
	int m_ScorePerLargePill = 50;
	int m_ScorePerKill = 200;

	//Teleportation Related
	std::vector<std::pair<location, location>> m_Teleports;

	//Movement Related
	MovementDirection m_Direction = MovementDirection::Left;
	std::shared_ptr<std::vector<std::vector<GameObject*>>> m_pGrid;

	location m_CurrentLoc;
	location m_GoalLoc;
	location m_RespawnLoc;

	float m_DistTraversedFromCurrentToGoal = 0; //From 0 to 1
	float m_DistBetweenLocations = 16;
	float m_DistancePerSecond = 8.0f;
	
	//Respawn Related
	float m_TotalRespawnTime = 2.0f;
	float m_CurrentRespawnTime = 0.0f;

	//Texture Related
	bool movementFrame = false; //there are only 2 movementFrames, false -> 01, true -> 02
	//Facing Right
	std::shared_ptr<Texture2D> m_Texture_R_01;
	std::shared_ptr<Texture2D> m_Texture_R_02;
	//Facing Down
	std::shared_ptr<Texture2D> m_Texture_D_01;
	std::shared_ptr<Texture2D> m_Texture_D_02;
	//Facing Left
	std::shared_ptr<Texture2D> m_Texture_L_01;
	std::shared_ptr<Texture2D> m_Texture_L_02;
	//Facing Up
	std::shared_ptr<Texture2D> m_Texture_U_01;
	std::shared_ptr<Texture2D> m_Texture_U_02;

	//Death
	std::vector<std::shared_ptr<Texture2D>> m_DeathTextures;
	int m_DeathFrame = 0;
	int m_LastDeathFrame = 11; //There are 12 DeathSprites, it cycles between them using these variables
	
	float m_TimeSinceLastAnimChange = 0; //Keeps track of the time passed since the last animation update
};