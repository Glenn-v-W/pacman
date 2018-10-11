#include "MiniginPCH.h"
#include "FPSComponent.h"
#include "TextComponent.h"
#include "..\Scenegraph\GameObject.h"

FPSComponent::FPSComponent()
{}

FPSComponent::~FPSComponent()
{}

void FPSComponent::Update(float deltaTime)
{
	auto gameObject = GetGameObject();
	gameObject->GetComponent<TextComponent>()->SetText(std::to_string(1.0f / deltaTime));
}