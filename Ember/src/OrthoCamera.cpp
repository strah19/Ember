/**
 * @file OrthoCamera.cpp
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
 * This file contains the ortho camera object.
 */

#include "OrthoCamera.h"

namespace Ember {
	/**
	* OrthoCamera Contructor that defines the coordinate system of the camera..
	*
	* @param float the left side of the screen.
	* @param float the right side of the screen.
	* @param float the bottom side of the screen.
	* @param float the top side of the screen.
	*/
	OrthoCamera::OrthoCamera(float left, float right, float bottom, float top) {
		SetProjection(left, right, bottom, top);
	}

	/**
	* Set the matrix of the camera.
	*
	* @param float the left side of the screen.
	* @param float the right side of the screen.
	* @param float the bottom side of the screen.
	* @param float the top side of the screen.
	*/
	void OrthoCamera::SetProjection(float left, float right, float bottom, float top) {
		projection_matrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		this->left = left;
		this->right = right;
		this->bottom = bottom;
		this->top = top;
		CalculateView();
	}
}