#pragma once
#include "vec2.h"

// キーボード入力の状態管理。毎フレーム update() を呼んでから問い合わせる。
// scancode は SDL_Scancode の値。
class Input {
  public:
    // 前フレームの状態を退避して、最新のキー状態を取り込む
    void update();

    // 押され続けているか
    bool isDown(int scancode) const;
    // このフレームで押された瞬間か（前フレームは未押下で、今は押下）
    bool wasPressed(int scancode) const;

    // 矢印キー / WASD から移動方向を返す。斜め入力でも長さ 1 に正規化済み。
    Vec2 moveAxis() const;

  private:
    static constexpr int kMaxKeys = 512;

    // SDL が所有する配列への参照（コピーではない）。解放は不要。
    const bool* current_ = nullptr;
    bool previous_[512] = {};
    int numKeys_ = 0;
};
