#include "entity.h"
#include "input.h"
#include "scene.h"
#include "sdl_context.h"
#include "window.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_timer.h>
#include <cstdio>
#include <memory>

int main(int, char**) {

    SdlContext sdl;
    if (!sdl.isValid()) {
        return 1;
    }

    Window window{"My Engine", 800, 600};
    if (!window.isValid()) {
        std::printf("ウインドウの作成に失敗しました\n");
        return 1;
    }

    Input input;
    Scene scene;

    auto player =
        std::make_unique<Player>("プレイヤー", Vec2{400, 300}, 40, colors::white, Vec2{800, 600});

    Player* playerPtr = player.get();
    scene.add(std::move(player));

    scene.add(std::make_unique<Box>("赤", Vec2{100, 100}, Vec2{220, 160}, 40, colors::red,
                                    Vec2(800, 600)));
    scene.add(std::make_unique<Box>("青", Vec2{300, 200}, Vec2{-180, 240}, 60, colors::blue,
                                    Vec2{800, 600}));
    scene.add(std::make_unique<Box>("黄", Vec2{500, 400}, Vec2{140, -200}, 30, colors::yellow,
                                    Vec2{800, 600}));

    Uint64 freq = SDL_GetPerformanceFrequency();
    Uint64 prev = SDL_GetPerformanceCounter();
    int frameCount = 0;

    Uint64 fpsTimer = SDL_GetTicks();
    bool running = true;

    while (running) {
        // --- 時間 ---
        Uint64 now = SDL_GetPerformanceCounter();
        float dt = static_cast<float>(now - prev) / static_cast<float>(freq); // 1秒あたり
        if (dt > 0.1f)
            dt = 0.1f;
        prev = now;

        input.update();

        // --- 入力 ---
        SDL_Event ev;
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_EVENT_QUIT)
                running = false;
            if (ev.type == SDL_EVENT_KEY_DOWN && ev.key.key == SDLK_ESCAPE) {
                running = false;
            }
        }

        playerPtr->handleInput(input);

        // --- 更新 ---
        scene.updateAll(dt);

        // --- 衝突判定 ---
        scene.clearCollisionFlags();
        for (const auto& [a, b] : scene.findCollisions()) {
            std::printf("衝突: %s <-> %s\n", a->name().c_str(), b->name().c_str());
            a->setColliding(true);
            b->setColliding(true);
        }

        SDL_SetRenderDrawColor(window.renderer(), colors::bg.r, colors::bg.g, colors::bg.b, 255);
        SDL_RenderClear(window.renderer());
        scene.renderAll(window.renderer());
        SDL_RenderPresent(window.renderer());

        // --- FPS計測 ---
        ++frameCount;
        if (SDL_GetTicks() - fpsTimer >= 1000) {
            std::printf("FPS: %d, dt: %f\n", frameCount, dt);
            frameCount = 0;
            fpsTimer = SDL_GetTicks();
        }
    }

    std::printf("正常終了\n");
    return 0;
}
