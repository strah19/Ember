#ifndef MY_SHAPE_H
#define MY_SHAPE_H

#include <SDL.h>
#include <iostream>
#include <cmath>

#define PI 3.141592

namespace Ember
{
	bool assert_renderer(SDL_Renderer* renderer); //check that the renderer isn't NULL

	void pixel(SDL_Renderer* renderer, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255); //draw single pixel
	void horizontal_line(SDL_Renderer* renderer, int x1, int x2, int y1, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255); //only horizontal line
	void line(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255); //any line
	void vertical_line(SDL_Renderer* renderer, int y1, int y2, int x1, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255); //only vertical line
	void arcs(SDL_Renderer* renderer, int x0, int y0, int radiusX, int radiusY, int quad1, int quad2, int quad3, int quad4, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255); //draw arcs according to the quad
	/*
	quad1 = TopRight
	quad2 = TopLeft
	quad3 = BottomLeft
	quad4 = BottomRight
	*/
	void rounded_rectangle_border(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
	void draw_circle(SDL_Renderer* renderer, int centreX, int centreY, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255); //only borders
	void fill_circle(SDL_Renderer* renderer, int centreX, int centreY, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
	void rounded_rectangle_fill(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
	void rectangle_border(SDL_Renderer* renderer, int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
	void rectangle_fill(SDL_Renderer* renderer, int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
}

#endif // !MY_SHAPE_H
