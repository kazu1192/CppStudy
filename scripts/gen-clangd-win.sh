#!/usr/bin/env bash
# app/win32gui/.clangd を生成する。
#
# なぜ必要か:
#   Nix のクロスコンパイララッパーは mingw のシステムインクルードを
#   コマンドラインではなく自分の nix-support/libc-cflags から注入する。
#   そのため compile_commands.json には現れず、clangd からは見えない。
#   ここでドライバに検索パスを問い合わせて .clangd に焼き込む。
#
# いつ実行するか:
#   flake.lock を更新して /nix/store のパスが変わったとき。
#   （clangd が急に windows.h を見失ったら、まずこれを疑う）

set -euo pipefail

root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
out="$root/app/win32gui/.clangd"

# devShell の中なら PATH から、外なら store から拾う
cc="$(command -v x86_64-w64-mingw32-g++ 2>/dev/null \
      || ls /nix/store/*/bin/x86_64-w64-mingw32-g++ 2>/dev/null | head -1)"
if [ -z "$cc" ]; then
  echo "x86_64-w64-mingw32-g++ が見つからない。nix develop を試すこと。" >&2
  exit 1
fi
echo "driver: $cc" >&2

{
  echo "# scripts/gen-clangd-win.sh が生成。手で編集しないこと。"
  echo "CompileFlags:"
  echo "  CompilationDatabase: ../../build-win"
  echo "  Add:"

  "$cc" -E -x c++ -v /dev/null 2>&1 \
    | sed -n '/#include <...> search starts here:/,/End of search list./p' \
    | grep '^ /nix/store' \
    | grep 'x86_64-w64-mingw32' `# devShell のホスト側インクルード(sdl3 等)を除く` \
    | grep -v 'lib/gcc/x86_64-w64-mingw32/[0-9.]*/include' `# gcc の組み込みヘッダは clang 自身のものを使わせる` \
    | sed 's|^ ||; s|//|/|g' \
    | while read -r d; do
        [ -d "$d" ] && echo "    - -isystem$d"
      done
} > "$out"

echo "書き出した: $out" >&2
