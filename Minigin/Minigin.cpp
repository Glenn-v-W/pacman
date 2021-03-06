#include "MiniginPCH.h"
// ReSharper disable once CppUnusedIncludeDirective
#include <vld.h>
#pragma comment(lib,"xinput.lib")
#include "SDL.h"
#include "Drawer.h"
#include <chrono>
#include <thread>
#include <vector>

#include "ResourceManager.h"
#include "InputManager.h"

#include "SceneGraph\SceneManager.h"
#include "SceneGraph\Scene.h"
#include "SceneGraph\GameObject.h"

#include "Components\TransformComponent.h"
#include "Components\TextComponent.h"
#include "Components\DrawComponent.h"
#include "Components\FPSComponent.h"
#include "Components\GridComponent.h"
#include "Components\PacmanComponent.h"
#include "Components\GhostComponent.h"
#include "Components\MenuComponent.h"
#include "Components\GameModeComponent.h"
#include "Components\ScoreComponent.h"
#include "Components\LivesComponent.h"

#include "BinaryReadWrite\BinaryWriter.h"
#include "BinaryReadWrite\BinaryReader.h"

const int msPerFrame = 16; //16 for 60 fps, 33 for 30 fps
SDL_Window* window;

void Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::stringstream ss; ss << "SDL_Init Error: " << SDL_GetError();
		throw std::runtime_error(ss.str().c_str());
	}

	window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		648,
		696,
		SDL_WINDOW_OPENGL
	);
	if (window == nullptr) {
		std::stringstream ss; ss << "SDL_CreateWindow Error: " << SDL_GetError();
		throw std::runtime_error(ss.str().c_str());
	}

	Drawer::GetInstance().Init(window);

	srand(unsigned int(time(NULL)));
}

void LoadFpsDemo()
{
	//auto& scene = SceneManager::GetInstance().CreateScene("Demo");
	//
	//auto go = std::make_shared<GameObject>();
	//go->SetTexture("background.jpg");
	//scene.Add(go);

	//go = std::make_shared<GameObject>();
	//go->SetTexture("logo.png");
	//go->SetPosition(216, 180);
	//scene.Add(go);

	//auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	//auto to = std::make_shared<TextObject>("Programming 4 Assignment", font);
	//to->SetPosition(80, 20);
	//scene.Add(to);

	//FPS =  1.0f / deltaTime;

	//auto fps = std::make_shared<TextObject>(std::to_string(1.0f / deltaTime), font);
	//scene.Add(fps);
	//t += std::chrono::milliseconds(msPerFrame);
	//std::this_thread::sleep_until(t);


	//Initialize Scene
	auto& scene = SceneManager::GetInstance().CreateScene("FPSDemo");

	//Create Background
	GameObject* backgroundObject = new GameObject();

	auto pTransformComponent2 = new TransformComponent();
	pTransformComponent2->SetPosition(0.0f, 0.0f, 0.0f);
	backgroundObject->AddComponent(pTransformComponent2);

	auto pDrawComponent2 = new DrawComponent();
	pDrawComponent2->SetTexture("background.jpg");
	backgroundObject->AddComponent(pDrawComponent2);

	scene.Add(backgroundObject);

	//Create Logo
	GameObject* logoObject = new GameObject();

	auto pTransformComponent3 = new TransformComponent();
	pTransformComponent3->SetPosition(216.0f, 180.0f, 0.0f);
	logoObject->AddComponent(pTransformComponent3);

	auto pDrawComponent3 = new DrawComponent();
	pDrawComponent3->SetTexture("logo.png");
	logoObject->AddComponent(pDrawComponent3);

	scene.Add(logoObject);

	//Create Text
	GameObject* textObject = new GameObject();

	auto pTransformComponent4 = new TransformComponent();
	pTransformComponent4->SetPosition(80.0f, 20.0f, 0.0f);
	textObject->AddComponent(pTransformComponent4);

	auto pDrawComponent4 = new DrawComponent();
	textObject->AddComponent(pDrawComponent4);

	auto pTextComponent4 = new TextComponent();
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	pTextComponent4->SetFont(font);
	pTextComponent4->SetText("Programming 4 Assignment");
	textObject->AddComponent(pTextComponent4);

	scene.Add(textObject);

	//Create FPS
	GameObject* fpsObject = new GameObject();

	auto pTransformComponent = new TransformComponent();
	pTransformComponent->SetPosition(0.0f, 0.0f, 0.0f);
	fpsObject->AddComponent(pTransformComponent);

	auto pTextComponent = new TextComponent();
	pTextComponent->SetFont(font);
	fpsObject->AddComponent(pTextComponent);

	auto pDrawComponent = new DrawComponent();
	fpsObject->AddComponent(pDrawComponent);

	auto pFPSComponent = new FPSComponent();
	fpsObject->AddComponent(pFPSComponent);

	scene.Add(fpsObject);
}

