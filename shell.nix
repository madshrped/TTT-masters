{
  pkgs ? import <nixpkgs> { },
}:
let
  raylibStatic = pkgs.raylib.overrideAttrs (old: {
    cmakeFlags = (old.cmakeFlags or [ ]) ++ [
      "-DBUILD_SHARED_LIBS=OFF"
    ];
  });
  glfwStatic = pkgs.glfw.overrideAttrs (old: {
    cmakeFlags = (old.cmakeFlags or [ ]) ++ [
      "-DBUILD_SHARED_LIBS=OFF"
    ];
  });
in
pkgs.mkShell {
  buildInputs = with pkgs; [
    raylibStatic
    glfwStatic
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
        pkgs.libGL
        pkgs.alsa-lib
        pkgs.pulseaudio
        pkgs.xorg.libX11
        pkgs.wayland
        pkgs.libxkbcommon
      ]
    }
  '';
}
