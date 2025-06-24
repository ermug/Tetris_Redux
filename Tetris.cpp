#include "Tetris.h"
#include <iostream>
#include <cmath>

/**
 * Constructor - Initialize the game with default values and setup
 */
Tetris::Tetris() :
    board(BOARD_HEIGHT, std::vector<int>(BOARD_WIDTH, 0)),  // Initialize empty board
    window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Tetris"),
    rng(std::chrono::steady_clock::now().time_since_epoch().count()),  // Seed RNG with current time
    pieceDist(0, 6),  // Distribution for 7 different piece types (0-6)
    score(0),
    level(1),
    linesCleared(0),
    gameOver(false),
    dropTimer(0),
    dropInterval(500.0f),  // Initial drop interval of 500ms
    soundEnabled(true)     // Enable sound by default
{
    // Set frame rate limit for smooth gameplay
    window.setFramerateLimit(60);

    // Initialize all Tetris piece shapes
    // Each piece is defined in a 4x4 grid with numbers representing colors
    pieces = {
        // I-piece (cyan) - straight line piece
        {{0,0,0,0},
         {1,1,1,1},
         {0,0,0,0},
         {0,0,0,0}},

         // O-piece (yellow) - square piece
         {{0,0,0,0},
          {0,2,2,0},
          {0,2,2,0},
          {0,0,0,0}},

          // T-piece (purple) - T-shaped piece
          {{0,0,0,0},
           {0,3,0,0},
           {3,3,3,0},
           {0,0,0,0}},

           // S-piece (green) - S-shaped piece
           {{0,0,0,0},
            {0,4,4,0},
            {4,4,0,0},
            {0,0,0,0}},

            // Z-piece (red) - Z-shaped piece
            {{0,0,0,0},
             {5,5,0,0},
             {0,5,5,0},
             {0,0,0,0}},

             // J-piece (blue) - J-shaped piece
             {{0,0,0,0},
              {6,0,0,0},
              {6,6,6,0},
              {0,0,0,0}},

              // L-piece (orange) - L-shaped piece
              {{0,0,0,0},
               {0,0,7,0},
               {7,7,7,0},
               {0,0,0,0}}
    };

    // Color mapping for each piece type (index 0 is empty/black)
    colors = {
        sf::Color::Black,        // 0 - empty space
        sf::Color::Cyan,         // 1 - I piece
        sf::Color::Yellow,       // 2 - O piece
        sf::Color::Magenta,      // 3 - T piece
        sf::Color::Green,        // 4 - S piece
        sf::Color::Red,          // 5 - Z piece
        sf::Color::Blue,         // 6 - J piece
        sf::Color(255,165,0)     // 7 - L piece (orange)
    };

    // Try to load font from multiple possible locations
    bool fontLoaded = false;
    std::vector<std::string> fontPaths = {
        "arial.ttf",                    // Current directory
        "fonts/arial.ttf",              // fonts subdirectory
        "assets/fonts/arial.ttf",       // assets/fonts subdirectory
        "/System/Library/Fonts/Arial.ttf",           // macOS system font
        "/Windows/Fonts/arial.ttf",                  // Windows system font
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",  // Linux common font
        "/usr/share/fonts/TTF/arial.ttf"            // Linux alternative
    };

    for (const auto& path : fontPaths) {
        if (font.loadFromFile(path)) {
            std::cout << "Successfully loaded font: " << path << std::endl;
            fontLoaded = true;
            break;
        }
    }

    if (!fontLoaded) {
        std::cout << "Warning: Could not load any font file. Text may not display correctly." << std::endl;
        std::cout << "To fix this, place arial.ttf in the game directory or fonts/ subdirectory." << std::endl;
    }

    // Initialize audio system
    loadSounds();

    // Initialize UI elements and spawn first piece
    setupText();
    spawnNewPiece();
}

/**
 * Load all sound effect files
 */
