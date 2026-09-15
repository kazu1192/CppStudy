# CppStudy

C++ 学習用リポジトリ。SDL3 を使って簡単な2Dゲームエンジンのようなものを作りながら、C++20 の書き方やゲームループ・衝突判定などの基礎を学んでいます。

## 内容

- `Entity` を基底クラスとした簡単なエンティティ管理（`Player` / `Box`）
- `Scene` によるエンティティの一括更新・描画・AABB衝突判定
- キーボード入力処理（`Input`）とプレイヤー移動
- 固定色パレット（`Color`）、`Vec2` / `Vec3` などの基本的な数学ユーティリティ

`src/main.cpp` を実行すると、プレイヤー（矩形）をキーボードで動かしつつ、跳ね返る3つの箱との衝突を検出してコンソールに表示するデモが起動します。

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
./build/engine
```

### Makefile

```sh
make
./app
```

## ディレクトリ構成

```
src/
├── main.cpp       # エントリポイント・ゲームループ
├── window.*       # SDL ウィンドウ/レンダラーのラッパー
├── entity.*       # Entity 基底クラス、Player / Box
├── scene.*        # エンティティ管理・衝突判定
├── input.*        # キーボード入力
├── aabb.h         # AABB（矩形）の衝突判定
├── vec2.h / vec3.h# ベクトル演算
├── color.h        # カラーパレット
└── fake_gl.h      # 補助ヘッダ
```
