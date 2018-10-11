#include "MiniginPCH.h"
#include "Scene.h"
#include "GameObject.h"

unsigned int Scene::idCounter = 0;

Scene::Scene(const std::string& name) : 
	m_Name(name),
	m_Objects(std::vector<GameObject*>())
{
}

Scene::~Scene()
{
	for (size_t i = 0; i < m_Objects.size(); i++)
	{
		if (m_Objects[i] != 0)
		{
			delete(m_Objects[i]);
			m_Objects[i] = 0;
		}
	}
};

void Scene::Add(GameObject* object)
{
	m_Objects.push_back(object);
}

void Scene::Update(float deltaTime)
{
	for (size_t i = 0; i < m_Objects.size(); i++)
	{
		m_Objects[i]->Update(deltaTime);
	}
}

void Scene::Draw() const
{
	for (size_t i = 0; i < m_Objects.size(); i++)
	{
		m_Objects[i]->Draw();
	}
}