void Tetris::loadSounds() {
    // Try to load sound files from multiple locations
    std::vector<std::string> soundPaths = {
        "",                     // Current directory
        "sounds/",              // sounds subdirectory
        "assets/sounds/",       // assets/sounds subdirectory
        "audio/"                // audio subdirectory
    };

    std::vector<std::string> soundFiles = {
        "move.wav", "rotate.wav", "drop.wav", "line_clear.wav", "game_over.wav", "level_up.wav"
    };

    bool anyLoaded = false;

    // Try to load each sound file
    for (const auto& basePath : soundPaths) {
        if (moveBuffer.loadFromFile(basePath + "move.wav")) {
            moveSound.setBuffer(moveBuffer);
            anyLoaded = true;
            std::cout << "Loaded move sound from: " << basePath << "move.wav" << std::endl;
            break;
        }
    }

    for (const auto& basePath : soundPaths) {
        if (rotateBuffer.loadFromFile(basePath + "rotate.wav")) {
            rotateSound.setBuffer(rotateBuffer);
            anyLoaded = true;
            std::cout << "Loaded rotate sound from: " << basePath << "rotate.wav" << std::endl;
            break;
        }
    }

    for (const auto& basePath : soundPaths) {
        if (dropBuffer.loadFromFile(basePath + "drop.wav")) {
            dropSound.setBuffer(dropBuffer);
            anyLoaded = true;
            std::cout << "Loaded drop sound from: " << basePath << "drop.wav" << std::endl;
            break;
        }
    }

    for (const auto& basePath : soundPaths) {
        if (lineClearBuffer.loadFromFile(basePath + "line_clear.wav")) {
            lineClearSound.setBuffer(lineClearBuffer);
            anyLoaded = true;
            std::cout << "Loaded line clear sound from: " << basePath << "line_clear.wav" << std::endl;
            break;
        }
    }

    for (const auto& basePath : soundPaths) {
        if (gameOverBuffer.loadFromFile(basePath + "game_over.wav")) {
            gameOverSound.setBuffer(gameOverBuffer);
            anyLoaded = true;
            std::cout << "Loaded game over sound from: " << basePath << "game_over.wav" << std::endl;
            break;
        }
    }

    for (const auto& basePath : soundPaths) {
        if (levelUpBuffer.loadFromFile(basePath + "level_up.wav")) {
            levelUpSound.setBuffer(levelUpBuffer);
            anyLoaded = true;
            std::cout << "Loaded level up sound from: " << basePath << "level_up.wav" << std::endl;
            break;
        }
    }

    if (!anyLoaded) {
        std::cout << "No sound files found. Generating simple sound effects..." << std::endl;
        generateSounds();
    }
}

/**
 * Generate simple sound effects programmatically
 */
