#ifndef ASSETS_H
#define ASSETS_H

#include <ft2build.h>
#include FT_FREETYPE_H  

namespace Ember {
	bool InitializeImageLoader();
	bool InitializeSoundLoader();
	bool InitializeFontLoader();
	FT_Library* GetFreeType();

	bool InitializeAssets();

	void AssetCleanUp();
}

#endif // !ASSETS_H