void WritePacmanLevelToFile()
{
	std::vector<std::vector<int>> tiles{
	{  2,  4,  4,  4,  4,  4,  4,  4,  4,  6, 45, 45, 45, 12, 45, 13, 45, 45, 45,  2,  4,  4,  4,  4,  8, 10,  4,  4,  4,  4,  6 },
	{ 11, 46, 46, 48, 46, 46, 46, 46, 46, 13, 45, 45, 45, 12, 45, 13, 45, 45, 45, 12, 46, 46, 46, 48, 16, 21, 46, 46, 46, 46, 13 },
	{ 11, 46, 24, 26, 28, 46, 24, 28, 46, 13, 45, 45, 45, 12, 45, 13, 45, 45, 45, 12, 46, 24, 28, 46, 23, 27, 46, 24, 28, 46, 13 },
	{ 11, 46, 16, 45, 21, 46, 16, 21, 46, 13, 45, 45, 45, 12, 45, 13, 45, 45, 45, 12, 46, 16, 21, 46, 46, 46, 46, 16, 21, 46, 13 },
	{ 11, 46, 16, 45, 21, 46, 16, 21, 46, 13, 45, 45, 45, 12, 45, 13, 45, 45, 45, 12, 46, 16, 23, 25, 25, 28, 46, 16, 21, 46, 13 },
	{ 11, 46, 39, 25, 27, 46, 23, 27, 46, 39,  4,  4,  4, 41, 45, 39,  4,  4,  4, 41, 46, 23, 26, 26, 26, 27, 46, 16, 21, 46, 13 },
	{ 11, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 16, 21, 46, 13 },
	{ 11, 46, 24, 26, 28, 46, 24, 26, 26, 26, 26, 26, 26, 28, 45, 24, 26, 26, 26, 28, 46, 24, 28, 46, 24, 26, 26, 41, 21, 46, 13 },
	{ 11, 46, 16, 45, 21, 46, 23, 25, 25, 37, 35, 25, 25, 27, 45, 23, 25, 25, 25, 27, 46, 16, 21, 46, 23, 26, 26, 42, 21, 46, 13 },
	{ 11, 46, 16, 45, 21, 46, 46, 46, 46, 16, 21, 45, 45, 45, 45, 45, 45, 45, 45, 45, 46, 16, 21, 46, 46, 46, 46, 16, 21, 46, 13 },
	{ 11, 46, 16, 45, 21, 46, 24, 28, 46, 16, 21, 45, 30,  3,  3,  3, 32, 45, 24, 28, 46, 16, 21, 46, 24, 28, 46, 16, 21, 46, 13 },
	{ 11, 46, 39, 25, 27, 46, 16, 21, 46, 23, 27, 45, 13, 45, 45, 45, 11, 45, 16, 21, 46, 23, 27, 46, 16, 21, 46, 23, 27, 46, 13 },
	{ 11, 46, 46, 46, 46, 46, 16, 21, 46, 45, 45, 45, 34, 45, 45, 45, 11, 45, 16, 21, 46, 46, 46, 46, 16, 21, 46, 46, 46, 46, 13 },
	{ 44, 26, 26, 26, 42, 46, 16, 36, 26, 26, 42, 45, 15, 45, 45, 45, 11, 45, 16, 36, 26, 26, 42, 45, 16, 36, 26, 26, 42, 46, 13 },
	{ 43, 25, 25, 25, 41, 46, 16, 35, 25, 25, 41, 45, 15, 45, 45, 45, 11, 45, 16, 35, 25, 25, 41, 45, 16, 35, 25, 25, 41, 46, 13 },
	{ 11, 46, 46, 46, 46, 46, 16, 21, 46, 45, 45, 45, 33, 45, 45, 45, 11, 45, 16, 21, 46, 46, 46, 46, 16, 21, 46, 46, 46, 46, 13 },
	{ 11, 46, 24, 26, 28, 46, 16, 21, 46, 24, 28, 45, 13, 45, 45, 45, 11, 45, 16, 21, 46, 24, 28, 46, 16, 21, 46, 24, 28, 46, 13 },
	{ 11, 46, 16, 45, 21, 46, 39, 27, 46, 16, 21, 45, 29,  4,  4,  4, 31, 45, 39, 27, 46, 16, 21, 46, 39, 27, 46, 16, 21, 46, 13 },
	{ 11, 46, 16, 45, 21, 46, 46, 46, 46, 16, 21, 45, 45, 45, 45, 45, 45, 45, 45, 45, 46, 16, 21, 46, 46, 46, 46, 16, 21, 46, 13 },
	{ 11, 46, 16, 45, 21, 46, 24, 26, 26, 38, 36, 26, 26, 28, 45, 24, 26, 26, 26, 28, 46, 16, 21, 46, 24, 25, 25, 41, 21, 46, 13 },
	{ 11, 46, 39, 25, 27, 46, 39, 25, 25, 25, 25, 25, 25, 27, 45, 39, 25, 25, 25, 27, 46, 39, 27, 46, 39, 25, 25, 42, 21, 46, 13 },
	{ 11, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 16, 21, 46, 13 },
	{ 11, 46, 24, 26, 28, 46, 24, 28, 46, 24,  3,  3,  3, 42, 45, 24,  3,  3,  3, 42, 46, 24, 25, 25, 25, 28, 46, 16, 21, 46, 13 },
	{ 11, 46, 16, 45, 21, 46, 16, 21, 46, 13, 45, 45, 45, 12, 45, 13, 45, 45, 45, 12, 46, 16, 24, 26, 26, 27, 46, 16, 21, 46, 13 },
	{ 11, 46, 16, 45, 21, 46, 16, 21, 46, 13, 45, 45, 45, 12, 45, 13, 45, 45, 45, 12, 46, 16, 21, 46, 46, 46, 46, 16, 21, 46, 13 },
	{ 11, 46, 39, 25, 27, 46, 39, 27, 46, 13, 45, 45, 45, 12, 45, 13, 45, 45, 45, 12, 46, 39, 27, 46, 24, 28, 46, 39, 27, 46, 13 },
	{ 11, 46, 46, 48, 46, 46, 46, 46, 46, 13, 45, 45, 45, 12, 45, 13, 45, 45, 45, 12, 46, 46, 46, 48, 16, 21, 46, 46, 46, 46, 13 },
	{  1,  3,  3,  3,  3,  3,  3,  3,  3,  5, 45, 45, 45, 12, 45, 13, 45, 45, 45,  1,  3,  3,  3,  3,  7,  9,  3,  3,  3,  3,  5 }
	};

	//Open Binary Writer
	BinaryWriter bw;
	bw.Open("PacmanLevel01.bin");
	//Write vec
	bw.Write(tiles);
	//Close Binary Writer
	bw.Close();
}