void Tetris::generateSounds() {
    const unsigned SAMPLE_RATE = 44100;
    const unsigned SAMPLES = SAMPLE_RATE / 4; // 0.25 second sounds

    // Generate move sound (short beep)
    std::vector<sf::Int16> moveSamples(SAMPLES);
    for (unsigned i = 0; i < SAMPLES; i++) {
        float time = static_cast<float>(i) / SAMPLE_RATE;
        float amplitude = 3000.0f * std::exp(-time * 10.0f); // Decaying amplitude
        moveSamples[i] = static_cast<sf::Int16>(amplitude * std::sin(2 * 3.1415 * 800 * time));
    }
    moveBuffer.loadFromSamples(moveSamples.data(), moveSamples.size(), 1, SAMPLE_RATE);
    moveSound.setBuffer(moveBuffer);

    // Generate rotate sound (higher pitch beep)
    std::vector<sf::Int16> rotateSamples(SAMPLES);
    for (unsigned i = 0; i < SAMPLES; i++) {
        float time = static_cast<float>(i) / SAMPLE_RATE;
        float amplitude = 3000.0f * std::exp(-time * 8.0f);
        rotateSamples[i] = static_cast<sf::Int16>(amplitude * std::sin(2 * 3.1415 * 1200 * time));
    }
    rotateBuffer.loadFromSamples(rotateSamples.data(), rotateSamples.size(), 1, SAMPLE_RATE);
    rotateSound.setBuffer(rotateBuffer);

    // Generate drop sound (lower thud)
    std::vector<sf::Int16> dropSamples(SAMPLES);
    for (unsigned i = 0; i < SAMPLES; i++) {
        float time = static_cast<float>(i) / SAMPLE_RATE;
        float amplitude = 5000.0f * std::exp(-time * 15.0f);
        dropSamples[i] = static_cast<sf::Int16>(amplitude * std::sin(2 * 3.1415 * 200 * time));
    }
    dropBuffer.loadFromSamples(dropSamples.data(), dropSamples.size(), 1, SAMPLE_RATE);
    dropSound.setBuffer(dropBuffer);

    // Generate line clear sound (pleasant chime)
    const unsigned LINE_SAMPLES = SAMPLE_RATE / 2; // 0.5 second
    std::vector<sf::Int16> lineSamples(LINE_SAMPLES);
    for (unsigned i = 0; i < LINE_SAMPLES; i++) {
        float time = static_cast<float>(i) / SAMPLE_RATE;
        float amplitude = 4000.0f * std::exp(-time * 3.0f);
        float freq1 = 523.25f; // C5
        float freq2 = 659.25f; // E5
        float freq3 = 783.99f; // G5
        lineSamples[i] = static_cast<sf::Int16>(amplitude * (
            std::sin(2 * 3.1415 * freq1 * time) +
            std::sin(2 * 3.1415 * freq2 * time) +
            std::sin(2 * 3.1415 * freq3 * time)
            ) / 3.0f);
    }
    lineClearBuffer.loadFromSamples(lineSamples.data(), lineSamples.size(), 1, SAMPLE_RATE);
    lineClearSound.setBuffer(lineClearBuffer);

    // Generate game over sound (descending tones)
    const unsigned GAME_OVER_SAMPLES = SAMPLE_RATE; // 1 second
    std::vector<sf::Int16> gameOverSamples(GAME_OVER_SAMPLES);
    for (unsigned i = 0; i < GAME_OVER_SAMPLES; i++) {
        float time = static_cast<float>(i) / SAMPLE_RATE;
        float freq = 440.0f * std::exp(-time * 2.0f); // Descending frequency
        float amplitude = 4000.0f * std::exp(-time * 2.0f);
        gameOverSamples[i] = static_cast<sf::Int16>(amplitude * std::sin(2 * 3.1415 * freq * time));
    }
    gameOverBuffer.loadFromSamples(gameOverSamples.data(), gameOverSamples.size(), 1, SAMPLE_RATE);
    gameOverSound.setBuffer(gameOverBuffer);

    // Generate level up sound (ascending chime)
    std::vector<sf::Int16> levelUpSamples(LINE_SAMPLES);
    for (unsigned i = 0; i < LINE_SAMPLES; i++) {
        float time = static_cast<float>(i) / SAMPLE_RATE;
        float amplitude = 4000.0f * std::exp(-time * 2.0f);
        float freq = 440.0f + time * 880.0f; // Ascending frequency
        levelUpSamples[i] = static_cast<sf::Int16>(amplitude * std::sin(2 * 3.1415 * freq * time));
    }
    levelUpBuffer.loadFromSamples(levelUpSamples.data(), levelUpSamples.size(), 1, SAMPLE_RATE);
    levelUpSound.setBuffer(levelUpBuffer);

    std::cout << "Generated synthetic sound effects successfully." << std::endl;
}

/**
 * Play a sound effect if audio is enabled
 */
void Tetris::playSound(sf::Sound& sound) {
    if (soundEnabled) {
        sound.play();
    }
}

/**
 * Setup all text elements for the user interface
 */
void Tetris::setupText() {
    // Configure score display
    scoreText.setFont(font);
    scoreText.setCharacterSize(20);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(BOARD_WIDTH * BLOCK_SIZE + 10, 10);  // Position to right of game board

    // Configure level display
    levelText.setFont(font);
    levelText.setCharacterSize(20);
    levelText.setFillColor(sf::Color::White);
    levelText.setPosition(BOARD_WIDTH * BLOCK_SIZE + 10, 40);  // Below score

    // Configure game over message
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(30);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setString("GAME OVER");
    gameOverText.setPosition(50, WINDOW_HEIGHT / 2);  // Center of screen
}

/**
 * Generate and spawn a new random piece at the top of the board
 */
void Tetris::spawnNewPiece() {
    // Select random piece type (0-6)
    currentPieceType = pieceDist(rng);
    currentPiece = pieces[currentPieceType];

    // Position piece at top-center of board
    currentX = BOARD_WIDTH / 2 - 2;  // Center horizontally (accounting for 4-wide piece grid)
    currentY = 0;                    // Start at top

    // Check if spawn position is blocked (game over condition)
    if (!isValidPosition(currentX, currentY, currentPiece)) {
        gameOver = true;
        playSound(gameOverSound);
    }
}

/**
 * Check if a piece can be placed at the specified position without collisions
 */
bool Tetris::isValidPosition(int x, int y, const std::vector<std::vector<int>>& piece) {
    // Check each block of the piece
    for (int py = 0; py < 4; py++) {
        for (int px = 0; px < 4; px++) {
            // Only check non-empty blocks of the piece
            if (piece[py][px] != 0) {
                int boardX = x + px;
                int boardY = y + py;

                // Check boundaries and collisions
                if (boardX < 0 || boardX >= BOARD_WIDTH ||          // Left/right boundaries
                    boardY >= BOARD_HEIGHT ||                       // Bottom boundary
                    (boardY >= 0 && board[boardY][boardX] != 0)) {  // Collision with existing blocks
                    return false;
                }
            }
        }
    }
    return true;
}

