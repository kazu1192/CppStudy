#include "texture.h"
#include "fake_gl.h"
#include <utility>

Texture::Texture(const std::string& name) : name_(name) { glGenTextures(1, &id_); }
Texture::~Texture() { glDeleteTextures(1, &id_); }

// ID を奪った側は 0（無効ID）にしておく。移動元が破棄されても glDeleteTextures が無視する。
Texture::Texture(Texture&& other) noexcept
    : id_(std::exchange(other.id_, 0)), name_(std::move(other.name_)) {}

Texture& Texture::operator=(Texture&& other) noexcept {
    // 自己代入だと自分のIDを消してしまうので除外し、既存のテクスチャは先に解放してから奪う
    if (this != &other) {
        glDeleteTextures(1, &id_);
        id_ = std::exchange(other.id_, 0);
        name_ = std::move(other.name_);
    }
    return *this;
}
