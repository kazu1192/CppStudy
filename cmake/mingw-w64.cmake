# WSL / Nix の devShell から Windows 向けにクロスコンパイルするための設定。
#
#   cmake -B build-win -DCMAKE_TOOLCHAIN_FILE=cmake/mingw-w64.cmake
#   cmake --build build-win
#   ./build-win/win_app.exe      # WSL interop でそのまま Windows 上で起動する
#
# ツールチェーンは flake.nix の pkgsCross.mingwW64 から来る。

set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# mingw-w64 が公式ドキュメントで使っている三つ組
set(TOOLCHAIN_PREFIX x86_64-w64-mingw32)

set(CMAKE_C_COMPILER   ${TOOLCHAIN_PREFIX}-gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}-g++)
set(CMAKE_RC_COMPILER  ${TOOLCHAIN_PREFIX}-windres)

# ホスト（Linux）側のライブラリやヘッダを誤って拾わないようにする。
# PROGRAM だけは NEVER：ビルド中に動かす道具は Linux のものを使う。
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# CMAKE_FIND_ROOT_PATH はあえて空のまま。Nix のコンパイララッパーが mingw の
# ヘッダ/ライブラリのパスを自動で注入するので、Win32 の標準ライブラリ
# （user32 等）は名前でリンクするだけで通る。将来クロスビルド済みの
# サードパーティライブラリを使うときは、その store パスをここに足すこと。
