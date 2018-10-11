#pragma once
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec3.hpp>
#pragma warning(pop)
#include "BaseComponent.h"
#include "..\Texture2D.h"

class DrawComponent : public BaseComponent
{
public:
	DrawComponent();
	virtual ~DrawComponent();
	void Initialize() {};
	void Update(float deltaTime) { UNREFERENCED_PARAMETER(deltaTime); };
	void Draw();

	void SetTexture(const std::string& filename);
	void SetTexture(const std::shared_ptr<Texture2D>& texture);
private:
	std::shared_ptr<Texture2D> m_Texture;
};
