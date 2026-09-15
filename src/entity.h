#pragma once
#include "color.h"
#include "vec2.h"
#include <string>

struct SDL_Renderer;
struct AABB;

class Entity {
  public:
    Entity(std::string name, Vec2 pos, Vec2 vel)
        : position_(pos), velocity_(vel), name_(std::move(name)) {}

    virtual ~Entity() = default;

    Entity(const Entity&) = delete;
    Entity& operator=(const Entity&) = delete;

    virtual void update(float dt) = 0;
    virtual void render(SDL_Renderer* r) const = 0;
    virtual AABB bounds() const = 0;

    const std::string& name() const { return name_; }

    const Vec2& position() const { return position_; }
    void setPosition(const Vec2& p) { position_ = p; }

    void setColliding(bool v) { colliding_ = v; }
    bool isColliding() const { return colliding_; }

  protected:
    Vec2 position_;
    Vec2 velocity_;
    bool colliding_ = false;

  private:
    std::string name_;
};

class Box final : public Entity {
  public:
    Box(std::string name, Vec2 pos, Vec2 vel, float size, Color color, Vec2 bounds);

    void update(float dt) override;
    void render(SDL_Renderer* renderer) const override;
    AABB bounds() const override;

  private:
    float size_;
    Color color_;
    Vec2 bounds_;
};

class Input; // 前方宣言

class Player final : public Entity {
  public:
    Player(std::string name, Vec2 pos, float size, Color color, Vec2 bounds);

    void handleInput(const Input& input);
    void update(float dt) override;
    void render(SDL_Renderer* renderer) const override;
    AABB bounds() const override;

  private:
    float size_;
    float speed_ = 300.0f; // ピクセル秒
    Color color_;
    Vec2 bounds_;
};
