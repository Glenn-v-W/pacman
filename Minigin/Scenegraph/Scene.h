#pragma once
#include "SceneManager.h"

//class SceneObject;
class GameObject;
class Scene
{
public:
	Scene(const std::string& name);
	virtual ~Scene();

	void Add(GameObject* object);

	void Update(float deltaTime);
	void Draw() const;

	Scene(const Scene& other) = delete;
	Scene(Scene&& other) = delete;
	Scene& operator=(const Scene& other) = delete;
	Scene& operator=(Scene&& other) = delete;

private: 

	std::string m_Name{};
	std::vector <GameObject*> m_Objects{};

	static unsigned int idCounter; 
};

