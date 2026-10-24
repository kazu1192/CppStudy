#pragma once

// SDL ライブラリ全体の初期化と終了を担う RAII クラス。
// Window など他の SDL オブジェクトより先に生成し、後に破棄すること。
// 初期化の成否は isValid() で確認する。
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