/**
 * Permanently place the current piece on the game board
 */
void Tetris::placePiece() {
    // Transfer each block of the current piece to the board
    for (int py = 0; py < 4; py++) {
        for (int px = 0; px < 4; px++) {
            if (currentPiece[py][px] != 0) {
                int boardX = currentX + px;
                int boardY = currentY + py;

                // Only place blocks that are within the visible board area
                if (boardY >= 0) {
                    board[boardY][boardX] = currentPiece[py][px];
                }
            }
        }
    }

    // Play drop sound when piece is placed
    playSound(dropSound);
}

/**
 * Check for complete lines and clear them, updating score and level
 */
void Tetris::clearLines() {
    int clearedCount = 0;
    int previousLevel = level;

    // Scan from bottom to top for complete lines
    for (int y = BOARD_HEIGHT - 1; y >= 0; y--) {
        bool fullLine = true;

        // Check if this line is completely filled
        for (int x = 0; x < BOARD_WIDTH; x++) {
            if (board[y][x] == 0) {
                fullLine = false;
                break;
            }
        }

        if (fullLine) {
            // Remove the complete line and add empty line at top
            board.erase(board.begin() + y);
            board.insert(board.begin(), std::vector<int>(BOARD_WIDTH, 0));
            clearedCount++;
            y++; // Re-check the same line index since we shifted everything down
        }
    }

    // Update game state if any lines were cleared
    if (clearedCount > 0) {
        linesCleared += clearedCount;

        // Score calculation: more lines cleared simultaneously = higher score multiplier
        score += clearedCount * 100 * level;

        // Level increases every 10 lines cleared
        level = 1 + linesCleared / 10;

        // Increase drop speed with level (minimum 50ms interval)
        dropInterval = std::max(50.0f, 500.0f - (level - 1) * 50.0f);

        // Play appropriate sound effects
        playSound(lineClearSound);

        // Play level up sound if level increased
        if (level > previousLevel) {
            // Small delay before level up sound
            sf::sleep(sf::milliseconds(200));
            playSound(levelUpSound);
        }
    }
}

/**
 * Rotate a piece 90 degrees clockwise using matrix rotation
 */
std::vector<std::vector<int>> Tetris::rotatePiece(const std::vector<std::vector<int>>& piece) {
    std::vector<std::vector<int>> rotated(4, std::vector<int>(4, 0));

    // Apply rotation transformation: (x,y) -> (y, 3-x)
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            rotated[x][3 - y] = piece[y][x];
        }
    }

    return rotated;
}

/**
 * Handle all user input and window events
 */
void Tetris::handleInput() {
    sf::Event event;

    while (window.pollEvent(event)) {
        // Handle window close event
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        // Handle gameplay input (only when game is active)
        if (event.type == sf::Event::KeyPressed && !gameOver) {
            switch (event.key.code) {
            case sf::Keyboard::Left:
                // Move piece left if possible
                if (isValidPosition(currentX - 1, currentY, currentPiece)) {
                    currentX--;
                    playSound(moveSound);
                }
                break;

            case sf::Keyboard::Right:
                // Move piece right if possible
                if (isValidPosition(currentX + 1, currentY, currentPiece)) {
                    currentX++;
                    playSound(moveSound);
                }
                break;

            case sf::Keyboard::Down:
                // Soft drop - move piece down faster for small score bonus
                if (isValidPosition(currentX, currentY + 1, currentPiece)) {
                    currentY++;
                    score += 1;  // Small bonus for manual dropping
                    playSound(moveSound);
                }
                break;

            case sf::Keyboard::Up:
                // Rotate piece clockwise if possible
            {
                auto rotated = rotatePiece(currentPiece);
                if (isValidPosition(currentX, currentY, rotated)) {
                    currentPiece = rotated;
                    playSound(rotateSound);
                }
            }
            break;

            case sf::Keyboard::Space:
                // Hard drop - instantly drop piece to bottom
                while (isValidPosition(currentX, currentY + 1, currentPiece)) {
                    currentY++;
                    score += 2;  // Higher bonus for hard drop
                }
                playSound(dropSound);
                break;

            case sf::Keyboard::M:
                // Toggle sound on/off
                soundEnabled = !soundEnabled;
                std::cout << "Sound " << (soundEnabled ? "enabled" : "disabled") << std::endl;
                break;
            }
        }

        // Handle game over input
        if (event.type == sf::Event::KeyPressed && gameOver) {
            if (event.key.code == sf::Keyboard::R) {
                // Restart game - reset all game state
                board = std::vector<std::vector<int>>(BOARD_HEIGHT, std::vector<int>(BOARD_WIDTH, 0));
                score = 0;
                level = 1;
                linesCleared = 0;
                gameOver = false;
                dropInterval = 500.0f;
                spawnNewPiece();
            }
        }
    }
}

