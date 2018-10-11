#pragma once
class GameObject;
class TransformComponent;

class BaseComponent
{
public:
	BaseComponent(void);
	virtual ~BaseComponent(void);

	GameObject* GetGameObject() const { return m_pGameObject; }
	TransformComponent* GetTransform() const;

protected:

	virtual void Initialize() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Draw() = 0;

	GameObject* m_pGameObject;

private:

	friend class GameObject;

private:
	BaseComponent(const BaseComponent &obj);
	BaseComponent& operator=(const BaseComponent& obj);
};
