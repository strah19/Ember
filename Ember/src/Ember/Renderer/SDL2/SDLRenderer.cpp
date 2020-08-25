#include "SDLRenderer.h"

constexpr int RenderingIndex = -1;

namespace ember {
    SDLRenderer::SDLRenderer(Window* window)
        : renderer(nullptr), window(nullptr) {
        if(window != nullptr){
            this->window = window;
            if (!Initializer())
                Destroy();
        }
    }

    SDLRenderer::~SDLRenderer() {
        Destroy();
    }

    SDL_Renderer* SDLRenderer::Renderer(){
        return renderer;
    }

    bool SDLRenderer::Initializer() {
       renderer = SDL_CreateRenderer((SDL_Window*) window->GetNativeWindow(), RenderingIndex,
                                     SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
       return (renderer != nullptr);
    }

    void SDLRenderer::Destroy(){
        SDL_DestroyRenderer(renderer);
    }

    void SDLRenderer::SetViewport(int x, int y, int w, int h) {
        SDL_Rect view_port = { x, y, w, h };
        SDL_RenderSetViewport(renderer, &view_port);
    }

    void SDLRenderer::Clear(Color& color) {
        SetColor(color);
        SDL_RenderClear(renderer);
    }

    void SDLRenderer::SetColor(Color& color) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    }

    void SDLRenderer::Show() {
        SDL_RenderPresent(renderer);
    }
}
