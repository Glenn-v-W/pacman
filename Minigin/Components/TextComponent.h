#pragma once
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec3.hpp>
#pragma warning(pop)
#include "BaseComponent.h"

class Font;
class Texture2D;

class TextComponent : public BaseComponent
{
public:
	TextComponent();
	virtual ~TextComponent();
	void Initialize() {};
	void Update(float deltaTime);
	void Draw() {};

	const std::string& GetText() const { return m_Text; }
	void SetText(const std::string & text);
	void SetFont(const std::shared_ptr<Font> & font);

private:
	bool m_NeedsUpdate;
	glm::vec3 m_Position{};
	std::string m_Text;
	std::shared_ptr<Font> m_Font;
	std::shared_ptr<Texture2D> m_Texture;
};
