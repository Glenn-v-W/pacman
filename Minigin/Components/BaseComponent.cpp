#include "MiniginPCH.h"
#include "BaseComponent.h"
#include "..\Scenegraph\GameObject.h"

BaseComponent::BaseComponent():
	m_pGameObject(nullptr)
{
}


BaseComponent::~BaseComponent()
{
}

TransformComponent * BaseComponent::GetTransform() const
{
#if _DEBUG
	if (!m_pGameObject)
	{
		std::cout << "GetTransform() -> could not find the TransformComponent";
		return nullptr;
	}
#endif

	return m_pGameObject->GetTransform();
}
