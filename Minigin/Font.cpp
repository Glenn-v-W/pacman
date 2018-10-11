#include "MiniginPCH.h"
#include "Font.h"

TTF_Font* Font::GetFont() const {
	return mFont;
}

Font::Font(const std::string& fullPath, unsigned size) : mFont(nullptr), mSize(size)
{
	mFont = TTF_OpenFont(fullPath.c_str(), size);
	if (mFont == nullptr) {
		std::stringstream ss; ss << "Failed to load font: " << SDL_GetError();
		throw std::runtime_error(ss.str().c_str());
	}
}

Font::~Font()
{
	TTF_CloseFont(mFont);
}
