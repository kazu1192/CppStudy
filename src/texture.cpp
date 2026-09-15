#include "texture.h"
#include "fake_gl.h"

Texture::Texture(const std::string& name) : name_(name) { glGenTextures(1, &id_); }
Texture::~Texture() { glDeleteTextures(1, &id_); }

Texture::Texture(Texture&& other) noexcept : id_(other.id_), name_(std::move(other.name_)) {
    other.id_ = 0;
}

Texture& Texture::operator=(Texture&& other) noexcept {
    if (this != &other) {
        glDeleteTextures(1, &id_);
        name_ = std::move(other.name_);
        other.id_ = 0;
    }
    return *this;
}
