#ifndef VULKAN_WINDOW_H
#define VULKAN_WINDOW_H

#include "SDLWindow.h"

namespace Ember {
	class VulkanWindow : public SDLWindow {
	public:
		VulkanWindow(WindowProperties* properties);

	private:
	};
}

#endif // !VULKAN_WINDOW_H
