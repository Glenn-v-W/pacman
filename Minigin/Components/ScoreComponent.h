#pragma once
#include "BaseComponent.h"

class ScoreComponent : public BaseComponent
{
public:
	ScoreComponent();
	virtual ~ScoreComponent();
	void Initialize() {};
	void Update(float deltaTime);
	void Draw() {};

	void SetPlayerId(int playerId);
	void SetPacManReference(std::shared_ptr<GameObject*> pPacman);
private:
	std::shared_ptr<GameObject*> m_pPacman;
	int m_Score = 0;
	int m_PlayerId = 0;
};