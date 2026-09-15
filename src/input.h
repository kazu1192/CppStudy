#pragma once
#include "vec2.h"

class Input {
  public:
    void update();

    bool isDown(int scancode) const;
    bool wasPressed(int scancode) const;

    Vec2 moveAxis() const;

  private:
    const bool* current_ = nullptr;
    bool previous_[512] = {};
    int numKeys_ = 0;
};
