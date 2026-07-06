# Aerostrike-CLI

A fast-paced, console-based arcade shooting game written entirely in C++. **AeroStrike-CLI** bypasses standard standard input/output buffering by utilizing POSIX operating system API headers to enable raw terminal mode, allowing for real-time, non-blocking keystroke processing. 

The objective is simple: pilot your aircraft, defend against endless waves of advancing enemies, and survive as long as possible to achieve the highest score.

---

## Gameplay Overview

The screen is redrawn every frame to simulate real-time motion and combat. The game features an infinite, dynamically scaling difficulty loop. As you clear waves of enemies, the game scales up the challenge by increasing the enemy count and their movement speed. 

### Game Elements
* **`A` - Player Aircraft:** Controlled by the user, positioned at the bottom row of the screen.
* **`v` - Enemy Units:** Spawn in a grid formation at the top of the screen.
* **`|` - Player Bullets:** Fired vertically to eliminate enemies.
* **`o` - Enemy Bombs:** Deadly projectiles dropped by enemies once the difficulty ramps up.

### Scaling Difficulty & Enemy Behavior
* **Grid Movement:** Enemies move horizontally in unison. Upon reaching the edge of the terminal, they reverse direction and descend one step.
* **Wave Progression:** The initial wave spawns a 2x5 grid of enemies. When a wave is entirely eliminated, the grid expands (up to a maximum size) and the movement speed increases.
* **Bomb Activation (100 Points):** The game introduces a new threat once the player reaches a score of 100. Enemies will dynamically begin dropping bombs randomly, requiring constant movement to survive.

---

## Technical Implementation

This project demonstrates core computer science and software engineering concepts:
* **Non-Blocking I/O:** Custom implementation of raw terminal modes using `<termios.h>` and `<fcntl.h>` to handle real-time keystrokes without waiting for the `Enter` key.
* **Game Loop Architecture:** A continuous `while` loop that handles state updates, input processing, and rendering simultaneously. 
* **State Management:** Utilizes efficient 2D boolean arrays (`enemy[MAX_ENEMY_ROWS][MAX_ENEMY_COLS]`) to track the grid status, and frame counters to manage independent movement speeds of the player, bullets, and enemies within a single loop cycle.

---

## Installation & Execution

### Prerequisites
* A C++ compiler (e.g., `g++` or `clang++`)
* A POSIX-compliant operating system (Linux or macOS). *Note: Terminal manipulation headers used in this game are not natively supported on standard Windows CMD.*

### Compilation
Clone the repository and compile the source code using the terminal:

```bash
# Compile the C++ file
g++ game.cpp -o game

# Run the executable
./game
