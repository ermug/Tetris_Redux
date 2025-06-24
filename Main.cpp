/**
 * @file main.cpp
 * @brief Entry point for the Tetris game
 *
 * This file contains the main function that creates and runs the Tetris game.
 * The game uses SFML for graphics and input handling.
 *
 * @author Your Name
 * @date 2025
 */

#include "Tetris.h"
#include <iostream>

 /**
  * @brief Main entry point for the Tetris game
  *
  * Creates a Tetris game instance and runs the main game loop.
  * Handles any exceptions that might occur during game execution.
  *
  * @return 0 on successful execution, non-zero on error
  */
int main() {
    try {
        // Create and run the Tetris game
        Tetris game;
        game.run();

        std::cout << "Game ended successfully." << std::endl;
        return 0;
    }
    catch (const std::exception& e) {
        // Handle any exceptions that occur during game execution
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    catch (...) {
        // Handle any other unknown exceptions
        std::cerr << "Unknown error occurred." << std::endl;
        return 1;
    }
}