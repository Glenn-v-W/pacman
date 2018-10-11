#pragma once
#include <memory>

//#include "..\Transform.h"
#include "..\Texture2D.h"
//#include "SceneObject.h"
#include "..\Components\BaseComponent.h"
#include "..\Components\TransformComponent.h"

class GameObject
{
public:
	
	GameObject();
	virtual ~GameObject();

	void AddComponent(BaseComponent* pComp);
	void RemoveComponent(BaseComponent* pComp);

	TransformComponent* GetTransform() const { return m_pTransform; }

	template<class T> bool HasComponent()
		{
			return GetComponent<T>() != nullptr;
		}

	template<class T> T* GetComponent()
		{
			const type_info& ti = typeid(T);
			for (auto *component : m_pComponents)
			{
				if (component && typeid(*component) == ti)
					return static_cast<T*>(component);
			}
			return nullptr;
		}

	template<class T> std::vector<T*> GetComponents()
		{
			const type_info& ti = typeid(T);
			vector<T*> components;

			for (auto *component : m_pComponents)
			{
				if (component && typeid(*component) == ti)
					components.push_back(static_cast<T*>(component));
			}
			return components;
		}

	void Initialize();
	void Update(float deltaTime);
	void Draw() const;
	void SetPosition(float x, float y);

private:
	bool m_IsInitialized;
	std::vector<BaseComponent*> m_pComponents;
	TransformComponent* m_pTransform;

	GameObject(const GameObject& other) = delete;
	GameObject& operator=(const GameObject& other) = delete;
};
