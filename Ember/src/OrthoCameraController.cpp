/**
 * @file OrthoCameraController.cpp
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
 * This file contains the ortho camera controller (2d camera).
 */

#include "OrthoCameraController.h"
#include "KeyboardCodes.h"
#include "EventHandler.h"

namespace Ember {
	constexpr float ZOOM_SPEED = 3.0f;

	/**
	* OrthoCameraController Constructor that creates an ortho camera.
	* 
	* @param const glm::vec2& window size to define the aspect ratio.
	*/
	OrthoCameraController::OrthoCameraController(const glm::vec2& window_size) {
		aspect_ratio = (float) window_size.x / (float) window_size.y;
		camera = OrthoCamera(aspect_ratio * zoom, -aspect_ratio * zoom, zoom, -zoom);
	}

	/**
	* Updates the cameras data.
	*/
	void OrthoCameraController::Update() {
		if (!freeze) {
			camera.SetPosition(camera_pos);
			if (KeyboardEvents::GetKeyboardState(EmberKeyCode::D))
				camera_pos.x += camera_speed.x;
			if (KeyboardEvents::GetKeyboardState(EmberKeyCode::A))
				camera_pos.x -= camera_speed.x;
			if (KeyboardEvents::GetKeyboardState(EmberKeyCode::W))
				camera_pos.y += camera_speed.y;
			if (KeyboardEvents::GetKeyboardState(EmberKeyCode::S))
				camera_pos.y -= camera_speed.y;
		}
	}

	/**
	* Updates the controllers events.
	* 
	* @param Event& current Ember event from the dispatcher.
	*/
	void OrthoCameraController::OnEvent(Event& event) {
		EventDispatcher dispatcher(&event);
		if (!freeze) 
			dispatcher.Dispatch<MouseWheelEvents>(EMBER_BIND_FUNC(MouseWheelHandler));
		dispatcher.Dispatch<ResizeEvent>(EMBER_BIND_FUNC(ResizeHandler));
	}

	/**
	* Mousewheel event handler.
	* 
	* @param const MouseWheelEvent& mousewheel event.
	*/
	void OrthoCameraController::MouseWheelHandler(const MouseWheelEvents& mousewheel) {
		zoom += mousewheel.direction * ZOOM_SPEED;
		camera.SetProjection(aspect_ratio * zoom, -aspect_ratio * zoom, zoom, -zoom);
	}

	/**
	* ResizeEvent event handler.
	*
	* @param const ResizeEvent& resize event.
	*/
	void OrthoCameraController::ResizeHandler(const ResizeEvent& resize) {
		aspect_ratio = (float) resize.w / (float) resize.h;
		camera.SetProjection(aspect_ratio * zoom, -aspect_ratio * zoom, zoom, -zoom);
	}
}