#pragma once
#include "BaseComponent.h"
#include "..\Texture2D.h"

class LivesComponent : public BaseComponent
{
public:
	LivesComponent();
	virtual ~LivesComponent();
	void Initialize() {};
	void Update(float deltaTime);
	void Draw() {};

	void SetIsMsPacman();
	void SetPacManReference(std::shared_ptr<GameObject*> pPacman);
	void SetTextures();
private:
	std::shared_ptr<GameObject*> m_pPacman;
	int m_Lives = 0;
	std::shared_ptr<Texture2D> m_Texture_3;
	std::shared_ptr<Texture2D> m_Texture_2;
	std::shared_ptr<Texture2D> m_Texture_1;
	std::shared_ptr<Texture2D> m_Texture_0;
	bool m_IsMsPacman = false;
};