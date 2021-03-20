#include "OSDepStructures.h"

#ifdef _WIN32   
namespace Ember {
	HWND GetNativeWindowHandler(Ember::Window* window) {
		SDL_SysWMinfo wmInfo;
		SDL_VERSION(&wmInfo.version);
		SDL_GetWindowWMInfo(static_cast<SDL_Window*>(window->GetNativeWindow()), &wmInfo);
		return wmInfo.info.win.window;
	}

	UINT GetNativeMsg(Ember::EventHandler* handler) {
		return handler->NativeEvent()->syswm.msg->msg.win.msg;
	}

	WPARAM GetWParam(Ember::EventHandler* handler) {
		return handler->NativeEvent()->syswm.msg->msg.win.wParam;
	}

	LPARAM GetLParam(Ember::EventHandler* handler) {
		return handler->NativeEvent()->syswm.msg->msg.win.lParam;
	}
}
#endif _WIN32