void LoadPacman(std::shared_ptr<GameMode> pGameMode)
{
	auto& scene = SceneManager::GetInstance().CreateScene("PacmanGame");

	auto font = ResourceManager::GetInstance().LoadFont("emulogic.ttf", 36);

	////Create FPS
	//GameObject* fpsObject = new GameObject();

	//auto pTransformComponentFPS = new TransformComponent();
	//pTransformComponentFPS->SetPosition(0.0f, 648.0f - 50.0f, 0.0f);
	//fpsObject->AddComponent(pTransformComponentFPS);

	//auto pTextComponentFPS = new TextComponent();
	//pTextComponentFPS->SetFont(font);
	//fpsObject->AddComponent(pTextComponentFPS);

	//auto pDrawComponentFPS = new DrawComponent();
	//fpsObject->AddComponent(pDrawComponentFPS);

	//auto pFPSComponent = new FPSComponent();
	//fpsObject->AddComponent(pFPSComponent);

	//scene.Add(fpsObject);

	//Create the grid
	int horOffset = 100;
	int verOffset = 100;
	int collumnSize = 31;
	int rowSize = 31;
	int tileSize = 16;

	//Read pacman level from file
	BinaryReader br;
	br.Open("PacmanLevel01.bin");
	std::vector<std::vector<int>> tiles;
	br.Read(tiles);
	br.Close();
	
	auto pGrid = std::make_shared<std::vector<std::vector<GameObject*>>>();

	(*pGrid).resize(tiles.size());
	for (size_t i = 0; i < (*pGrid).size(); i++)
	{
		(*pGrid)[i].resize(tiles[i].size());
	}

	for (int i = 0; i < collumnSize; i++)
	{
		for (int j = 0; j < rowSize; j++)
		{
			int locInVector = i * collumnSize + j;
			if (!(locInVector >= int(tiles.size() * tiles[0].size())))
			{
				GameObject* tileObject = new GameObject();

				auto pTransformComponent = new TransformComponent();
				pTransformComponent->SetPosition(float(horOffset + i * tileSize), float(verOffset + j * tileSize), 0.0f);
				tileObject->AddComponent(pTransformComponent);

				auto pDrawComponent = new DrawComponent();
				tileObject->AddComponent(pDrawComponent);

				auto pGridComponent = new GridComponent();
				int type = tiles[i][j];
				pGridComponent->SetTileType(TileType(type));

				tileObject->AddComponent(pGridComponent);

				(*pGrid)[i][j] = tileObject;
				//std::cout << i << ", " << j << "\n";
				scene.Add(tileObject);
			}
		}
	}



	//Next: Make Pacman & the Ghosts!
	GameObject* pacmanObject = new GameObject();

	auto pTransformComponent = new TransformComponent();
	pTransformComponent->SetPosition(float(horOffset) + 14 * tileSize - 8, float(verOffset) + 23 * tileSize - 8, 0.0f);
	pacmanObject->AddComponent(pTransformComponent);

	auto pDrawComponent = new DrawComponent();
	pacmanObject->AddComponent(pDrawComponent);

	auto pPacmanComponent = new PacmanComponent();
	pPacmanComponent->SetTextures();
	pPacmanComponent->SetGridReference(pGrid);
	pPacmanComponent->SetRespawnLoc(14, 23);
	pPacmanComponent->AddTeleportLocation(0, 14, 27, 14);
	pPacmanComponent->AddTeleportLocation(28, 14, 1, 14);
	pPacmanComponent->SetPlayerId(0);
	pacmanObject->AddComponent(pPacmanComponent);

	scene.Add(pacmanObject);

	std::shared_ptr<GameObject*> pacmanRef =  std::make_shared<GameObject*>(pacmanObject);

	GameObject* msPacmanObject = new GameObject();

	//MsPacman

	auto pTransformComponentP2 = new TransformComponent();
	pTransformComponentP2->SetPosition(float(horOffset) + 14 * tileSize - 8, float(verOffset) + 5 * tileSize - 8, 0.0f);
	msPacmanObject->AddComponent(pTransformComponentP2);

	auto pDrawComponentP2 = new DrawComponent();
	msPacmanObject->AddComponent(pDrawComponentP2);

	auto pPacmanComponentP2 = new PacmanComponent();
	pPacmanComponentP2->SetIsMsPacman();
	pPacmanComponentP2->SetTextures();
	pPacmanComponentP2->SetGridReference(pGrid);
	pPacmanComponentP2->SetRespawnLoc(14, 5);
	pPacmanComponentP2->AddTeleportLocation(0, 14, 27, 14);
	pPacmanComponentP2->AddTeleportLocation(28, 14, 1, 14);
	pPacmanComponentP2->SetPlayerId(1);
	msPacmanObject->AddComponent(pPacmanComponentP2);

	if (*pGameMode == GameMode::MsPacman)
	{
		scene.Add(msPacmanObject);
	}

	std::shared_ptr<GameObject*> msPacmanRef = std::make_shared<GameObject*>(msPacmanObject);

	//Ghosts

	//Ghost 0
	GameObject* ghost0Object = new GameObject();

	auto pTransformComponentGhost0 = new TransformComponent();
	pTransformComponentGhost0->SetPosition(float(horOffset) + 9 * tileSize - 8, float(verOffset) + 11 * tileSize - 8, 0.0f);
	ghost0Object->AddComponent(pTransformComponentGhost0);

	auto pDrawComponentGhost0 = new DrawComponent();
	ghost0Object->AddComponent(pDrawComponentGhost0);

	auto pGhostComponent0 = new GhostComponent();
	pGhostComponent0->SetGridReference(pGrid);
	pGhostComponent0->SetRespawnLoc(9, 11);
	pGhostComponent0->SetGhostId(0);
	pGhostComponent0->AddTeleportLocation(0, 14, 27, 14);
	pGhostComponent0->AddTeleportLocation(28, 14, 1, 14);
	pGhostComponent0->SetTextures();
	if (*pGameMode == GameMode::Ghost)
	{
		pGhostComponent0->SetPlayerId(1);
	}
	ghost0Object->AddComponent(pGhostComponent0);

	scene.Add(ghost0Object);

	std::shared_ptr<GameObject*> ghost0Ref = std::make_shared<GameObject*>(ghost0Object);


	//Ghost 1
	GameObject* ghost1Object = new GameObject();

	auto pTransformComponentGhost1 = new TransformComponent();
	pTransformComponentGhost1->SetPosition(float(horOffset) + 18 * tileSize - 8, float(verOffset) + 11 * tileSize - 8, 0.0f);
	ghost1Object->AddComponent(pTransformComponentGhost1);

	auto pDrawComponentGhost1 = new DrawComponent();
	ghost1Object->AddComponent(pDrawComponentGhost1);

	auto pGhostComponent1 = new GhostComponent();
	pGhostComponent1->SetGridReference(pGrid);
	pGhostComponent1->SetRespawnLoc(18, 11);
	pGhostComponent1->SetGhostId(1);
	pGhostComponent1->AddTeleportLocation(0, 14, 27, 14);
	pGhostComponent1->AddTeleportLocation(28, 14, 1, 14);
	pGhostComponent1->SetTextures();
	ghost1Object->AddComponent(pGhostComponent1);

	scene.Add(ghost1Object);

	std::shared_ptr<GameObject*> ghost1Ref = std::make_shared<GameObject*>(ghost1Object);


	//Ghost 2
	GameObject* ghost2Object = new GameObject();

	auto pTransformComponentGhost2 = new TransformComponent();
	pTransformComponentGhost2->SetPosition(float(horOffset) + 9 * tileSize - 8, float(verOffset) + 17 * tileSize - 8, 0.0f);
	ghost2Object->AddComponent(pTransformComponentGhost2);

	auto pDrawComponentGhost2 = new DrawComponent();
	ghost2Object->AddComponent(pDrawComponentGhost2);

	auto pGhostComponent2 = new GhostComponent();
	pGhostComponent2->SetGridReference(pGrid);
	pGhostComponent2->SetRespawnLoc(9, 17);
	pGhostComponent2->SetGhostId(2);
	pGhostComponent2->AddTeleportLocation(0, 14, 27, 14);
	pGhostComponent2->AddTeleportLocation(28, 14, 1, 14);
	pGhostComponent2->SetTextures();
	ghost2Object->AddComponent(pGhostComponent2);

	scene.Add(ghost2Object);

	std::shared_ptr<GameObject*> ghost2Ref = std::make_shared<GameObject*>(ghost2Object);


	//Ghost 3
	GameObject* ghost3Object = new GameObject();

	auto pTransformComponentGhost3 = new TransformComponent();
	pTransformComponentGhost3->SetPosition(float(horOffset) + 18 * tileSize - 8, float(verOffset) + 17 * tileSize - 8, 0.0f);
	ghost3Object->AddComponent(pTransformComponentGhost3);

	auto pDrawComponentGhost3 = new DrawComponent();
	ghost3Object->AddComponent(pDrawComponentGhost3);

	auto pGhostComponent3 = new GhostComponent();
	pGhostComponent3->SetGridReference(pGrid);
	pGhostComponent3->SetRespawnLoc(18, 17);
	pGhostComponent3->SetGhostId(3);
	pGhostComponent3->AddTeleportLocation(0, 14, 27, 14);
	pGhostComponent3->AddTeleportLocation(28, 14, 1, 14);
	pGhostComponent3->SetTextures();
	ghost3Object->AddComponent(pGhostComponent3);

	scene.Add(ghost3Object);

	std::shared_ptr<GameObject*> ghost3Ref = std::make_shared<GameObject*>(ghost3Object);


	//Add ghost refs to pacmans
	pPacmanComponent->AddGhostRef(ghost0Ref);
	pPacmanComponent->AddGhostRef(ghost1Ref);
	pPacmanComponent->AddGhostRef(ghost2Ref);
	pPacmanComponent->AddGhostRef(ghost3Ref);

	//Add pacman refs to ghosts
	pGhostComponent0->AddPacmanRef(pacmanRef);
	pGhostComponent1->AddPacmanRef(pacmanRef);
	pGhostComponent2->AddPacmanRef(pacmanRef);
	pGhostComponent3->AddPacmanRef(pacmanRef);

	if (*pGameMode == GameMode::MsPacman)
	{
		//Add ghost refs to pacmans
		pPacmanComponentP2->AddGhostRef(ghost0Ref);
		pPacmanComponentP2->AddGhostRef(ghost1Ref);
		pPacmanComponentP2->AddGhostRef(ghost2Ref);
		pPacmanComponentP2->AddGhostRef(ghost3Ref);

		//Add pacman refs to ghosts
		pGhostComponent0->AddPacmanRef(msPacmanRef);
		pGhostComponent1->AddPacmanRef(msPacmanRef);
		pGhostComponent2->AddPacmanRef(msPacmanRef);
		pGhostComponent3->AddPacmanRef(msPacmanRef);
	}

	//Gamemode manager -> ends gamemode when all pacmans are dead
	GameObject* gamemodeObject = new GameObject();

	auto pGameModeComponent = new GameModeComponent();
	pGameModeComponent->SetGameModeReference(pGameMode);
	pGameModeComponent->SetPacManReference(pacmanRef);
	pGameModeComponent->SetMsPacManReference(msPacmanRef);
	gamemodeObject->AddComponent(pGameModeComponent);

	scene.Add(gamemodeObject);


	GameObject* scoreObject0 = new GameObject();

	auto pTransformComponentScore0 = new TransformComponent();
	pTransformComponentScore0->SetPosition(0.0f, 0.0f, 0.0f);
	scoreObject0->AddComponent(pTransformComponentScore0);

	auto pTextComponentScore0 = new TextComponent();
	pTextComponentScore0->SetFont(font);
	scoreObject0->AddComponent(pTextComponentScore0);

	auto pDrawComponentScore0 = new DrawComponent();
	scoreObject0->AddComponent(pDrawComponentScore0);

	auto pScoreComponent0 = new ScoreComponent();
	pScoreComponent0->SetPacManReference(pacmanRef);
	pScoreComponent0->SetPlayerId(0);
	scoreObject0->AddComponent(pScoreComponent0);

	scene.Add(scoreObject0);


	GameObject* livesObject0 = new GameObject();

	auto pTransformComponentLives0 = new TransformComponent();
	pTransformComponentLives0->SetPosition(648.0f - 115.0f, 10.0f, 0.0f);
	livesObject0->AddComponent(pTransformComponentLives0);

	auto pDrawComponentLives0 = new DrawComponent();
	livesObject0->AddComponent(pDrawComponentLives0);

	auto pLivesComponent0 = new LivesComponent();
	pLivesComponent0->SetPacManReference(pacmanRef);
	pLivesComponent0->SetTextures();
	livesObject0->AddComponent(pLivesComponent0);

	scene.Add(livesObject0);

	if (*pGameMode == GameMode::MsPacman)
	{
		GameObject* scoreObject1 = new GameObject();

		auto pTransformComponentScore1 = new TransformComponent();
		pTransformComponentScore1->SetPosition(0.0f, 50.0f, 0.0f);
		scoreObject1->AddComponent(pTransformComponentScore1);

		auto pTextComponentScore1 = new TextComponent();
		pTextComponentScore1->SetFont(font);
		scoreObject1->AddComponent(pTextComponentScore1);

		auto pDrawComponentScore1 = new DrawComponent();
		scoreObject1->AddComponent(pDrawComponentScore1);

		auto pScoreComponent1 = new ScoreComponent();
		pScoreComponent1->SetPacManReference(msPacmanRef);
		pScoreComponent1->SetPlayerId(1);
		scoreObject1->AddComponent(pScoreComponent1);

		scene.Add(scoreObject1);


		GameObject* livesObject1 = new GameObject();

		auto pTransformComponentLives1 = new TransformComponent();
		pTransformComponentLives1->SetPosition(648.0f - 115.0f, 60.0f, 0.0f);
		livesObject1->AddComponent(pTransformComponentLives1);

		auto pDrawComponentLives1 = new DrawComponent();
		livesObject1->AddComponent(pDrawComponentLives1);

		auto pLivesComponent1 = new LivesComponent();
		pLivesComponent1->SetIsMsPacman();
		pLivesComponent1->SetPacManReference(msPacmanRef);
		pLivesComponent1->SetTextures();
		livesObject1->AddComponent(pLivesComponent1);

		scene.Add(livesObject1);

	}




}

