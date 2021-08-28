#ifndef ASSETS_H
#define ASSETS_H

namespace Ember {
	bool InitializeImageLoader();
	bool InitializeSoundLoader();
	bool InitializeAssets();

	void AssetCleanUp();
}

#endif // !ASSETS_H
