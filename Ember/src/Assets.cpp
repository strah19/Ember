#include "Assets.h"
#include "Ember.h"
#include "Logger.h"

namespace Ember {
	FT_Library ft;

	bool InitializeImageLoader() {
		int imgFlags = IMG_INIT_PNG;

		if (!(IMG_Init(imgFlags) & imgFlags))
			return false;

		return true;
	}

	bool InitializeSoundLoader() {
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
			EMBER_LOG_ERROR("Failed to init SDL_Mixer with error code '%s'.", Mix_GetError());
			return false;
		}
		return true;
	}

	bool InitializeFontLoader() {
		if (FT_Init_FreeType(&ft)) {
			EMBER_LOG_ERROR("Failed to init FreeType Library.");
			return false;
		}
		return true;
	}

	FT_Library* GetFreeType() {
		return &ft;
	}

	bool InitializeAssets() {
		EMBER_LOG_GOOD("Asset loaders initialized.");
		return (InitializeImageLoader() && InitializeSoundLoader() && InitializeFontLoader());
	}

	void AssetCleanUp() {
		IMG_Quit();
		Mix_Quit();
		FT_Done_FreeType(ft);
	}
}