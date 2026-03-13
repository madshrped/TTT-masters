{
  pkgs ? import <nixpkgs> { },
}:
pkgs.mkShell {
  buildInputs = with pkgs; [
    raylib
    gcc
    pkg-config
    bear
    nlohmann_json
    xorg.libX11
    xorg.libXcursor
    xorg.libXrandr
    xorg.libXinerama
    xorg.libXi
    wayland
    libxkbcommon
  ];
  shellHook = ''
      export LD_LIBRARY_PATH=${
        pkgs.lib.makeLibraryPath [
          pkgs.raylib
          pkgs.libGL
          pkgs.alsa-lib
          pkgs.pulseaudio
          pkgs.xorg.libX11
          pkgs.xorg.libXcursor
          pkgs.xorg.libXrandr
          pkgs.xorg.libXinerama
          pkgs.xorg.libXi
          pkgs.wayland
          pkgs.libxkbcommon
        ]
      }
    unset WAYLAND_DISPLAY 
    export DISPLAY=:0
  '';
}
