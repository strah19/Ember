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
		renderer = SDL_CreateRenderer((SDL_Window*)window->GetNativeWindow(), RenderingIndex,
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

}