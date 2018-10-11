#include "MiniginPCH.h"
#include "ResourceManager.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Drawer.h"
#include "Texture2D.h"
#include "Font.h"

void ResourceManager::Init(std::string&& dataPath)
{
	mDataPath = std::move(dataPath);

	// load support for png and jpg, this takes a while!

	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
		std::stringstream ss; ss << "Failed to load support for png: " << SDL_GetError();
		throw std::runtime_error(ss.str().c_str());
	}

	if ((IMG_Init(IMG_INIT_JPG) & IMG_INIT_JPG) != IMG_INIT_JPG) {
		std::stringstream ss; ss << "Failed to load support for png: " << SDL_GetError();
		throw std::runtime_error(ss.str().c_str());
	}

	if (TTF_Init() != 0) {
		std::stringstream ss; ss << "Failed to load support for fonts: " << SDL_GetError();
		throw std::runtime_error(ss.str().c_str());
	}
}

std::shared_ptr<Texture2D> ResourceManager::LoadTexture(const std::string& file)
{
	std::string fullPath = (mDataPath + file).c_str();
	SDL_Texture *texture = IMG_LoadTexture(Drawer::GetInstance().GetSDLRenderer(), fullPath.c_str());
	if (texture == nullptr) {
		std::stringstream ss; ss << "Failed to load texture: " << SDL_GetError();
		throw std::runtime_error(ss.str().c_str());
	}
	return std::make_shared<Texture2D>(texture);
}

std::shared_ptr<Font> ResourceManager::LoadFont(const std::string& file, unsigned int size)
{
	std::string fullPath = (mDataPath + file).c_str();
	return std::make_shared<Font>(fullPath, size);
}
