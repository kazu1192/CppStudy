#pragma once
#include <string>

// テクスチャIDを所有する RAII クラス。生成で確保し、破棄で解放する。
// コピーすると同じIDを二重解放してしまうため、コピー禁止・ムーブのみ可能にしている。
class Texture {
  public:
    explicit Texture(const std::string& name);
    ~Texture();

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

    unsigned int id() const { return id_; };
    const std::string& name() const { return name_; }

  private:
    unsigned int id_ = 0;
    std::string name_;
};
