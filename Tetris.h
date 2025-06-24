#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <random>
#include <chrono>

/**
 * @brief Main Tetris game class
 *
 * This class handles all game logic, rendering, input, and audio for a complete Tetris implementation.
 * It manages the game board, pieces, scoring, user interface, and sound effects.
 */
class Tetris {
private:
    // Game constants
    static const int BOARD_WIDTH = 10;          ///< Width of the game board in blocks
    static const int BOARD_HEIGHT = 20;         ///< Height of the game board in blocks
    static const int BLOCK_SIZE = 30;           ///< Size of each block in pixels
    static const int WINDOW_WIDTH = BOARD_WIDTH * BLOCK_SIZE + 200;   ///< Total window width
    static const int WINDOW_HEIGHT = BOARD_HEIGHT * BLOCK_SIZE + 100; ///< Total window height

    // Game board representation
    std::vector<std::vector<int>> board;        ///< 2D grid representing the game board (0 = empty, >0 = color index)

    // Tetromino piece definitions
    std::vector<std::vector<std::vector<int>>> pieces;  ///< All 7 Tetris piece shapes in 4x4 grids
    std::vector<sf::Color> colors;              ///< Color mapping for each piece type

    // Current active piece state
    std::vector<std::vector<int>> currentPiece; ///< Currently falling piece shape
    int currentX;                               ///< X position of current piece on board
    int currentY;                               ///< Y position of current piece on board
    int currentPieceType;                       ///< Type index of current piece (0-6)

    // Game state variables
    int score;                                  ///< Current player score
    int level;                                  ///< Current difficulty level
    int linesCleared;                          ///< Total lines cleared (used for level calculation)
    bool gameOver;                             ///< Flag indicating if game has ended

    // Timing control
    sf::Clock clock;                           ///< SFML clock for timing
    float dropTimer;                           ///< Timer for automatic piece dropping
    float dropInterval;                        ///< Interval between automatic drops (decreases with level)

    // Graphics and UI
    sf::RenderWindow window;                   ///< Main game window
    sf::Font font;                             ///< Font for text rendering
    sf::Text scoreText;                        ///< Score display text
    sf::Text levelText;                        ///< Level display text
    sf::Text gameOverText;                     ///< Game over message text

    // Audio system
    sf::SoundBuffer moveBuffer;                ///< Sound buffer for piece movement
    sf::SoundBuffer rotateBuffer;              ///< Sound buffer for piece rotation
    sf::SoundBuffer dropBuffer;                ///< Sound buffer for piece drop/landing
    sf::SoundBuffer lineClearBuffer;           ///< Sound buffer for line clearing
    sf::SoundBuffer gameOverBuffer;            ///< Sound buffer for game over
    sf::SoundBuffer levelUpBuffer;             ///< Sound buffer for level up

    sf::Sound moveSound;                       ///< Sound object for movement
    sf::Sound rotateSound;                     ///< Sound object for rotation
    sf::Sound dropSound;                       ///< Sound object for dropping
    sf::Sound lineClearSound;                  ///< Sound object for line clearing
    sf::Sound gameOverSound;                   ///< Sound object for game over
    sf::Sound levelUpSound;                    ///< Sound object for level up

    bool soundEnabled;                         ///< Flag to enable/disable sound effects

    // Random number generation
    std::mt19937 rng;                          ///< Random number generator
    std::uniform_int_distribution<int> pieceDist; ///< Distribution for selecting random pieces

public:
    /**
     * @brief Constructor - initializes the game
     */
    Tetris();

    /**
     * @brief Main game loop
     *
     * Runs the complete game loop handling input, updates, and rendering
     * until the window is closed.
     */
    void run();

private:
    /**
     * @brief Load all sound effect files
     *
     * Attempts to load sound files from multiple possible locations.
     * Sets soundEnabled flag based on success of loading sounds.
     */
    void loadSounds();

    /**
     * @brief Generate simple sound effects programmatically
     *
     * Creates basic sound effects if audio files cannot be loaded.
     * This ensures the game has some audio feedback even without external files.
     */
    void generateSounds();

    /**
     * @brief Play a sound effect if audio is enabled
     *
     * @param sound The sound object to play
     *
     * Helper function to play sounds only when audio is enabled.
     */
    void playSound(sf::Sound& sound);

    /**
     * @brief Initialize all UI text elements
     *
     * Sets up fonts, colors, positions, and sizes for all text displays
     * including score, level, and game over messages.
     */
    void setupText();

    /**
     * @brief Spawn a new random piece at the top of the board
     *
     * Selects a random piece type, positions it at the spawn location,
     * and checks for game over condition if the spawn position is blocked.
     */
    void spawnNewPiece();

    /**
     * @brief Check if a piece can be placed at a specific position
     *
     * @param x X coordinate to check
     * @param y Y coordinate to check
     * @param piece The piece shape to check
     * @return true if position is valid (no collisions), false otherwise
     *
     * Validates that the piece doesn't collide with board boundaries or existing blocks.
     */
    bool isValidPosition(int x, int y, const std::vector<std::vector<int>>& piece);

    /**
     * @brief Place the current piece permanently on the board
     *
     * Transfers the current piece from its temporary state to permanent
     * positions on the game board grid.
     */
    void placePiece();

    /**
     * @brief Check for and clear any complete horizontal lines
     *
     * Scans the board for completely filled rows, removes them,
     * adds new empty rows at the top, and updates score and level.
     * Also plays appropriate sound effects for line clearing and level up.
     */
    void clearLines();

    /**
     * @brief Rotate a piece 90 degrees clockwise
     *
     * @param piece The piece to rotate
     * @return The rotated piece as a new 4x4 grid
     *
     * Uses matrix rotation formula to transform the piece shape.
     */
    std::vector<std::vector<int>> rotatePiece(const std::vector<std::vector<int>>& piece);

    /**
     * @brief Handle all keyboard input and window events
     *
     * Processes player input for piece movement, rotation, dropping,
     * game restart, and sound toggle. Also handles window close events.
     */
    void handleInput();

    /**
     * @brief Update game state each frame
     *
     * Handles automatic piece dropping based on timing, piece placement
     * when pieces can't fall further, line clearing, and spawning new pieces.
     */
    void update();

    /**
     * @brief Render all game graphics to the screen
     *
     * Draws the game board, current piece, UI elements, and game over screen.
     * Handles all visual aspects of the game display.
     */
    void render();
};