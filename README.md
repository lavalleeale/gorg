# Gorg - A GTK Application Launcher and Command Center

Gorg is a keyboard-driven application launcher and command center built with GTK that provides quick access to applications, calculations, and even AI assistance.

## Features

- **Application Launcher**: Quickly find and launch desktop applications
- **Command Execution**: Run terminal commands directly
- **Expression Evaluation**: Calculate mathematical expressions on the fly
- **AI Assistance**: Ask questions to an AI assistant (using LLama model)
- **Modern Interface**: Clean, transparent UI with fuzzy search capabilities

## Installation

### Using Nix

If you're using the Nix package manager, you can install Gorg directly from the flake:

```bash
nix profile install github:lavalleeale/gorg
```

Or run it without installing:

```bash
nix run github:lavalleeale/gorg
```

### Compiling from Source

#### Dependencies

- gtkmm-3.0
- libcurl
- pkg-config
- g++ with C++17 support

#### Build Steps

1. Clone the repository:

   ```bash
   git clone https://github.com/lavalleeale/gorg.git
   cd gorg
   ```

2. Compile the project:

   ```bash
   make
   ```

3. Run the application:
   ```bash
   ./gorg.out
   ```

## Usage

Launch Gorg with:

```bash
gorg
```

### Basic Commands

- **Application Search**: Just start typing the name of an application
- **Calculations**: Type a mathematical expression
- **AI Questions**: Type a question and select the AI assistant option

Press `Escape` to close Gorg at any time.

## Plugins

Gorg's functionality is extended through a plugin system:

- **Drun**: Searches for executables in PATH
- **Run**: Searches for desktop applications
- **Equation**: Evaluates mathematical expressions using exprtk
- **Chat**: Provides AI assistance using local LLM models

## Development

### Adding New Plugins

Create a new plugin by implementing the

Plugin

class interface:

```cpp
class MyPlugin : public Plugin {
public:
    float matchChance(std::string input);
    std::vector<Match *> getMatches(std::string input);
    ~MyPlugin() {};
};
```

### Building with Debug Information

```bash
make debug
```

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.
