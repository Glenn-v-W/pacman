#pragma once
#include "Singleton.h"

struct SDL_Window;
struct SDL_Renderer;

class Texture2D;
class Drawer final : public Singleton<Drawer>
{
	SDL_Renderer* mRenderer = nullptr;

public:
	void Init(SDL_Window* window);
	void Draw();
	void Destroy();

	void RenderTexture(const Texture2D& texture, float x, float y) const;
	void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;

	SDL_Renderer* GetSDLRenderer() const { return mRenderer; }
};

