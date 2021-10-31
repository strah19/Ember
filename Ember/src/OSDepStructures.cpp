/**
 * @file OSDepStructures.cpp
 * @author strah19
 * @date October 3 2021
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the MIT License as
 * published by the Free Software Foundation.
 *
 * @section DESCRIPTION
 *
 * This file fetchs window handlers for specific OS's.
 */

#include "OSDepStructures.h"

#ifdef _WIN32   
namespace Ember {
	/**
	* Will get the window handler in Windows.
	* 
	* @param Window* a pointer to the current application window.
	* 
	* @return HWND native window handler.
	*/
	HWND GetNativeWindowHandler(Ember::Window* window) {
		SDL_SysWMinfo wmInfo;
		SDL_VERSION(&wmInfo.version);
		SDL_GetWindowWMInfo(static_cast<SDL_Window*>(window->GetNativeWindow()), &wmInfo);
		return wmInfo.info.win.window;
	}

	/**
	* Will fetch the Windows msg.
	* 
	* @param EvenetHandler* a pointer to the event handler.
	* 
	* @return UINT get the msg of the current window.
	*/
	UINT GetNativeMsg(Ember::EventHandler* handler) {
		return handler->NativeEvent()->syswm.msg->msg.win.msg;
	}

	/**
	* Will fetch the wparam of the native Window.
	* 
	* @param EvenetHandler* a pointer to the event handler.
	* 
	* @param WPARAM get the wparam of the window.
	*/
	WPARAM GetWParam(Ember::EventHandler* handler) {
		return handler->NativeEvent()->syswm.msg->msg.win.wParam;
	}

	/**
	* Will fetch the lparam of the native Window.
	*
	* @param EvenetHandler* a pointer to the event handler.
	*
	* @param LPARAM get the lparam of the window.
	*/
	LPARAM GetLParam(Ember::EventHandler* handler) {
		return handler->NativeEvent()->syswm.msg->msg.win.lParam;
	}
}
#endif _WIN32