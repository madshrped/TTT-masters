{
  pkgs ? import <nixpkgs> { },
}:

pkgs.mkShell {
  buildInputs = with pkgs; [
    raylib
    gcc # or clang
    pkg-config
    bear
    nlohmann_json
  ];

  shellHook = ''
    export LD_LIBRARY_PATH=${
      pkgs.lib.makeLibraryPath [
        pkgs.raylib
        pkgs.libGL
        pkgs.xorg.libX11
        pkgs.xorg.libXcursor
        pkgs.xorg.libXi
        pkgs.xorg.libXrandr
        pkgs.alsa-lib
        pkgs.pulseaudio
      ]
    }
  '';
}
