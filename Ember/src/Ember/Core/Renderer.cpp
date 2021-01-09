#include "Renderer.h"

constexpr int RenderingIndex = -1;

namespace Ember {
	rRenderer::rRenderer(Window* window)
		: renderer(nullptr), window(nullptr) {
		if (window != nullptr) {
			this->window = window;
			if (!Initializer())
				Destroy();
		}
	}

	rRenderer::~rRenderer() {
		Destroy();
	}

	SDL_Renderer* rRenderer::Renderer() {
		return renderer;
	}

	bool rRenderer::Initializer() {
		renderer = SDL_CreateRenderer(window->GetNativeWindow(), RenderingIndex,
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	
		return (renderer != nullptr);
	}

	void rRenderer::Destroy() {
		SDL_DestroyRenderer(renderer);
	}

	void rRenderer::SetViewport(int x, int y, int w, int h) {
		SDL_Rect view_port = { x, y, w, h };
		SDL_RenderSetViewport(renderer, &view_port);
	}

	void rRenderer::Clear(const Color& color) {
		SetColor(color);
		SDL_RenderClear(renderer);
	}

	void rRenderer::SetColor(const Color& color) {
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	}

	void rRenderer::Show() {
		SDL_RenderPresent(renderer);
	}

	void rRenderer::Rectangle(const Rect& rect, const Color& color) {
		SetColor(color);
		SDL_RenderFillRect(renderer, &rect.rect);
	}

	void rRenderer::Border(const Rect& rect, const Color& color) {
		SetColor(color);
		SDL_RenderDrawRect(renderer, &rect.rect);
	}

	void rRenderer::Line(const IVec2& point1, const IVec2& point2, const Color& color) {
		SetColor(color);
		SDL_RenderDrawLine(renderer, point1.x, point1.y, point2.x, point2.y);
	}

	void rRenderer::Point(const IVec2& point, const Color& color) {
		SetColor(color);
		SDL_RenderDrawPoint(renderer, point.x, point.y);
	}

	void rRenderer::RectangleF(const FRect& rect, const Color& color) {
		SetColor(color);
		SDL_RenderFillRectF(renderer, &rect.rect);
	}

	void rRenderer::BorderF(const FRect& rect, const Color& color) {
		SetColor(color);
		SDL_RenderDrawRectF(renderer, &rect.rect);
	}

	void rRenderer::LineF(const Vec2& point1, const Vec2& point2, const Color& color) {
		SetColor(color);
		SDL_RenderDrawLineF(renderer, point1.x, point1.y, point2.x, point2.y);
	}

	void rRenderer::PointF(const Vec2& point, const Color& color) {
		SetColor(color);
		SDL_RenderDrawPointF(renderer, point.x, point.y);
	}

	void rRenderer::DrawCircle(const IVec2& position, int radius, const Color& color) {
		const int diameter = (radius * 2);

		int x = (radius - 1);
		int y = 0;
		int tx = 1;
		int ty = 1;
		int error = (tx - diameter);
		SetColor(color);
		while (x >= y) {
			SDL_RenderDrawPoint(renderer, position.x + x, position.y - y);
			SDL_RenderDrawPoint(renderer, position.x + x, position.y + y);
			SDL_RenderDrawPoint(renderer, position.x - x, position.y - y);
			SDL_RenderDrawPoint(renderer, position.x - x, position.y + y);
			SDL_RenderDrawPoint(renderer, position.x + y, position.y - x);
			SDL_RenderDrawPoint(renderer, position.x + y, position.y + x);
			SDL_RenderDrawPoint(renderer, position.x - y, position.y - x);
			SDL_RenderDrawPoint(renderer, position.x - y, position.y + x);

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

	void rRenderer::FillCircle(const IVec2& position, int radius, const Color& color) {
		SetColor(color);

		for (double dy = 1; dy <= radius; dy += 1.0) {
			double dx = floor(sqrt((2.0 * radius * dy) - (dy * dy)));
			int x = position.x - (int)dx;
			SDL_RenderDrawLine(renderer, position.x - (int)dx, position.y + (int)dy - radius, position.x + (int)dx, position.y + (int)dy - radius);
			SDL_RenderDrawLine(renderer, position.x - (int)dx, position.y - (int)dy + radius, position.x + (int)dx, position.y - (int)dy + radius);
		}
	}

	void rRenderer::Curve(IVec2 pos[], const Color& color) {
		for (float t = 0.0f; t <= 1.0f; t += 0.001f) {
			float px = ((1 - t) * (1 - t)) * pos[0].x + 2 * (1 - t) * t * pos[1].x + (t * t) * pos[2].x;
			float py = ((1 - t) * (1 - t)) * pos[0].y + 2 * (1 - t) * t * pos[1].y + (t * t) * pos[2].y;
			PointF({ px, py }, color);
		}
	}

	void rRenderer::AdvCurve(IVec2 pos[], const Color& color) {
		for (float t = 0.0f; t <= 1.0f; t += 0.001f) {
			float px = ((1 - t) * (1 - t) * (1 - t)) * pos[0].x + 3 * ((1 - t) * (1 - t)) * t * pos[1].x + 3 * (1 - t) * (t * t) * pos[2].x + (t * t * t) * pos[3].x;
			float py = ((1 - t) * (1 - t) * (1 - t)) * pos[0].y + 3 * ((1 - t) * (1 - t)) * t * pos[1].y + 3 * (1 - t) * (t * t) * pos[2].y + (t * t * t) * pos[3].y;
			PointF({ px, py }, color);
		}
	}
}