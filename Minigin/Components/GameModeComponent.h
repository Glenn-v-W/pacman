#pragma once
#include "BaseComponent.h"

class GameModeComponent : public BaseComponent
{
public:
	GameModeComponent();
	virtual ~GameModeComponent();
	void Initialize() {};
	void Update(float deltaTime);
	void Draw() {};

	void SetGameModeReference(std::shared_ptr<GameMode> pGameMode);
	void SetPacManReference(std::shared_ptr<GameObject*> pPacman);
	void SetMsPacManReference(std::shared_ptr<GameObject*> pMsPacman);

private:
	std::shared_ptr<GameMode> m_pGameMode;
	std::shared_ptr<GameObject*> m_pPacman;
	std::shared_ptr<GameObject*> m_pMsPacman;

};
