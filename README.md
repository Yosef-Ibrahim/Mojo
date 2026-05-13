# FCAI Board Games Collection

A comprehensive C++ gaming application featuring 13 unique board game implementations, developed as a team project for **CS213: Object Oriented Programming** at the Faculty of Computers and Artificial Intelligence, Cairo University.

## Overview

This project extends a generic OOP framework to create a diverse collection of Tic-Tac-Toe variants and classic strategy games. Each game implements core OOP principles including inheritance, polymorphism, templates, and smart AI opponents using the Minimax algorithm.

## Key Features

- **13 Unique Games**: From classic Tic-Tac-Toe to innovative variations like Pyramid, Diamond, and Infinity boards
- **Smart AI Opponents**: Minimax algorithm implementation for challenging gameplay
- **Human vs Computer**: Play against random or AI-powered computer players
- **Object-Oriented Design**: Clean architecture with reusable Board, Player, Move, UI, and GameManager classes
- **Doxygen Documentation**: Auto-generated API documentation in the `DOXYGEN/` folder
- **Cross-Platform**: Works on Windows, Linux, and macOS (via g++/clang)

## Tech Stack

| Component | Technology |
|-----------|------------|
| **Language** | C++23 (C++20/17 compatible) |
| **Compiler** | MSVC v143 (Visual Studio 2022) / GCC / Clang |
| **Documentation** | Doxygen |
| **Build System** | Visual Studio Solution / Makefile |

## Prerequisites

Before running the project, ensure you have:

- **C++ Compiler** supporting C++17 or later
  - **Windows**: Visual Studio 2022 (or newer) with C++ workload
  - **Linux/macOS**: GCC 11+ or Clang 14+
- **Terminal/Console**: For running the interactive text-based games

No external dependencies are required — the project uses only the C++ Standard Library.

## Installation & Setup

### 1. Clone the Repository

```bash
git clone https://github.com/your-repo/Board_Games-Assignment_3.git
cd Board_Games-Assignment_3
```

### 2. Windows (Visual Studio)

1. Open `Board_Game_A3.sln` in Visual Studio
2. Select **Build** → **Build Solution** (Ctrl+Shift+B)
3. Press **F5** to run the application

Or compile manually:

```powershell
cl /std:c++17 /EHsc XO_Demo.cpp *.cpp
```

### 3. Linux/macOS (GCC/Clang)

```bash
g++ -std=c++17 -o game XO_Demo.cpp *.cpp
./game
```

Or for a cleaner build, create a simple Makefile:

```makefile
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
TARGET = game
SRC = XO_Demo.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)
```

Then run:
```bash
make
./game
```

## Usage

After running the executable, you'll see the main menu:

```
============================================
      Welcome to FCAI Board Game Station    
============================================
1. Pyramid Tic-Tac-Toe
2. Four-in-a-row (Connect 4)
3. 5x5 Tic-Tac-Toe
4. Word Tic-Tac-Toe
5. Numerical Tic-Tac-Toe
6. Misere Tic-Tac-Toe
7. 4x4 Tic-Tac-Toe
8. Ultimate Tic-Tac-Toe
9. SUS Game
10. Obstacles Tic-Tac-Toe
11. Infinity Tic-Tac-Toe
12. Diamond Tic-Tac-Toe 
13. Memory Tic-Tac-Toe 
14. X-O game
0. Exit 
------------------------------------------------
Enter your choice: 
```

### Playing a Game

1. Enter the number corresponding to your desired game
2. Enter player names when prompted
3. Choose player type: **1** (Human) or **2** (Computer/Random)
4. Follow the in-game instructions to play

**Example**:
```
Enter your choice: 1

========================================
  Welcome to Pyramid Tic-Tac-Toe
========================================

Enter Player 1 name: Alice
Player 1 type (1:Human, 2:Computer): 1
Enter Player 2 name: Bob
Player 2 type (1:Human, 2:Computer): 2
```

### Game Controls

