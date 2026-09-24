---
name: run-engine
description: Build and run this repo's SDL3 app (build/engine), its raii_demo, and its tests. Use whenever asked to run, start, or verify the engine, or to confirm a change works in the real app rather than only in tests. Covers the sandbox limitation that stops a real window from opening and the headless recipe that works instead.
---

# Running the engine

Three CMake targets. Build them all first:

```sh
cmake -B build && cmake --build build
```

| Target | Needs a display? |
|---|---|
| `build/engine` | yes — the SDL3 game loop demo |
| `build/raii_demo` | no |
| `build/test_math` | no |

Run the two headless ones directly; they exit on their own:

```sh
./build/raii_demo
ctest --test-dir build --output-on-failure
```

`raii_demo` is not just a demo — `fake_gl.h` tracks texture id lifetimes, so
`[GL] リークなし ✓` is the check that `Texture`'s move constructor and move
assignment still transfer ownership exactly once.

## engine: a real window does not open in the Bash sandbox

The sandbox blocks `socket(AF_UNIX, SOCK_STREAM)` with `EPERM`, so SDL can
reach neither the WSLg Wayland socket nor the X11 one:

```
Failed to connect to the Wayland display server: Operation not permitted
Failed to connect to the X11 display server
SDL_Init 失敗: No available video device
```

This is the sandbox, not the machine and not the build. WSLg is up
(`DISPLAY=:0`, `WAYLAND_DISPLAY=wayland-0`) and this SDL3 does include the x11
and wayland backends — confirm with `SDL_GetNumVideoDrivers()` if in doubt.
Do not burn time on `LD_LIBRARY_PATH`, `SDL_VIDEODRIVER=x11|wayland`, or
hunting for `libX11.so.6` in `/nix/store`; none of it reaches the syscall.

**For anything visual, or for real keyboard input, ask the user to run it.**
In an interactive session they can type:

```
! ./build/engine
```

That runs outside the sandbox and its output comes back into the conversation.

## engine: headless recipe that does work

```sh
export LSAN_OPTIONS=suppressions=$PWD/.lsan-suppressions
SDL_VIDEODRIVER=dummy SDL_RENDER_DRIVER=software \
  script -qec "timeout -s TERM 5 ./build/engine" /dev/null
```

Each piece matters:

- **`dummy`, not `offscreen`.** On WSL the `offscreen` driver hangs retrying
  `MESA: error: Failed to create DXCore adapter factory`, never reaches the
  game loop, and its Mesa worker thread trips LeakSanitizer with frames that
  are all `<unknown module>`. Those leaks are Mesa's, not this code's.
- **`script -qec ... /dev/null`** gives the process a pty so stdout is line
  buffered. Without it, output redirected to a file is lost when the process
  is signalled. Do **not** reach for `stdbuf`: it works via `LD_PRELOAD`, and
  ASan aborts with "ASan runtime does not come first in initial library list".
- **`timeout -s TERM`** is a clean shutdown, not a kill. SDL installs a SIGTERM
  handler that posts `SDL_EVENT_QUIT`, so the loop exits normally, `正常終了`
  prints, and LeakSanitizer runs. That last part is the point — it is what
  makes this a real check of the `SdlContext` / `Window` teardown.

`timeout` reports exit status 124 even on that clean exit. Judge the run by
the log, not the status.

### What a healthy run looks like

```
[Window] コンストラクタ: My Engine
[Window] 生成完了: window_=0x... renderer_=0x...
[Window] レンダラ: software
 Scene: プレイヤー を追加 / 赤 / 青 / 黄
FPS: 60, dt: 0.016667          ← ~60fps, dt steady near 16.67ms
衝突: プレイヤー <-> 赤          ← in bursts, with quiet gaps between
正常終了
[Window] デストラクタ: window_=0x...
```

Check, in order: all four entities registered; FPS near 60 with a steady `dt`;
collisions arriving in bursts separated by quiet seconds (the boxes bounce in
and out — continuous collisions every frame, or none at all, means the AABB
logic broke); `正常終了` and the destructor line both present; and **no ASan,
UBSan or LeakSanitizer output at all**.

### What this does not cover

`Input::update()` runs every frame but no key is ever down, so `moveAxis()`
stays zero and the player never moves under its own power. Player collisions
seen here come from a box bouncing into it. Driving actual input needs the
user and `! ./build/engine`.
