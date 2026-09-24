# CppStudy

C++ 学習用リポジトリ。SDL3 を使って簡単な2Dゲームエンジンのようなものを作りながら、C++20 の書き方やゲームループ・衝突判定などの基礎を学んでいます。

## 内容

- `Entity` を基底クラスとした簡単なエンティティ管理（`Player` / `Box`）
- `Scene` によるエンティティの一括更新・描画・AABB衝突判定
- キーボード入力処理（`Input`）とプレイヤー移動
- `SdlContext` / `Window` / `Texture` による RAII なリソース管理
- 固定色パレット（`Color`）、`Vec2` / `Vec3` などの基本的な数学ユーティリティ（`constexpr` 対応）

## 学習について

Claude（AI）とペアプログラミングしながら、C++20 の基礎とゲームプログラミングの定番パターンを学習しています。主に押さえたポイントは以下の通りです。

- 継承と仮想関数によるポリモーフィズム（`Entity` → `Player` / `Box`）
- `std::unique_ptr` によるオブジェクトの所有権管理
- RAII の考え方と、サブシステム初期化の寿命・破棄順（`SdlContext`）
- ムーブセマンティクス（`std::exchange` による所有権の移譲、二重解放の回避）
- `constexpr` によるコンパイル時計算
- AABB を用いた矩形同士の衝突判定
- ゲームループにおける入力サンプリングのタイミング
- CMake のサブディレクトリによるマルチターゲット構成（アプリごとの分割と共有ライブラリ）
- AddressSanitizer / UndefinedBehaviorSanitizer を使ったデバッグ

## ビルド環境

- C++20
- [SDL3](https://www.libsdl.org/)
- AddressSanitizer / UndefinedBehaviorSanitizer 有効化でビルド

`flake.nix` に Nix 開発シェル（GCC, GDB, CMake, SDL3 など）を定義しているので、Nix を使える場合は次で環境に入れます。

```sh
nix develop
```

## ビルド & 実行

### CMake

```sh
cmake -B build
cmake --build build
```

3つのターゲットができます。

| ターゲット | 内容 |
| --- | --- |
| `./build/engine` | SDL3 のゲームループデモ（本体） |
| `./build/raii_demo` | RAII / ムーブセマンティクスの練習デモ（SDL 不要） |
| `./build/test_math` | 数学ユーティリティのテスト |

`engine` を実行すると、プレイヤー（矩形）をキーボードで動かしつつ、跳ね返る3つの箱との衝突を検出してコンソールに表示するデモが起動します。

### Windows 向けクロスビルド

`flake.nix` の devShell に mingw-w64 のクロスコンパイラ（`pkgsCross.mingwW64`）が
入っているので、WSL 上から Windows の `.exe` を直接生成できる。

```sh
cmake -B build-win -DCMAKE_TOOLCHAIN_FILE=cmake/mingw-w64.cmake
cmake --build build-win
./build-win/test_math.exe        # WSL interop でそのまま Windows 上で起動する
```

ネイティブ用の `build/` とは別ディレクトリなので、両方を並行して持てる。
クロス側では `engine` と `raii_demo`（SDL 依存）は構成されず、OS 非依存の
`test_math` と、今後の `app/win32gui` だけがビルド対象になる。

AddressSanitizer / UndefinedBehaviorSanitizer は mingw-w64 の GCC が
サポートしていないため、`CMakeLists.txt` 側で Windows ターゲットのときだけ
無効化している。

### テスト

```sh
ctest --test-dir build --output-on-failure
```

外部のテストフレームワークには依存していません。`constexpr` にできる検証は `static_assert` で書いてあるので、コンパイルが通った時点で検証済みです。

## ディレクトリ構成

```
core/               # OS 非依存の共有ヘッダ（全ターゲットから参照）
├── aabb.h          # AABB（矩形）の衝突判定
├── vec2.h / vec3.h # ベクトル演算
└── color.h         # カラーパレット

app/
├── sdl_engine/     # SDL3 ゲームループデモ → build/engine
│   ├── main.cpp        # エントリポイント・ゲームループ
│   ├── sdl_context.*   # SDL の初期化/終了（RAII）
│   ├── window.*        # SDL ウィンドウ/レンダラーのラッパー
│   ├── entity.*        # Entity 基底クラス、Player / Box
│   ├── scene.*         # エンティティ管理・衝突判定
│   └── input.*         # キーボード入力
├── raii_demo/      # RAII / ムーブの練習デモ（SDL 不要） → build/raii_demo
│   ├── raii_demo.cpp
│   ├── texture.*       # RAII 練習用のテクスチャ
│   └── fake_gl.h       # 偽 OpenGL。テクスチャIDの生存を追跡してリーク・二重解放を報告する
└── win32gui/       # Windows GUI アプリ（作成予定）

tests/
└── test_math.cpp   # Vec2 / Vec3 / AABB のテスト
```
