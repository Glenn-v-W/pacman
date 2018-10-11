#pragma once
#include "BaseComponent.h"

class MenuComponent : public BaseComponent
{
public:
	MenuComponent();
	virtual ~MenuComponent();
	void Initialize() {};
	void Update(float deltaTime);
	void Draw() {};

	void SetGameModeReference(std::shared_ptr<GameMode> pGameMode);

private:
	std::shared_ptr<GameMode> m_pGameMode;
};
