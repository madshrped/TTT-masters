{
  pkgs ? import <nixpkgs> { },
}:
let
  muslPkgs = pkgs.pkgsMusl;
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
  buildInputs = [
    raylibStatic
    glfwStatic
    muslPkgs.gcc
    pkgs.pkg-config
    pkgs.bear
    pkgs.nlohmann_json
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
