#include "texture.h"
#include "fake_gl.h"
#include <utility>

Texture::Texture(const std::string& name) : name_(name) { glGenTextures(1, &id_); }
Texture::~Texture() { glDeleteTextures(1, &id_); }

Texture::Texture(Texture&& other) noexcept
    : id_(std::exchange(other.id_, 0)), name_(std::move(other.name_)) {}

Texture& Texture::operator=(Texture&& other) noexcept {
    if (this != &other) {
        glDeleteTextures(1, &id_);
        id_ = std::exchange(other.id_, 0);
        name_ = std::move(other.name_);
    }
    return *this;
}
