# Development Notes

## Overview
This document outlines the design decisions, challenges encountered, and optimization strategies used in developing Galaga for the Game Boy Advance.

## Architecture Decisions

### State Machine Design
The game uses a finite state machine (FSM) to manage game flow:
```
START → CHECK → WORDS → CREATE → PLAY → WIN/LOSE → CHECK (restart)
```

**Rationale**: FSMs provide clear control flow and make state transitions explicit and maintainable. Each state handles a distinct phase of gameplay, making the code modular and easier to debug.

### Game Entity Structure
Implemented struct-based entity management rather than arrays of separate parameters:
```c
typedef struct rocket {
    int row, col;      // Position
    int width, height; // Dimensions
    int cd, rd;        // Column/row deltas for movement
} ROCKET;
```

**Rationale**: Encapsulating entity data reduces parameter passing overhead and makes the code more scalable. Adding new entity types requires minimal refactoring.

## Performance Optimizations

### DMA-Accelerated Rendering
Used hardware DMA (Direct Memory Access) transfers for large graphics operations instead of CPU loops:
```c
drawFullScreenImageDMA(galaga);  // DMA transfer (~60KB)
drawRectDMA(row, col, width, height, BLACK);  // Efficient memory fill
```

**Why**: DMA transfers bypass the CPU entirely, freeing it up for game logic while the hardware handles graphics. This is critical on embedded systems with limited CPU cycles. Results in smooth 60 FPS gameplay.

**Benchmark**: CPU-based memcpy would stall the frame; DMA allows collision detection and input handling in the same frame.

### VBlank Synchronization
Game loop uses `waitForVBlank()` before rendering:
```c
waitForVBlank();  // Wait for screen refresh
// Perform game logic and rendering
```

**Why**: Synchronizing with VBlank prevents screen tearing and locks the game to the GBA's 60 Hz refresh rate, ensuring consistent frame pacing.

### Collision Detection Algorithm
Implemented bounding box collision detection with optimized bounds checking:
```c
if ((miss.row == enemies[j].row + 21 || miss.row == enemies[j].row + 20 || 
     miss.row == enemies[j].row + 19) && 
    (miss.col > enemies[j].col + 1) && 
    (miss.col < enemies[j].col + 15))
```

**Rationale**: 
- Checks only relevant neighbors rather than all 5 enemies
- Uses frame-perfect hit detection with 3-pixel tolerance (accounts for missile animation frames)
- O(n) complexity where n=5 (acceptable for small enemy counts)

**Trade-off**: Pixel-perfect collision would be more accurate but requires sprite bitmasking—not worth the CPU overhead for this game.

## Key Challenges & Solutions

### Challenge 1: Memory Constraints
**Problem**: GBA has only 32 KB internal RAM. Large sprite data can't be pre-loaded.

**Solution**: 
- Sprites generated at compile-time using `nin10kit` and stored in ROM
- Only entity state (positions, velocities) kept in RAM
- Reuse same sprite asset (`enemy` bitmap) for all enemies

**Result**: ~2 KB RAM used for game state vs. potential 100+ KB if sprites were stored separately.

### Challenge 2: Input Debouncing
**Problem**: Raw button reads can cause multiple state transitions in a single frame.

**Solution**: 
Implemented key press macro that tracks previous frame state:
```c
#define KEY_JUST_PRESSED(key, buttons, oldbuttons) \
    (~(KEY_DOWN(key, oldbuttons)) & (KEY_DOWN(key, buttons)))
```

This ensures a key only registers once per press, not every frame while held.

### Challenge 3: Sprite Visibility & Undrawing
**Problem**: Sprites need to be "erased" before redrawing in new position to avoid trails.

**Solution**: 
Implemented inverse drawing: draw black rectangle over old position before drawing new sprite position:
```c
drawRectDMA(rock.row, rock.col - rock.cd, ROCKET_WIDTH, ROCKET_HEIGHT, BLACK);
drawImageDMA(rock.row, rock.col, ROCKET_WIDTH, ROCKET_HEIGHT, rocket);
```

**Trade-off**: Slightly slower than dirty rect tracking but simpler to implement and sufficient for this project's performance requirements.

### Challenge 4: Game State Persistence
**Problem**: Score/high score needed to persist across multiple play sessions (in theory).

**Solution**: 
Stored in main memory (currently volatile). On real GBA hardware with SRAM, this could be persisted:
```c
unsigned int highScore = 0;  // Would save to SRAM on real hardware
```

## What Went Well

**Clean separation of concerns**: Graphics (gba.c), game logic (game.c), and text rendering (text.c) are cleanly separated
**Efficient use of hardware**: DMA and VBlank synchronization result in smooth, stable gameplay
**Manageable scope**: Single-level game with 5 enemies keeps complexity down while demonstrating core concepts
**Modular code**: Easy to add new game states or entity types

## What Could Be Improved

### Performance Enhancements (if time permitted)
- **Sprite animation**: Implement sprite bitmasks for pixel-perfect collision detection
- **Enemy AI**: Implement predictive shooting (current code has commented stubs)
- **Audio**: Add sound effects and background music (would require hardware audio setup)
- **Graphics**: Parallax scrolling background, visual effects for explosions

### Code Quality Improvements
- **Constants file**: Extract magic numbers (row/column bounds) to `#define` constants
- **Function modularization**: Refactor large switch statement into separate handler functions
- **Error handling**: Bounds checking on array accesses
- **Buffer overflow protection**: Use `snprintf()` instead of `sprintf()` for score formatting

### Scalability
- **Enemy wave system**: Multiple levels with increasing difficulty
- **Multiplier scoring**: Bonus multiplier for consecutive hits
- **Lives system UI**: Visual representation of remaining lives
- **Persistent high score**: SRAM integration for real hardware

## Technical Learnings

### Low-Level Systems Programming
- Direct hardware register manipulation (memory-mapped I/O)
- Understanding of CPU/GPU memory hierarchies and DMA transfers
- Embedded constraints: working within 32 KB RAM and fixed CPU clock

### Real-Time Systems
- Frame-locked game loops and synchronization primitives
- The importance of VBlank coordination for visual consistency
- Tight performance budgets: must complete game logic + rendering in one 16ms frame

### Optimization Mindset
- Profile before optimizing: DMA transfer vs. CPU loop is measurable difference
- Know your platform: GBA's hardware capabilities (DMA channels, bus architecture) enable specific optimizations unavailable on other systems
- Hardware-software co-design: choosing algorithms that align with hardware capabilities

## Building & Testing

**Compilation**: GBA toolchain (arm-none-eabi-gcc) with custom Makefile
**Emulation**: Mednafen GBA emulator for testing across platforms
**Debugging**: Limited to print statements and visual inspection (GBA has no built-in debugger)

## References & Resources
- ARM7TDMI architecture documentation (GBA's processor)
- GBA hardware specs (memory map, register usage)
- Tonc GBA programming guide (excellent resource for GBA development)

---

**Created**: CS 2110 Final Project  
**Author**: Henna Mian  
**Platform**: Game Boy Advance (GBA)
