#pragma once
#include "color.h"
#include "vec2.h"
#include <string>

struct SDL_Renderer;
struct AABB;

// シーンに置かれるオブジェクトの基底クラス。
// Scene が unique_ptr<Entity> で所有するため、デストラクタは virtual にしてある。
// 所有者が一意なので、コピーは禁止している。
class Entity {
  public:
    Entity(std::string name, Vec2 pos, Vec2 vel)
        : position_(pos), velocity_(vel), name_(std::move(name)) {}

    virtual ~Entity() = default;

    Entity(const Entity&) = delete;
    Entity& operator=(const Entity&) = delete;

    // dt は前フレームからの経過秒
    virtual void update(float dt) = 0;
    virtual void render(SDL_Renderer* r) const = 0;
    virtual AABB bounds() const = 0;

    const std::string& name() const { return name_; }

    const Vec2& position() const { return position_; }
    void setPosition(const Vec2& p) { position_ = p; }

    // 衝突中かどうか。毎フレーム Scene が付け直し、描画色の切り替えに使う。
    void setColliding(bool v) { colliding_ = v; }
    bool isColliding() const { return colliding_; }

  protected:
    Vec2 position_;
    Vec2 velocity_;
    bool colliding_ = false;

  private:
    std::string name_;
};

// 画面端で跳ね返りながら動き続ける正方形。bounds は移動可能範囲（画面サイズ）。
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

// キー入力で動かす正方形。Box と違い、画面端では跳ね返らず止まる。
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
