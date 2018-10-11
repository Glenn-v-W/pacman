#pragma once
#include "BaseComponent.h"
#include <chrono>

class FPSComponent : public BaseComponent
{
public:
	FPSComponent();
	virtual ~FPSComponent();
	void Initialize() {};
	void Update(float deltaTime);
	void Draw() {};
};