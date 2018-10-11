#include "MiniginPCH.h"
#include "GameObject.h"
#include "..\ResourceManager.h"
#include "..\Drawer.h"
#include "..\Components\BaseComponent.h"
#include "..\Components\TransformComponent.h"

GameObject::GameObject():
	m_pComponents(std::vector<BaseComponent*>()),
	m_IsInitialized(false),
	m_pTransform(nullptr)
{
}

GameObject::~GameObject(void)
{
	for (size_t i = 0; i < m_pComponents.size(); i++)
	{
		if (m_pComponents[i] != 0)
		{
			delete(m_pComponents[i]);
			m_pComponents[i] = 0;
		}
	}
}

void GameObject::AddComponent(BaseComponent * pComp)
{
#if _DEBUG
	if (typeid(*pComp) == typeid(TransformComponent) && HasComponent<TransformComponent>())
	{
		std::cout << "AddComponent -> can't add a TransformComponent (there can only be one)";
		return;
	}

	for (size_t i = 0; i < m_pComponents.size(); i++)
	{
		if (m_pComponents[i] == pComp)
		{
			std::cout << "AddComponent -> this Component is already in the GameObject";
			return;
		}
	}
#endif

	m_pComponents.push_back(pComp);
	pComp->m_pGameObject = this;
}

void GameObject::RemoveComponent(BaseComponent * pComp)
{
	auto it = find(m_pComponents.begin(), m_pComponents.end(), pComp);

#if _DEBUG
	if (it == m_pComponents.end())
	{
		std::cout << "RemoveComponent -> cannot find component to remove";
		return;
	}

	if (typeid(*pComp) == typeid(TransformComponent))
	{
		std::cout << "RemoveComponent -> cannot remove TransformComponent";
		return;
	}
#endif

	m_pComponents.erase(it);
	pComp->m_pGameObject = nullptr;
}

void GameObject::Initialize()
{
	if (m_IsInitialized)
	{
		return;
	}

	for (size_t i = 0; i < m_pComponents.size(); i++)
	{
		m_pComponents[i]->Initialize();
	}

	m_IsInitialized = true;
}

void GameObject::Update(float deltaTime)
{
	for (size_t i = 0; i < m_pComponents.size(); i++)
	{
		m_pComponents[i]->Update(deltaTime);
	}
}

void GameObject::Draw() const
{
	//auto pos = mTransform.GetPosition();
	//Drawer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
	for (size_t i = 0; i < m_pComponents.size(); i++)
	{
		m_pComponents[i]->Draw();
	}
}

void GameObject::SetPosition(float x, float y)
{
	m_pTransform->SetPosition(x, y, 0.0f);
}

//void GameObject::SetTexture(const std::string& filename)
//{
//	m_Texture = ResourceManager::GetInstance().LoadTexture(filename);
//}
//
//void GameObject::SetPosition(float x, float y)
//{
//	mTransform.SetPosition(x, y, 0.0f);
//}
