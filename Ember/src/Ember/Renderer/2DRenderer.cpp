#include "EmberPch.h"
#include "2DRenderer.h"

constexpr int RenderingIndex = -1;

namespace ember {
    Renderer2D::Renderer2D(Window* window)
        : renderer(nullptr), window(nullptr) {
        if(window != nullptr){
            this->window = window;
            if (!Initializer())
                Destroy();
        }
    }

    Renderer2D::~Renderer2D() {
        Destroy();
    }

    SDL_Renderer* Renderer2D::Renderer(){
        return renderer;
    }

    bool Renderer2D::Initializer() {
       renderer = SDL_CreateRenderer((SDL_Window*) window->GetNativeWindow(), RenderingIndex,
                                     SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
       return (renderer != nullptr);
    }

    void Renderer2D::Destroy(){
        SDL_DestroyRenderer(renderer);
    }

    void Renderer2D::SetViewport(int x, int y, int w, int h) {
        SDL_Rect view_port = { x, y, w, h };
        SDL_RenderSetViewport(renderer, &view_port);
    }

    void Renderer2D::Clear(const Color& color) {
        SetColor(color);
        SDL_RenderClear(renderer);
    }

    void Renderer2D::SetColor(const Color& color) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    }

    void Renderer2D::Show() {
        SDL_RenderPresent(renderer);
    }

    void Renderer2D::Rectangle(const Rect& rect, const Color& color) {
        SetColor(color);
        SDL_RenderFillRect(renderer, &rect.rect);
    }

    void Renderer2D::Line(const IVec2& point1, const IVec2& point2, const Color& color) {
        SetColor(color);
        SDL_RenderDrawLine(renderer, point1.x, point1.y, point2.x, point2.y);
    }

    void Renderer2D::Point(const IVec2& point, const Color& color) {
        SetColor(color);
        SDL_RenderDrawPoint(renderer, point.x, point.y);
    }

    void Renderer2D::RectangleF(const FRect& rect, const Color& color) {
        SetColor(color);
        SDL_RenderFillRectF(renderer, &rect.rect);
    }

    void Renderer2D::LineF(const Vec2& point1, const Vec2& point2, const Color& color) {
        SetColor(color);
        SDL_RenderDrawLineF(renderer, point1.x, point1.y, point2.x, point2.y);
    }

    void Renderer2D::PointF(const Vec2& point, const Color& color) {
        SetColor(color);
        SDL_RenderDrawPointF(renderer, point.x, point.y);
    }
}
