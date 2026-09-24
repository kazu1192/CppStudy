#include "sdl_context.h"
#include <SDL3/SDL.h>
#include <cstdio>

SdlContext::SdlContext() {
    ok_ = SDL_Init(SDL_INIT_VIDEO);
    if (!ok_) {
        std::printf("SDL_Init 失敗: %s\n", SDL_GetError());
    }
}

SdlContext::~SdlContext() {
    if (ok_) {
        SDL_Quit();
    }
}
