# Galaga - Game Boy Advance

A fully functional arcade-style space shooter game developed in C for the Game Boy Advance (GBA) platform. This project demonstrates low-level hardware programming, game state management, and real-time graphics rendering on embedded systems.

## Demo

[![Galaga Gameplay Demo](https://img.shields.io/badge/Watch%20Demo-Loom-5c2d91)](https://www.loom.com/share/4d28218138ed4c1bb9d532ed59ab9678)

## Features

- **Player-Controlled Rocket**: Smooth movement in all directions with pixel-perfect collision detection
- **Enemy Combat System**: Procedurally spawned enemies with intelligent movement patterns
- **Shooting Mechanics**: Real-time projectile generation and collision detection
- **Score Tracking**: High score persistence across game sessions
- **Lives System**: Multiple lives with visual feedback
- **Multi-State Architecture**: Robust state machine handling game flow (Start, Create, Play, Win, Lose)
- **Professional UI**: Custom text rendering, title screen, game over screens
- **Performance Optimized**: DMA-accelerated rendering for smooth 60 FPS gameplay

## Technical Architecture

### Core Components
- **game.c/game.h**: Main game loop and state machine logic
- **gba.c/gba.h**: Low-level GBA hardware abstraction layer (memory-mapped I/O, video modes, input handling)
- **Graphics System**: Hardware-optimized sprite rendering using DMA transfers
- **Text Rendering**: Custom font engine for on-screen UI elements

### Key Implementation Details
- **Video Mode 3**: 16-bit color bitmap rendering (240×160 resolution)
- **Input Handling**: Button state tracking with debouncing
- **Memory Management**: Static allocation for fixed game entities (5 enemies, player, missiles)
- **Sprite System**: Struct-based entity management for rockets, missiles, and enemies

```c
typedef struct rocket {
    int row, col;
    int width, height;
    int cd, rd;  // collision/render dimensions
} ROCKET;
```

## Controls

| Action | Button | Keyboard (Emulator) |
|--------|--------|-------------------|
| Start Game | A | Z |
| Move | D-Pad | Arrow Keys |
| Shoot | B | X |
| Menu | SELECT | Backspace |

## Gameplay

1. **Objective**: Destroy all 5 enemies to advance
2. **Scoring**: +20 points per enemy defeated
3. **Lives**: Start with 3 lives; game ends when depleted
4. **Win Condition**: Clear all enemies to see victory screen
5. **Lose Condition**: Run out of lives triggers game over screen

## Build & Run

### Requirements
- GBA toolchain (arm-none-eabi-gcc)
- Mednafen GBA emulator
- POSIX-compliant system (Linux/macOS)

### Compilation
```bash
cd src
make              # Optimized build
make debug        # Debug build with symbols
```

### Execution
```bash
make emu          # Launch in Mednafen emulator
```

## Project Structure

```
src/
├── game.c/h           # Main game logic and state machine
├── gba.c/h            # Hardware abstraction layer
├── text.c/h           # Text rendering engine
├── font.c/h           # Font bitmap data
├── Makefile           # Build configuration
├── mednafen-09x.cfg   # Emulator settings
└── images/            # Pre-compiled sprite assets
    ├── rocket.c/h
    ├── enemy.c/h
    ├── missile*.c/h
    └── ...
```

## What I Learned

This project taught me valuable skills in:
- **Embedded Systems Programming**: Working directly with hardware constraints and memory-mapped I/O
- **Real-Time Systems**: Managing frame-locked game loops with VBlank synchronization
- **Game Development**: State machines, collision detection, sprite management
- **Performance Optimization**: DMA transfers, efficient memory access patterns
- **Low-Level C**: Manual memory management, hardware registers, bitwise operations

## Course Context

Created as a final project for **CS 2110: Computer Organization and Programming** at Georgia Tech. Demonstrates proficiency in systems-level C programming and practical application of computer architecture concepts.

---

**Author**: Henna Mian  
**Language**: C  
**Platform**: Game Boy Advance (GBA)  
**Build System**: Make + arm-none-eabi toolchain

