# Gorg

Gorg is a keyboard-driven GTKmm application launcher and command center. It can search desktop applications, run executables from `PATH`, evaluate expressions, open web searches, read dmenu-style options from stdin, and hand a query to an OpenAI-compatible chat completions endpoint.

## Features

- Desktop application search from `.desktop` files
- Executable search from `PATH`
- Expression evaluation with exprtk
- Web search through `xdg-open`
- Optional AI answers through an OpenAI-compatible streaming API
- User and system plugin loading
- JSON configuration and CSS overrides

## Installation

With Nix:

```bash
nix profile install github:lavalleeale/gorg
```

Or run without installing:

```bash
nix run github:lavalleeale/gorg
```

## Building From Source

Dependencies:

- C++20 compiler
- Meson and Ninja
- `gtkmm-3.0`
- `libcurl`
- `pkg-config`

Build and test:

```bash
meson setup build
meson compile -C build
meson test -C build
```

Run the local binary:

```bash
./build/gorg
```

With Nix:

```bash
nix develop
meson setup build
meson test -C build
```

## Usage

Start Gorg:

```bash
gorg
```

Useful options:

```bash
gorg --modes run,equation,web
gorg --starting-query "1 + 2 * 3"
gorg --auto-restore
gorg --big
gorg --option web.searchUrl=https://duckduckgo.com/?q=
```

Inside the prompt, prefix a query with `!plugin` to limit results to matching plugins:

```text
!equation 10 / 4
!web gtkmm launcher
```

Keyboard shortcuts:

- `Escape`: close Gorg
- `Enter`: run the first visible result
- `Ctrl+Enter`: toggle larger result icons/window size

## Configuration

Gorg reads configuration from `$XDG_CONFIG_HOME/gorg` or `~/.config/gorg`.

Example `config.json`:

```json
{
  "windowWidth": 600,
  "windowHeight": 200,
  "maxResults": 25,
  "imageSize": 32,
  "plugins": {
    "web": {
      "searchUrl": "https://www.google.com/search?q=",
      "relevance": 0.5
    },
    "ai": {
      "apiUrl": "https://api.openai.com/v1/chat/completions",
      "apiKey": "",
      "model": "gpt-3.5-turbo",
      "maxWidth": 50,
      "relevance": 0.5
    }
  }
}
```

Optional `style.css` in the same directory overrides the built-in GTK CSS.

## Plugins

Built-in plugin names:

- `drun`: executables from `PATH`
- `run`: desktop applications from `XDG_DATA_DIRS`
- `equation`: mathematical expressions
- `web`: web search
- `ai`: chat completions request
- `dmenu`: newline-delimited choices from stdin

External plugins are loaded from:

- `$XDG_CONFIG_HOME/gorg/plugins`
- `~/.config/gorg/plugins`
- `GORG_SYSTEM_PLUGIN_PATH`, or `/usr/local/share/gorg/plugins` by default

Plugins implement the `Plugin` interface and export `extern "C" Plugin *create()`. Matches returned from `getMatches` must remain valid until the next plugin update or plugin destruction; the caller treats them as borrowed pointers.

## Verification

Run the normal local checks with:

```bash
meson test -C build
nix flake check
```

The Nix package runs the Meson test suite during its check phase.
