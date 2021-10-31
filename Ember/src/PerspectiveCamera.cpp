/**
 * @file PerspectiveCamera.cpp
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
 * This file contains the matrix transformations of a perspective camera.
 */

#include "PerspectiveCamera.h"

namespace Ember {
	/**
	* Perspective Camera Contructor.
	* 
	* @param float field of view.
	* @param float aspect ratio of the screen.
	*/
	PerspectiveCamera::PerspectiveCamera(float fov, float aspect_ratio) {
		SetProjection(fov, aspect_ratio);
	}

	/**
	* Updates the camera.
	* 
	* @param float field of view.
	* @param float aspect ratio of the screen.
	*/
	void PerspectiveCamera::SetProjection(float fov, float aspect_ratio) {
		CalculateView();
		projection_matrix = glm::perspective(fov, aspect_ratio, 0.1f, 100.0f);
	}
}