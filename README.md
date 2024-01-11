# C++ Showcase Project

This C++ project showcases my abilities using SFML!

## Prerequisites

Before you begin, ensure you have met the following requirements:

- C++ compiler (e.g., GCC on macOS, MinGW on Windows)
- CMake (version 3.0.0 or higher)
- SFML library (version 2.5.1 or higher)

## Build and Run Instructions

### macOS

1. **Install Dependencies:**
   - Install Xcode Command Line Tools (if not already installed):
     ```bash
     xcode-select --install
     ```

   - Install Homebrew (if not already installed):
     ```bash
     /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
     ```

   - Install SFML using Homebrew:
     ```bash
     brew install sfml
     ```

2. **Build the Project:**
    ```bash
    mkdir build && cd build # cd PhysicsShowcase first
    make clean # optional
    cmake ..
    make
3. **Run Executable**
  ```bash
   ./PhysicsShowcase
  ```
