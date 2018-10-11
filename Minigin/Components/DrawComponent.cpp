#include "MiniginPCH.h"
#include "..\Scenegraph\GameObject.h"
#include "DrawComponent.h"
#include "TransformComponent.h"
#include "..\ResourceManager.h"
#include "..\Drawer.h"

DrawComponent::DrawComponent():
	m_Texture(std::shared_ptr<Texture2D>())
{
}

DrawComponent::~DrawComponent()
{
}

void DrawComponent::Draw()
{
	if (m_Texture != nullptr)
	{
		const auto pos = m_pGameObject->GetComponent<TransformComponent>()->GetPosition();
		Drawer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
	}
}

void DrawComponent::SetTexture(const std::string & filename)
{
	m_Texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void DrawComponent::SetTexture(const std::shared_ptr<Texture2D> & texture)
{
	m_Texture = texture;
}