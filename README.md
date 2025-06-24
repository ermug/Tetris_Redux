# 🎮 Tetris Game

A complete, feature-rich implementation of the classic Tetris game built with C++ and SFML. Experience smooth gameplay, vibrant graphics, and all the nostalgic fun of the original Tetris!

![Tetris Screenshot](https://img.shields.io/badge/Game-Tetris-blue?style=for-the-badge&logo=game&logoColor=white)
![C++](https://img.shields.io/badge/C%2B%2B-17-blue?style=for-the-badge&logo=cplusplus)
![SFML](https://img.shields.io/badge/SFML-2.5+-green?style=for-the-badge)

## 🚀 Quick Start

### 💾 Download & Play (Recommended)

**Want to play immediately? No setup required!**

1. **📦 [Download Latest Release](../../releases/latest)**
2. **📂 Extract the zip file**
3. **🎮 Run `tetris.exe` and enjoy!**

> ✨ **Windows users**: The release package includes everything you need - no SFML installation or font setup required!

### 🛠️ Or Build From Source

If you want to modify the game or build for other platforms, see the [Installation](#-installation) section below.

## ✨ Features

- 🎯 **Complete Tetris Experience** - All 7 standard Tetrimino pieces (I, O, T, S, Z, J, L)
- 🎨 **Colorful Graphics** - Each piece type has its distinctive color
- ⚡ **Smooth Controls** - Responsive keyboard input with no lag
- 🔄 **Piece Rotation** - Full 90-degree clockwise rotation with wall kicks
- 💥 **Line Clearing** - Satisfying line clear animations and scoring
- 📈 **Progressive Difficulty** - Game speed increases with level progression
- 🏆 **Scoring System** - Points for drops, line clears, and level bonuses
- 🎮 **Game Over & Restart** - Clean game over detection with instant restart
- 🎵 **Clean UI** - Score, level, and status display

## 🎮 Controls

| Key | Action | Points |
|-----|--------|--------|
| **←** / **→** | Move piece left/right | - |
| **↓** | Soft drop (faster fall) | +1 per drop |
| **↑** | Rotate piece clockwise | - |
| **Space** | Hard drop (instant drop) | +2 per row |
| **R** | Restart game (when game over) | - |
| **ESC** | Close game | - |

## 🏗️ Project Structure

```
tetris/
├── 📄 Tetris.h              # Class declaration and interface
├── 🔧 Tetris.cpp            # Core game implementation
├── 🚀 main.cpp              # Application entry point
├── ⚙️ Makefile              # Build automation
├── 📖 README.md             # This documentation
├── 📁 fonts/                # Font files directory (Optional, not included, must create manually)
│   └── arial.ttf            # Place your font file here
```

## 📥 Installation

> 💡 **Just want to play?** Skip this section and [download the release](#-download--play-recommended) instead!

### Prerequisites for Building

- **C++ Compiler**: Supporting C++17 standard (GCC 7.0+ / Clang 5.0+ / MSVC 2017+)
- **SFML Library**: Version 2.5 or newer
- **Font File**: Any TrueType font (.ttf) - see [Font Setup](#-font-setup)

### 🐧 Ubuntu/Debian
```bash
# Install dependencies
sudo apt-get update
sudo apt-get install build-essential libsfml-dev

# Clone and build
git clone <your-repo-url>
cd tetris
make
```

### 🍎 macOS
```bash
# Install dependencies (requires Homebrew)
brew install sfml

# Clone and build
git clone <your-repo-url>
cd tetris
make
```

### 🪟 Windows

> **🎯 Prefer the easy way?** [Download the pre-built release](#-download--play-recommended) - no setup needed!

#### Option 1: Visual Studio
1. Download SFML from [sfml-dev.org](https://www.sfml-dev.org/download.php)
2. Extract SFML to `C:\SFML`
3. Create new C++ project in Visual Studio
4. Add source files to project
5. Configure project properties:
   - **C/C++ → General → Additional Include Directories**: `C:\SFML\include`
   - **Linker → General → Additional Library Directories**: `C:\SFML\lib`
   - **Linker → Input → Additional Dependencies**: Add:
     ```
     sfml-graphics.lib
     sfml-window.lib
     sfml-system.lib
     ```

#### Option 2: MinGW/MSYS2
```bash
# Install SFML through MSYS2
pacman -S mingw-w64-x86_64-sfml

# Compile manually
g++ -std=c++17 main.cpp Tetris.cpp -o tetris.exe -lsfml-graphics -lsfml-window -lsfml-system
```

## 🔧 Building

### Using Makefile (Recommended for Linux/macOS)
```bash
# Build the game
make

# Build and run immediately
make run

# Build with debug symbols
make debug

# Clean build files
make clean

# Show all available commands
make help
```

### Manual Compilation
```bash
# Standard build
g++ -std=c++17 -Wall -Wextra -O2 main.cpp Tetris.cpp -o tetris -lsfml-graphics -lsfml-window -lsfml-system

# Debug build
g++ -std=c++17 -g -DDEBUG main.cpp Tetris.cpp -o tetris -lsfml-graphics -lsfml-window -lsfml-system

# Run
./tetris
```

## 🎨 Font Setup

> **📦 Using the release version?** Fonts are already included - skip this section!

The game requires a TrueType font for text display. If you see font loading warnings:

### Quick Setup (Choose One):

1. **Download Free Font**:
   - Visit [Google Fonts](https://fonts.google.com/)
   - Download "Roboto" or "Open Sans"
   - Rename the `.ttf` file to `arial.ttf`
   - Place in game directory

2. **Use System Font**:
   ```bash
   # Linux
   cp /usr/share/fonts/truetype/dejavu/DejaVuSans.ttf ./arial.ttf
   
   # macOS
   cp /System/Library/Fonts/Arial.ttf ./arial.ttf
   
   # Windows (Command Prompt)
   copy "C:\Windows\Fonts\arial.ttf" .\arial.ttf
   ```

3. **Create Fonts Directory**:
   ```bash
   mkdir fonts
   # Place any .ttf font in fonts/arial.ttf
   ```

### Automatic Detection
The game searches these locations automatically:
- `./arial.ttf`
- `./fonts/arial.ttf`
- `./assets/fonts/arial.ttf`
- System font directories

## 🎯 Game Rules & Scoring

### Objective
Clear horizontal lines by completely filling them with Tetrimino blocks.

### Scoring System
- **Soft Drop** (↓): +1 point per row
- **Hard Drop** (Space): +2 points per row
- **Line Clear**: 100 × Level × Lines cleared simultaneously
  - Single: 100 × Level
  - Double: 200 × Level  
  - Triple: 300 × Level
  - Tetris: 400 × Level

### Level Progression
- **Level increases** every 10 lines cleared
- **Speed increases** with each level
- **Drop interval** decreases: 500ms - (Level-1) × 50ms
- **Minimum speed**: 50ms between drops

### Game Over
- Occurs when new pieces cannot spawn (board is full)
- Press **R** to restart immediately
- All progress resets to Level 1

## 🚀 Running the Game

### From Release (Windows)
```bash
# After downloading and extracting
tetris.exe
```

### From Source Build
```bash
# After building
./tetris

# Or with make
make run
```

### First Launch Checklist:
- ✅ Download from [Releases](../../releases/latest) (easiest), or
- ✅ SFML libraries installed (if building from source)
- ✅ Font file available (arial.ttf) (if building from source)
- ✅ Executable has proper permissions
- ✅ Terminal/console for error messages

## 🐛 Troubleshooting

### For Release Users

**"Windows Defender / Antivirus blocks the file"**
- This is normal for unsigned executables
- Add the folder to your antivirus exceptions
- Or right-click → Properties → "Unblock" checkbox

**"MSVCP140.dll missing" or similar**
- Install [Microsoft Visual C++ Redistributable](https://aka.ms/vs/17/release/vc_redist.x64.exe)
- Both x86 and x64 versions may be needed

### For Source Build Users

**"Command not found" / "File not found"**
```bash
# Make executable (Linux/macOS)
chmod +x tetris

# Check if file exists
ls -la tetris
```

**"SFML libraries not found"**
```bash
# Check SFML installation
pkg-config --libs sfml-all

# Install SFML (Ubuntu)
sudo apt-get install libsfml-dev
```

**"Font loading warning"**
- Follow the [Font Setup](#-font-setup) section
- Game will work but text may be invisible without fonts

**Game runs but no graphics appear**
- Check graphics drivers
- Try running in compatibility mode
- Verify SFML installation

**Compilation errors**
```bash
# Check compiler version
g++ --version  # Should be 7.0+

# Verify C++17 support
g++ -std=c++17 --version
```

## 🎮 Gameplay Tips

- **Plan Ahead**: Look at upcoming pieces and plan placement
- **Create Tetrises**: Save the I-piece for 4-line clears when possible
- **Avoid Holes**: Don't trap empty spaces under blocks
- **Use Hard Drop**: Space bar for quick placement and extra points
- **Master Rotation**: Learn piece rotation patterns for tight spaces
- **Speed Practice**: Start slow and gradually increase difficulty

## 🔧 Customization

### Piece Colors
Edit the `colors` vector in `Tetris.cpp`:
```cpp
colors = {
    sf::Color::Black,           // 0 - Empty
    sf::Color::Cyan,            // 1 - I piece (straight)
    sf::Color::Yellow,          // 2 - O piece (square)
    sf::Color::Magenta,         // 3 - T piece
    sf::Color::Green,           // 4 - S piece
    sf::Color::Red,             // 5 - Z piece
    sf::Color::Blue,            // 6 - J piece
    sf::Color(255,165,0)        // 7 - L piece (orange)
};
```

### Game Difficulty
Modify timing in the constructor:
```cpp
dropInterval = 500.0f;  // Starting speed (milliseconds)
```

### Board Size
Change constants in `Tetris.h`:
```cpp
static const int BOARD_WIDTH = 10;   // Standard: 10
static const int BOARD_HEIGHT = 20;  // Standard: 20
```

## 🤝 Contributing

Contributions are welcome! Here are some ideas:

### Easy Additions
- [ ] Next piece preview
- [ ] Pause functionality
- [ ] High score saving
- [ ] Sound effects

### Medium Features
- [ ] Ghost piece (shadow)
- [ ] Hold piece function
- [ ] Particle effects for line clears
- [ ] Multiple game modes

### Advanced Features
- [ ] AI opponent
- [ ] Network multiplayer
- [ ] Custom piece sets
- [ ] Replay system

### Development Setup
```bash
# Fork the repository
git clone <your-fork>
cd tetris

# Create feature branch
git checkout -b feature/amazing-feature

# Make changes and test
make debug
./tetris

# Commit and push changes
git commit -m "Add amazing feature"
git push origin feature/amazing-feature
```

## 📝 Code Architecture

### Class Design
```cpp
class Tetris {
    // Game state management
    // Input handling
    // Collision detection
    // Rendering system
    // Scoring logic
};
```

### Key Methods
- **`spawnNewPiece()`**: Piece generation and placement
- **`isValidPosition()`**: Collision detection system
- **`rotatePiece()`**: Matrix rotation with boundary checking
- **`clearLines()`**: Line detection and removal logic
- **`handleInput()`**: Real-time input processing
- **`update()`**: Game state updates and timing
- **`render()`**: Graphics rendering pipeline

## 📦 Release Notes

### Latest Release Features
- ✅ **Added Sound Effects**: Now 50% Less Boring!
- ✅ **No Installation Required**: Just download and play!
- ✅ **All Dependencies Included**: SFML libraries bundled
- ✅ **Font Included**: No need to find arial.ttf
- ✅ **Windows Compatible**: Tested on Windows 10/11
- ✅ **Portable**: Run from any folder, no installation needed

*Check the [Releases page](../../releases) for version history and changelog.*

## 📄 License

This project is open source and available under the [MIT License](LICENSE).

## 🙏 Acknowledgments

- **SFML Team** - For the excellent multimedia library
- **Tetris Company** - For creating the timeless original game

---

**Happy Gaming! 🎮✨**

*Built with ❤️ using C++ and SFML*

**🎯 [Download Now](../../releases/latest) | 🛠️ Build From Source | 🐛 Report Issues**
