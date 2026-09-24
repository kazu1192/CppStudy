{
  description = "Temporary C/C++ development environment with GCC and GDB";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
    }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
        # Windows 向けクロスコンパイル用ツールチェーン（x86_64-w64-mingw32-*）
        mingw = pkgs.pkgsCross.mingwW64;
      in
      {
        devShells.default = pkgs.mkShell {
          name = "gcc-gdb-env";

          packages =
            (with pkgs; [
              gcc
              gdb
              gnumake
              cmake
              pkg-config
              sdl3
              clang-tools
            ])
            ++ [
              # クロスコンパイラと windres（.rc のコンパイル用）
              mingw.buildPackages.gcc
              mingw.buildPackages.binutils
            ];

          shellHook = ''
            echo "GCC $(gcc --version | head -1)"
            echo "GDB $(gdb --version | head -1)"
            echo "MinGW $(x86_64-w64-mingw32-g++ --version | head -1)"
            export LSAN_OPTIONS=suppressions=$PWD/.lsan-suppressions
          '';
        };
      }
    );
}
