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

    void Renderer2D::Clear(Color& color) {
        SetColor(color);
        SDL_RenderClear(renderer);
    }

    void Renderer2D::SetColor(Color& color) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    }

    void Renderer2D::Show() {
        SDL_RenderPresent(renderer);
    }
}
