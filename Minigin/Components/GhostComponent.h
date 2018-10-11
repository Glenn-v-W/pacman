#pragma once
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec3.hpp>
#pragma warning(pop)
#include "BaseComponent.h"
#include <vector>
#include "..\Texture2D.h"

struct ghostLocation
{
	int x;
	int y;
};

enum class GhostStates : int
{
	Roaming = 0,
	Scared = 1,
	Dead = 2,
	Spawning = 3,
	Stationary = 4 //For when player-controlled
};

enum class GhostMovementDirection : int
{
	Right = 0,
	Down = 1,
	Left = 2,
	Up = 3
};

class GhostComponent : public BaseComponent
{
public:
	GhostComponent();
	virtual ~GhostComponent();
	void Initialize() {};
	void Update(float deltaTime);
	void Draw() {};

	void SetTextures();
	void SetPlayerId(int playerId);
	void SetGhostId(int ghostId);
	void SetRespawnLoc(int x, int y);
	void SetGridReference(std::shared_ptr<std::vector<std::vector<GameObject*>>> pGrid);
	void AddTeleportLocation(int xFrom, int yFrom, int xTo, int yTo);
	void AddPacmanRef(std::shared_ptr<GameObject*> pacman);
	void Scare();
	bool IsScared();
	void Kill();

private:

	//FUNCTIONS 
	void HandleScared(float deltaTime); //Handles everything related to ghosts being scared
	void HandleEatingPacman(); //If close enough to pacman & not scared -> eat pacman
	void CheckTeleportation(); //Check if ghost is on a teleportation square, if so, use it!
	void SetDirection(GhostMovementDirection dir); //Update direction depending on input, or do simple AI if not player-controlled
	void UpdateDirection(); //Helper for setting direction
	void HandleMovement(float deltaTime); //Handles Pacman's movement
	void UpdateTexture(); //Update texture of DrawComponent to the one it should be
	void SetActiveTexture(std::shared_ptr<Texture2D> activeTexture); //Helper for updating textures
	void SetState(GhostStates state); //Helper for setting ghost's state


	//VARIABLES

	//Ghost ID
	int m_GhostId = 0;

	//Player ID
	int m_PlayerId = 9999;

	//States
	GhostStates m_CurrentState = GhostStates::Dead;

	//Pacmans Ref
	std::vector<std::shared_ptr<GameObject*>> m_pPacmans;
	float eatDistance = 8.0f;

	//Scared
	float m_TotalScaredTime = 10.0f;
	float m_CurrentScaredTime = 0.0f;

	//Teleportation Related
	std::vector<std::pair<ghostLocation, ghostLocation>> m_Teleports;

	//Movement Related
	GhostMovementDirection m_Direction = GhostMovementDirection::Left;

	std::shared_ptr<std::vector<std::vector<GameObject*>>> m_pGrid;

	ghostLocation m_CurrentLoc;
	ghostLocation m_GoalLoc;
	ghostLocation m_RespawnLoc;

	float m_DistTraversedFromCurrentToGoal = 0; //From 0 to 1
	float m_DistBetweenLocations = 16;
	float m_DistancePerSecond = 8.0f;
	
	//Respawn Related
	void HandleRespawning(float deltaTime);
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
	//Respawning
	std::shared_ptr<Texture2D> m_Texture_R_Eyes;
	std::shared_ptr<Texture2D> m_Texture_D_Eyes;
	std::shared_ptr<Texture2D> m_Texture_L_Eyes;
	std::shared_ptr<Texture2D> m_Texture_U_Eyes;
	//Scared
	std::shared_ptr<Texture2D> m_Texture_Scared_01;
	std::shared_ptr<Texture2D> m_Texture_Scared_02;

	//Keeps track of the time passed since the last animation update
	float m_TimeSinceLastAnimChange = 0;
};