- **Position Input**: When prompted, enter row and column numbers separated by space (e.g., `2 3`)
- **Return to Menu**: Press Enter after each game ends

## Project Structure

```
Board_Games-Assignment_3/
├── BoardGame_Classes.h       # Base OOP framework (Board, Player, Move, UI, GameManager)
├── XO_Demo.cpp               # Main entry point with all game runners
│
├── Game Implementations (Header + Implementation):
├── XO_Classes.{h,cpp}              # Classic 3x3 Tic-Tac-Toe
├── Pyramid_TicTacToe.{h,cpp}        # Pyramid-shaped board
├── FiveByFive_Classes.{h,cpp}       # 5x5 extended grid
├── FourByFour_Classes.{h,cpp}       # 4x4 with moving pieces
├── Four_In_A_Row_UI.{h,cpp}         # Connect Four
├── Word_TicTacToe.{h,cpp}           # Form 3-letter words
├── Numerical_TicTacToe.{h,cpp}      # Odd/even numbers
├── Misere.{h,cpp}                   # Avoid 3-in-a-row
├── Obstacles_Classes.{h,cpp}        # Board with blocked cells
├── Infinity_TicTacToe.{h,cpp}       # Expanding board
├── Diamond_TicTacToe.{h,cpp}        # Diamond-shaped grid
├── Memory_TicTacToe.{h,cpp}        # Hidden positions (bonus)
├── Ultimate_TicTacToe_Classes.{h,cpp} # 9 boards (bonus)
├── sus_classes.{h,cpp}              # S-U-S sequence game
│
├── DOXYGEN/                  # Auto-generated documentation
├── index.html               # Web interface for online play
├── Board_Game_A3.sln        # Visual Studio solution
├── Board_Game_A3.vcxproj    # Visual Studio project
└── README.md                # This file
```

## Game Descriptions

| # | Game | Description |
|---|------|-------------|
| 1 | Pyramid Tic-Tac-Toe | Play on a 3-level pyramid board |
| 2 | Four-in-a-row | Classic Connect 4 — drop pieces to get 4 in a row |
| 3 | 5x5 Tic-Tac-Toe | Extended 5x5 grid, win by forming 3-in-a-row |
| 4 | Word Tic-Tac-Toe | Form valid 3-letter English words to win |
| 5 | Numerical Tic-Tac-Toe | Use odd/even numbers instead of X/O |
| 6 | Misere Tic-Tac-Toe | **Reverse rules** — avoid making 3-in-a-row |
| 7 | 4x4 Tic-Tac-Toe | Tic-Tac-Toe with moving pieces on 4x4 |
| 8 | Ultimate Tic-Tac-Toe | 9 mini-boards — win small boards to win big |
| 9 | SUS Game | Form "S-U-S" sequences to score points |
| 10 | Obstacles Tic-Tac-Toe | Some cells are blocked — adapt your strategy |
| 11 | Infinity Tic-Tac-Toe | Board expands infinitely during play |
| 12 | Diamond Tic-Tac-Toe | Unique diamond-shaped board layout |
| 13 | Memory Tic-Tac-Toe | Positions hidden after placement — remember them! |
| 14 | Classic X-O | Standard 3x3 Tic-Tac-Toe |

## Development Team

| Name | Student ID | Games Developed |
|------|------------|------------------|
| Youssef Ibrahim | 20240671 | 4x4 Tic-Tac-Toe, Pyramid, Obstacles |
| Shahd Ayman | 20240279 | Connect 4, 5x5 Tic-Tac-Toe, Ultimate |
| Marwan Medhat | 20240569 | Misère, Diamond, Infinity, Memory |
| Mohamed Rami | 20240496 | 5x5 Tic-Tac-Toe, Word, Numerical |

## License

This project is created for educational purposes as part of CS213 coursework at Cairo University.

---

**Course**: CS213 — Object Oriented Programming  
**Faculty**: Faculty of Computers and Artificial Intelligence, Cairo University