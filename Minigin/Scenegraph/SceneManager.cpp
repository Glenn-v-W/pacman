#include "MiniginPCH.h"
#include "SceneManager.h"
#include "Scene.h"


void SceneManager::Update(float deltaTime)
{
	for(auto scene : mScenes)
	{
		scene->Update(deltaTime);
	}
}

void SceneManager::Draw()
{
	for (const auto scene : mScenes)
	{
		scene->Draw();
	}
}

void SceneManager::Clear()
{
	mScenes.clear();
}

Scene& SceneManager::CreateScene(const std::string& name)
{
	const auto scene = std::shared_ptr<Scene>(new Scene(name));
	mScenes.push_back(scene);
	return *scene;
}
