#include "MiniginPCH.h"
#include "..\Scenegraph\GameObject.h"
#include "TextComponent.h"
#include "DrawComponent.h"
#include "TransformComponent.h"
#include "..\Font.h"
#include "..\Drawer.h"

TextComponent::TextComponent() :
	m_Text(std::string()),
	m_NeedsUpdate(true), 
	m_Font(nullptr),
	m_Texture(nullptr)
{
}

TextComponent::~TextComponent()
{
}

void TextComponent::Update(float deltaTime)
{
	UNREFERENCED_PARAMETER(deltaTime);
	if (m_NeedsUpdate && (m_Font != nullptr) && (m_Text != std::string()))
	{
		const SDL_Color color = { 255,255,255 }; // only white text is supported now
		const auto surf = TTF_RenderText_Blended(m_Font->GetFont(), m_Text.c_str(), color);
		if (surf == nullptr) {
			std::stringstream ss; ss << "Draw text failed: " << SDL_GetError();
			throw std::runtime_error(ss.str().c_str());
		}
		auto texture = SDL_CreateTextureFromSurface(Drawer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr) {
			std::stringstream ss; ss << "Create text texture from surface failed: " << SDL_GetError();
			throw std::runtime_error(ss.str().c_str());
		}
		SDL_FreeSurface(surf);
		m_Texture = std::make_shared<Texture2D>(texture);

		GetGameObject()->GetComponent<DrawComponent>()->SetTexture(m_Texture);
	}
}

void TextComponent::SetText(const std::string & text)
{
	m_Text = text;
}

void TextComponent::SetFont(const std::shared_ptr<Font>& font)
{
	m_Font = font;
}