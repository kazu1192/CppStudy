#pragma once

class SdlContext {
  public:
    SdlContext();
    ~SdlContext();

    SdlContext(const SdlContext&) = delete;
    SdlContext& operator=(const SdlContext&) = delete;
    SdlContext(SdlContext&&) = delete;
    SdlContext& operator=(SdlContext&&) = delete;

    bool isValid() const { return ok_; }

  private:
    bool ok_ = false;
};
