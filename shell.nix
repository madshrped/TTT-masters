{
  pkgs ? import <nixpkgs> { },
}:
pkgs.mkShell {
  buildInputs = with pkgs; [
    raylib
    glfw
    gcc
    pkg-config
    bear
    nlohmann_json
    wayland
    libxkbcommon
  ];
  shellHook = ''
    export LD_LIBRARY_PATH=${
      pkgs.lib.makeLibraryPath [
        pkgs.libGL
        pkgs.alsa-lib
        pkgs.pulseaudio
        pkgs.wayland
        pkgs.libxkbcommon
      ]
    }
  '';
}
