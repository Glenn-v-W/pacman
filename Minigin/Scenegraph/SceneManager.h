#pragma once
#include "..\Singleton.h"

class Scene;
class SceneManager final : public Singleton<SceneManager>
{
public:
	Scene & CreateScene(const std::string& name);

	void Update(float deltaTime);
	void Draw();
	void Clear();

private:
	std::vector<std::shared_ptr<Scene>> mScenes;
};


