# Single-Player Games Architecture Plan

Here is the architectural plan for integrating **Single-Player Games** (ألعاب فردية) like Sudoku, 2048, or Solitaire into our existing MVP structure. The goal is to reuse our powerful MVP framework without breaking the 2-player logic.

## 1. Core Architecture Changes

### A. Game Metadata (`types.ts`)
We will add a new property `isSinglePlayer?: boolean` to the `Game` interface. 
This tells the system how to handle setup and UI.

### B. The Presenter (`GamePresenter.ts`)
Currently, `GamePresenter` flips the turn after every move: `activePlayerIndex = 0 -> 1 -> 0`.
We will modify the `handleMove` logic:
- **If `isSinglePlayer` is true:** We **do not** switch turns. The `activePlayerIndex` remains `0`.
- The `checkWin` and `isDraw` functions will evaluate if the puzzle is solved or if there are no moves left.

### C. Player Setup (`PlayerSetup.tsx`)
When a user selects a Single-Player game from the menu:
- The `PlayerSetup` screen will **hide the Opponent section completely**.
- It will only ask for "Your Name" and launch the game immediately.

### D. Game Board UI (`GameBoard.tsx`)
- The sidebar will detect `players.length === 1` or `currentGame.isSinglePlayer`.
- It will hide the second player's stats/panel.
- "Turn" indicators will be hidden since it's always the human's turn.

## 2. Updated Architecture Diagram

```mermaid
classDiagram
    %% Core MVP Interfaces
    class IGameModel {
        <<interface>>
        +checkWin(board, symbol) boolean
        +isDraw(board) boolean
        +getInitialBoard(size) Array
        +isValidMove(board, x, y, currentPlayer, moves) boolean
    }

    class Game {
        <<interface>>
        +id: string
        +name: string
        +isSinglePlayer: boolean
    }

    class IPresenter {
        <<interface>>
        +onCellClicked(x, y, symbol)
        +startGame()
        +undoMove()
    }

    class IPlayer {
        <<interface>>
        +name: String
        +type: String
    }

    class SudokuModel {
        <<Model>>
        +checkWin() : Checks if grid is fully solved
        +isValidMove() : Validates digit constraints
    }

    class MinesweeperModel {
        <<Model>>
        +checkWin() : Checks if all non-mine sectors are decrypted
        +checkLose() : Checks if mine sector is detonated
        +isValidMove() : Validates unrevealed cell click
    }

    class Neon2048Model {
        <<Model>>
        +checkWin() : Checks if 2048 block is formed
        +isValidMove() : Checks if grid sliding is possible
    }

    class SolitaireModel {
        <<Model>>
        +checkWin() : Checks if all cards are stacked A-K on foundations
        +isValidMove() : Validates drag-and-drop / select-to-move rules
    }

    class SnakesLaddersModel {
        <<Model>>
        +checkWin() : Checks if player reached cell 100
        +isValidMove() : Rolling is always valid
        +handleHistory() : Rolls die, moves player, handles snakes/ladders
    }

    class MultiPlayerModel {
        <<Model>>
        +checkWin() : Checks TicTacToe, Connect4, Chess etc.
    }

    class GamePresenter {
        -gameState: GameState
        -isSinglePlayer: boolean
        +handleMove()
    }

    class ReactGameBoard {
        <<React Component>>
        +renderBoard()
        +renderSidebar()
    }

    %% Relationships
    IPresenter <|.. GamePresenter : Implements
    IPlayer <|.. HumanPlayer : Implements
    IGameModel <|.. SudokuModel : Implements
    IGameModel <|.. MinesweeperModel : Implements
    IGameModel <|.. Neon2048Model : Implements
    IGameModel <|.. SolitaireModel : Implements
    IGameModel <|.. SnakesLaddersModel : Implements
    IGameModel <|.. MultiPlayerModel : Implements
    GamePresenter --> IGameModel : Uses
    GamePresenter --> ReactGameBoard : Updates
```

## User Review Required
> [!IMPORTANT]
> This architecture ensures that **Single-Player Games** will fit perfectly into the SOLID design we already built without duplicating code.
> 
> What do you think of this architecture? Also, **which specific single-player games** would you like to add first? (e.g., Sudoku, 2048, Sliding Puzzle, etc.)
