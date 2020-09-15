#ifndef ASSET_INITIALIZERS_H
#define ASSET_INITIALIZERS_H

#include <SDL.h>

namespace ember {
	bool InitializeImageLoader();
	bool InitializeFontLoader();
	bool InitializeSoundLoader();

	bool InitializeAssets();

	void AssetCleanUp();

	SDL_DisplayMode GetDisplay();
	void CheckVersion();
}

#endif // !ASSET_INITIALIZERS_H
