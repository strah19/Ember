#include "Shape.h"

namespace Ember
{
    bool assert_renderer(SDL_Renderer* renderer)
    {
        return renderer != NULL;
    }

    void pixel(SDL_Renderer* renderer, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
    {
        if (assert_renderer(renderer)) {
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, r, g, b, a);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }

    void horizontal_line(SDL_Renderer* renderer, int x1, int x2, int y1, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
    {
        if (assert_renderer(renderer)) {
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, r, g, b, a);
            SDL_RenderDrawLine(renderer, x1, y1, x2, y1);
        }
    }

    void vertical_line(SDL_Renderer* renderer, int y1, int y2, int x1, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
    {
        if (assert_renderer(renderer)) {
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, r, g, b, a);
            SDL_RenderDrawLine(renderer, x1, y1, x1, y2);
        }
    }

    void line(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
    {
        if (assert_renderer(renderer)) {
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, r, g, b, a);
            SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
        }
    }

    void arcs(SDL_Renderer* renderer, int x0, int y0, int radiusX, int radiusY, int quad1, int quad2, int quad3, int quad4, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
    {
        if (assert_renderer(renderer)) {
            float pih = PI / 2.0f;

            const int prec = 27;
            float theta = 0;

            float x = (float)radiusX * cos(theta);
            float y = (float)radiusY * sin(theta);
            int x1 = (int)x;
            int y1 = (int)y;

            float step = pih / (float)prec;

            for (theta = step; theta <= pih; theta += step) {
                x1 = (float)radiusX * cosf(theta) + 0.5;
                y1 = (float)radiusY * sinf(theta) + 0.5;
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                SDL_SetRenderDrawColor(renderer, r, g, b, a);
                if (((int)x != x1) || ((int)y != y1)) {
                    if (quad1)
                        SDL_RenderDrawLine(renderer, x0 + (int)x, y0 - (int)y, x0 + x1, y0 - y1);
                    if (quad2)
                        SDL_RenderDrawLine(renderer, x0 - (int)x, y0 - (int)y, x0 - x1, y0 - y1);
                    if (quad3)
                        SDL_RenderDrawLine(renderer, x0 - (int)x, y0 + (int)y, x0 - x1, y0 + y1);
                    if (quad4)
                        SDL_RenderDrawLine(renderer, x0 + (int)x, y0 + (int)y, x0 + x1, y0 + y1);
                }

                x = (float)x1;
                y = (float)y1;
            }
        }
    }

    void rounded_rectangle_border(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
    {
        if (assert_renderer(renderer)) {
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            arcs(renderer, x1 + radius, y1, radius, radius, 0, 1, 0, 0, r, g, b, a);
            horizontal_line(renderer, x1 + radius, x2 - radius, y1 - radius, r, g, b, a);
            arcs(renderer, x2 - radius, y1, radius, radius, 1, 0, 0, 0, r, g, b, a);
            vertical_line(renderer, y1, y2, x1, r, g, b, a);
            arcs(renderer, x1 + radius, y2, radius, radius, 0, 0, 1, 0, r, g, b, a);
            vertical_line(renderer, y1, y2, x2, r, g, b, a);
            arcs(renderer, x2 - radius, y2, radius, radius, 0, 0, 0, 1, r, g, b, a);
            horizontal_line(renderer, x1 + radius, x2 - radius, y2 + radius, r, g, b, a);
        }
    }

    void draw_circle(SDL_Renderer* renderer, int centreX, int centreY, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
    {
        if (assert_renderer(renderer)) {
            const int diameter = (radius * 2);

            int x = (radius - 1);
            int y = 0;
            int tx = 1;
            int ty = 1;
            int error = (tx - diameter);
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, r, g, b, a);
            while (x >= y) {
                SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
                SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
                SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
                SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
                SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
                SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
                SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
                SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

                if (error <= 0) {
                    ++y;
                    error += ty;
                    ty += 2;
                }

                if (error > 0) {
                    --x;
                    tx += 2;
                    error += (tx - diameter);
                }
            }
        }
    }

    void fill_circle(SDL_Renderer* renderer, int centreX, int centreY, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
    {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, r, g, b, a);
        if (assert_renderer(renderer)) {
            for (double dy = 1; dy <= radius; dy += 1.0) {
                double dx = floor(sqrt((2.0 * radius * dy) - (dy * dy)));
                int x = centreX - (int)dx;
                SDL_RenderDrawLine(renderer, centreX - (int)dx, centreY + (int)dy - radius, centreX + (int)dx, centreY + (int)dy - radius);
                SDL_RenderDrawLine(renderer, centreX - (int)dx, centreY - (int)dy + radius, centreX + (int)dx, centreY - (int)dy + radius);
            }
        }
    }

    void rounded_rectangle_fill(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
    {
        y1 += radius;
        if (assert_renderer(renderer)) {
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, r, g, b, a);
            rounded_rectangle_border(renderer, x1, y1, x2, y2, radius, r, g, b);
            for (double dy = 1; dy <= radius; dy += 1.0) {
                double dx = floor(sqrt((2.0 * radius * dy) - (dy * dy)));
                SDL_RenderDrawLine(renderer, x1 - (int)dx + radius, y1 + (int)dy - radius, x1 + (int)dx / 4 + radius, y1 + (int)dy - radius);
            }
            for (double dy = 1; dy <= radius; dy += 1.0) {
                double dx = floor(sqrt((2.0 * radius * dy) - (dy * dy)));
                SDL_RenderDrawLine(renderer, x2 - (int)dx / 4 - radius, y1 + (int)dy - radius, x2 + (int)dx - radius, y1 + (int)dy - radius);
            }
            for (double dy = 1; dy <= radius; dy += 1.0) {
                double dx = floor(sqrt((2.0 * radius * dy) - (dy * dy)));
                SDL_RenderDrawLine(renderer, x1 - (int)dx + radius, y2 - (int)dy + radius, x1 + (int)dx / 4 + radius, y2 - (int)dy + radius);
            }
            for (double dy = 1; dy <= radius; dy += 1.0) {
                double dx = floor(sqrt((2.0 * radius * dy) - (dy * dy)));
                SDL_RenderDrawLine(renderer, x2 - (int)dx / 4 - radius, y2 - (int)dy + radius, x2 + (int)dx - radius, y2 - (int)dy + radius);
            }
            SDL_Rect MainBody = { x1, y1, x2 - x1, y2 - y1 };
            SDL_Rect TopBody = { x1 + radius, y1 - radius, x2 - x1 - radius * 2, radius };
            SDL_Rect BottomBody = { x1 + radius, y2, x2 - x1 - radius * 2, radius };

            SDL_RenderFillRect(renderer, &MainBody);
            SDL_RenderFillRect(renderer, &TopBody);
            SDL_RenderFillRect(renderer, &BottomBody);
        }
    }

    void rectangle_border(SDL_Renderer* renderer, int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
    {
        if (assert_renderer(renderer)) {
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, r, g, b, a);
            SDL_Rect Position = { x, y, w, h };
            SDL_RenderDrawRect(renderer, &Position);
        }
    }

    void rectangle_fill(SDL_Renderer* renderer, int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
    {
        if (assert_renderer(renderer)) {
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, r, g, b, a);
            SDL_Rect Position = { x, y, w, h };
            SDL_RenderFillRect(renderer, &Position);
        }
    }
}