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
  ];
  shellHook = ''
    export LD_LIBRARY_PATH=${
      pkgs.lib.makeLibraryPath [
        pkgs.raylib
        pkgs.libGL
        pkgs.alsa-lib
        pkgs.pulseaudio
      ]
    }
  '';
}
