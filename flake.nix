{
  description = "Gorg launcher";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
        dependencies = with pkgs; [
          gtkmm3
          curl
          cmark
          atkmm
          atk
          gtk3
          glib
          glibmm
          pango
          pangomm
          cairo
          cairomm
          gdk-pixbuf
          zlib
          libsigcxx
          harfbuzz
          stdenv.cc.cc.lib # For libstdc++
          meson
          ninja
        ];
      in {
        packages = {
          default = self.packages.${system}.gorg;
          gorg = pkgs.stdenv.mkDerivation {
            pname = "gorg";
            version = "0.6.0";

            src = ./.;

            nativeBuildInputs = [ pkgs.pkg-config pkgs.makeWrapper ];

            buildInputs = dependencies;

            configurePhase = ''
              meson setup build
            '';

            buildPhase = ''
              meson compile -C build
            '';

            installPhase = ''
              mkdir -p $out/bin
              cp build/gorg $out/bin/

              # Wrap the executable with proper environment variables
              wrapProgram $out/bin/gorg \
                --prefix LD_LIBRARY_PATH : "${
                  pkgs.lib.makeLibraryPath dependencies
                }" \
                --prefix GIO_EXTRA_MODULES : "${pkgs.glib-networking}/lib/gio/modules" \
                --prefix XDG_DATA_DIRS : "$XDG_ICON_DIRS:$GSETTINGS_SCHEMAS_PATH" \
                --set GSETTINGS_SCHEMAS_PATH "${pkgs.gsettings-desktop-schemas}/share/gsettings-schemas/${pkgs.gsettings-desktop-schemas.name}"
            '';

            meta = {
              description = "Gorg launcher";
              platforms = pkgs.lib.platforms.linux;
            };
          };
        };

        devShells.default = pkgs.mkShell {
          nativeBuildInputs = [ pkgs.pkg-config pkgs.gdb ];
          buildInputs = dependencies;
        };
      });
}
