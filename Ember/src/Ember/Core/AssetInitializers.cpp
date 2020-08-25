#include "EmberPch.h"
#include "AssetInitializers.h"

namespace ember {
	bool InitializeImageLoader() {
		int imgFlags = IMG_INIT_PNG;

		if (!(IMG_Init(imgFlags) & imgFlags))
			return false;

		return true;
	}

	bool InitializeFontLoader() {
		if (TTF_Init() == -1) 
			return false;
		
		return true;
	}

	bool InitializeSoundLoader() {
		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

		return true;
	}

	bool InitializeAssets() {
		return (InitializeImageLoader() && InitializeFontLoader() && InitializeSoundLoader());
	}
}