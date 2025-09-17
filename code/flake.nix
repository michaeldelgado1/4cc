{
  description = "4coder Environment";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
  };

  outputs = inputs:
    let
      supportedSystems = [ "x86_64-linux" ];
      forEachSupportedSystem = f: inputs.nixpkgs.lib.genAttrs supportedSystems (system: f {
        pkgs = import inputs.nixpkgs {
          inherit system;
        };
      });
    in
    {
      devShells = forEachSupportedSystem ({ pkgs }: {
        default = pkgs.mkShell {
          packages = with pkgs; [
            # Linux specific Libs
            xorg.libX11
            xorg.libX11.dev
            xorg.libX11.dev.out
            xorg.libXcursor
            xorg.libXrandr
            xorg.libXinerama
            xorg.libXi
            libxkbcommon
            libGL
            alsa-lib
            freetype
            fontconfig
          ];
        };
      });
    };
}
