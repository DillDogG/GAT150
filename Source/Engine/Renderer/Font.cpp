#include "Font.h"
#include <SDL2-2.28.0/include/SDL_ttf.h>

namespace kiko {
	Font::Font(const std::string& filename, int fontSize) {
		Load(filename, fontSize);
	}

	Font::~Font() {
		if (m_ttfFont) Font(TTF_CloseFont);
	}

	void Font::Load(const std::string& filename, int fontSize) {
		m_ttfFont = TTF_OpenFont(filename.c_str(), fontSize);
	}
}