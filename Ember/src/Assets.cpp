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
		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

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
		return (InitializeImageLoader() && InitializeSoundLoader() && InitializeFontLoader());
	}

	void AssetCleanUp() {
		IMG_Quit();
		Mix_Quit();
		FT_Done_FreeType(ft);
	}
}