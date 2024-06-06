{
  description = "small 1d riemann-based hydrodynamics solver";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-parts.url = "github:hercules-ci/flake-parts";
  };

  outputs = inputs @ {self, ...}:
    inputs.flake-parts.lib.mkFlake {inherit inputs;} {
      systems = ["x86_64-linux" "aarch64-linux" "aarch64-darwin"];
      perSystem = {
        system,
        pkgs,
        ...
      }: let
        gccVersion = "14";
        gccPkg = pkgs."gcc${gccVersion}";

        llvmVersion = "18";
        llvmPkgs = pkgs."llvmPackages_${llvmVersion}";

        clangStdEnv = pkgs.stdenvAdapters.overrideCC llvmPkgs.stdenv (llvmPkgs.clang.override {
          bintools = llvmPkgs.bintools;
        });
        clangTools = pkgs."clang-tools_${llvmVersion}";
      in {
        devShells.default = pkgs.mkShell.override {stdenv = clangStdEnv;} {
          nativeBuildInputs = with pkgs;
            [
              ninja
              cppcheck
              ccls
              meson
              pkg-config
              just

              nil
              statix
              alejandra
            ]
            ++ [gccPkg llvmPkgs.clang clangTools];
          shellHook = ''
            PATH="${clangTools}/bin:$PATH"
          '';
        };
      };
    };
}