/**
 * Update game state each frame
 */
void Tetris::update() {
    // Don't update game logic if game is over
    if (gameOver) return;

    // Handle automatic piece dropping based on timing
    float deltaTime = clock.restart().asMilliseconds();
    dropTimer += deltaTime;

    // Check if it's time to drop the piece automatically
    if (dropTimer >= dropInterval) {
        if (isValidPosition(currentX, currentY + 1, currentPiece)) {
            // Piece can fall further
            currentY++;
        }
        else {
            // Piece has landed - place it, clear lines, and spawn new piece
            placePiece();
            clearLines();
            spawnNewPiece();
        }
        dropTimer = 0;  // Reset drop timer
    }
}

/**
 * Render all game graphics to the screen
 */
void Tetris::render() {
    // Clear screen with black background
    window.clear(sf::Color::Black);

    // Create a rectangle shape for drawing blocks
    sf::RectangleShape block(sf::Vector2f(BLOCK_SIZE - 1, BLOCK_SIZE - 1));  // -1 for grid lines

    // Draw all placed pieces on the board
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        for (int x = 0; x < BOARD_WIDTH; x++) {
            if (board[y][x] != 0) {
                block.setFillColor(colors[board[y][x]]);
                block.setPosition(x * BLOCK_SIZE, y * BLOCK_SIZE);
                window.draw(block);
            }
        }
    }

    // Draw the currently falling piece (if game is active)
    if (!gameOver) {
        for (int py = 0; py < 4; py++) {
            for (int px = 0; px < 4; px++) {
                if (currentPiece[py][px] != 0) {
                    block.setFillColor(colors[currentPiece[py][px]]);
                    block.setPosition((currentX + px) * BLOCK_SIZE, (currentY + py) * BLOCK_SIZE);
                    window.draw(block);
                }
            }
        }
    }

    // Draw game board border
    sf::RectangleShape border;
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineColor(sf::Color::White);
    border.setOutlineThickness(2);
    border.setSize(sf::Vector2f(BOARD_WIDTH * BLOCK_SIZE, BOARD_HEIGHT * BLOCK_SIZE));
    border.setPosition(0, 0);
    window.draw(border);

    // Update and draw UI text
    scoreText.setString("Score: " + std::to_string(score));
    levelText.setString("Level: " + std::to_string(level));
    window.draw(scoreText);
    window.draw(levelText);

    // Draw sound status
    sf::Text soundStatusText;
    soundStatusText.setFont(font);
    soundStatusText.setCharacterSize(16);
    soundStatusText.setFillColor(soundEnabled ? sf::Color::Green : sf::Color::Red);
    soundStatusText.setString("Sound: " + std::string(soundEnabled ? "ON" : "OFF"));
    soundStatusText.setPosition(BOARD_WIDTH * BLOCK_SIZE + 10, 70);
    window.draw(soundStatusText);

    // Draw controls help
    sf::Text controlsText;
    controlsText.setFont(font);
    controlsText.setCharacterSize(14);
    controlsText.setFillColor(sf::Color::White);
    controlsText.setString("Controls:\nArrows: Move\nUp: Rotate\nSpace: Hard Drop\nM: Toggle Sound");
    controlsText.setPosition(BOARD_WIDTH * BLOCK_SIZE + 10, 100);
    window.draw(controlsText);

    // Draw game over screen if applicable
    if (gameOver) {
        window.draw(gameOverText);

        // Draw restart instruction
        sf::Text restartText;
        restartText.setFont(font);
        restartText.setCharacterSize(20);
        restartText.setFillColor(sf::Color::White);
        restartText.setString("Press R to restart");
        restartText.setPosition(50, WINDOW_HEIGHT / 2 + 40);
        window.draw(restartText);
    }

    // Display everything on screen
    window.display();
}

/**
 * Main game loop - runs until window is closed
 */
void Tetris::run() {
    while (window.isOpen()) {
        handleInput();  // Process user input
        update();       // Update game state
        render();       // Draw everything
    }
}