std::shared_ptr<GameMode> LoadGame()
{
	std::shared_ptr<GameMode> pGameMode = std::make_shared<GameMode>(GameMode::Menu);

	auto& scene = SceneManager::GetInstance().CreateScene("ModeSelect");

	//Create Menu Text:
	GameObject* menuObject = new GameObject();

	auto pTransformComponentMenu = new TransformComponent();
	pTransformComponentMenu->SetPosition(0.0f, 0.0f, 0.0f);
	menuObject->AddComponent(pTransformComponentMenu);

	auto pTextComponentMenu = new TextComponent();
	auto font = ResourceManager::GetInstance().LoadFont("emulogic.ttf", 30);
	pTextComponentMenu->SetFont(font);
	pTextComponentMenu->SetText("Select a mode:");
	menuObject->AddComponent(pTextComponentMenu);

	auto pDrawComponentMenu = new DrawComponent();
	menuObject->AddComponent(pDrawComponentMenu);

	auto pMenuComponent = new MenuComponent();
	pMenuComponent->SetGameModeReference(pGameMode);
	menuObject->AddComponent(pMenuComponent);

	scene.Add(menuObject);


	//Create Option 1
	GameObject* option1Object = new GameObject();

	auto pTransformComponentOption1 = new TransformComponent();
	pTransformComponentOption1->SetPosition(0.0f, 50.0f, 0.0f);
	option1Object->AddComponent(pTransformComponentOption1);

	auto pTextComponentOption1 = new TextComponent();
	pTextComponentOption1->SetFont(font);
	pTextComponentOption1->SetText("1. SinglePlayer");
	option1Object->AddComponent(pTextComponentOption1);

	auto pDrawComponentOption1 = new DrawComponent();
	option1Object->AddComponent(pDrawComponentOption1);

	scene.Add(option1Object);

	//Create Option 2
	GameObject* option2Object = new GameObject();

	auto pTransformComponentOption2 = new TransformComponent();
	pTransformComponentOption2->SetPosition(0.0f, 100.0f, 0.0f);
	option2Object->AddComponent(pTransformComponentOption2);

	auto pTextComponentOption2 = new TextComponent();
	pTextComponentOption2->SetFont(font);
	pTextComponentOption2->SetText("2. Pacman & MsPacman");
	option2Object->AddComponent(pTextComponentOption2);

	auto pDrawComponentOption2 = new DrawComponent();
	option2Object->AddComponent(pDrawComponentOption2);

	scene.Add(option2Object);

	//Create Option 3
	GameObject* option3Object = new GameObject();

	auto pTransformComponentOption3 = new TransformComponent();
	pTransformComponentOption3->SetPosition(0.0f, 150.0f, 0.0f);
	option3Object->AddComponent(pTransformComponentOption3);

	auto pTextComponentOption3 = new TextComponent();
	pTextComponentOption3->SetFont(font);
	pTextComponentOption3->SetText("3. Pacman & Ghost");
	option3Object->AddComponent(pTextComponentOption3);

	auto pDrawComponentOption3 = new DrawComponent();
	option3Object->AddComponent(pDrawComponentOption3);

	scene.Add(option3Object);





	//LoadPacman();

	return pGameMode;
}

void Cleanup()
{
	Drawer::GetInstance().Destroy();
	SDL_DestroyWindow(window);
	SDL_Quit();
}

#pragma warning( push )  
#pragma warning( disable : 4100 )  
int main(int argc, char* argv[]) {
#pragma warning( pop )

	Initialize();

	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Init("../Data/");

	std::shared_ptr<GameMode> pGameMode = LoadGame();

	{
		auto t = std::chrono::high_resolution_clock::now();
		auto& renderer = Drawer::GetInstance();
		auto& sceneManager = SceneManager::GetInstance();
		auto& input = InputManager::GetInstance();
		bool doContinue = true;
		auto lastTime = std::chrono::high_resolution_clock::now();

		bool pacmanStarted = false;
		//bool pacmanIsDead = false;
		//bool msPacmanIsDead = false;

		while (doContinue)
		{
			//Game Mode was selected, start pacman
			if (*pGameMode != GameMode::Menu && !pacmanStarted)
			{
				sceneManager.Clear();
				LoadPacman(pGameMode);
				pacmanStarted = true;
			}

			if (pacmanStarted && *pGameMode == GameMode::Menu)
			{
				sceneManager.Clear();
				pGameMode = LoadGame();
				pacmanStarted = false;
			}

			auto currentTime = std::chrono::high_resolution_clock::now();
			float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
			doContinue = input.ProcessInput();

			sceneManager.Update(deltaTime);

			renderer.Draw();

			lastTime = currentTime;
		}
	}

	Cleanup();
    return 0;
}