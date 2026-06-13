import React, { useEffect, useState } from 'react';
import { useGame } from '../context/GameContext';
import { getGameLogic } from '../games/logic';
import { getSmartMove } from '../games/ai';
import { getOpeningMove } from '../games/openingBook';
import { AudioManager } from '../games/audio';
import { TutorialManager } from '../games/tutorial';
import Tutorial from './Tutorial';

const GameBoard: React.FC = () => {
  const { currentGame, players, gameState, setGameState, scores, updateScore, resetGame, theme, difficulty, addLeaderboardEntry } = useGame();
  const [lastMove, setLastMove] = useState<{ x: number; y: number } | null>(null);
  const [selectedPiece, setSelectedPiece] = useState<{ x: number; y: number } | null>(null);
  const [wordLetter, setWordLetter] = useState('A');
  const [numValue, setNumValue] = useState<number | null>(null);
  const [isAIThinking, setIsAIThinking] = useState(false);
  const [showTutorial, setShowTutorial] = useState(false);
  const [tutorialData, setTutorialData] = useState<{ title: string; steps: { title: string; description: string; action: string }[] } | null>(null);

  const getAIDelay = () => {
    switch (difficulty) {
      case 'easy': return 300;
      case 'medium': return 500;
      case 'hard': return 700;
      default: return 500;
    }
  };

  const logic = currentGame ? getGameLogic(currentGame.id) : null;

  const boardCellStyle = (isSelected: boolean, cell: string | null): React.CSSProperties => {
    if (isSelected) {
      return { 
        backgroundColor: 'var(--board-cell-selected)', 
        borderColor: theme === 'light' ? '#eab308' : '#854d0e',
        transform: 'scale(1.05)'
      };
    }
    if (cell === null) {
      return { 
        backgroundColor: 'var(--board-cell-bg)', 
        borderColor: 'var(--board-cell-border)' 
      };
    }
    if (cell === '#') {
      return { 
        backgroundColor: '#1f2937', 
        borderColor: '#111827',
        color: 'white',
        cursor: 'not-allowed' 
      };
    }
    return { 
      backgroundColor: 'var(--card-bg)', 
      borderColor: theme === 'light' ? '#c084fc' : '#a855f7' 
    };
  };

  useEffect(() => {
    if (!gameState && currentGame && logic) {
      const initialBoard = logic.getInitialBoard(currentGame.boardSize);
      setGameState({
        board: initialBoard,
        currentPlayer: 1,
        gameStatus: 'active',
        winner: null,
        moves: [],
      });
    }
  }, [currentGame, gameState, setGameState, logic]);

  useEffect(() => {
    if (!gameState || gameState.gameStatus === 'active' || !players || !currentGame) return;
    addLeaderboardEntry({
      gameId: currentGame.id,
      gameName: currentGame.name,
      winner: gameState.gameStatus === 'won'
        ? players[(gameState.winner as number) - 1].name
        : 'Draw',
      player1: players[0].name,
      player2: players[1].name,
      date: new Date().toLocaleDateString(),
      difficulty,
    });
  }, [gameState?.gameStatus]);

  useEffect(() => {
    if (!currentGame || !gameState) return;
    const data = TutorialManager.getTutorial(currentGame.id);
    if (data && !TutorialManager.hasCompleted(currentGame.id)) {
      setTutorialData(data);
      setShowTutorial(true);
    }
  }, [currentGame?.id]);

  const handleTutorialComplete = () => {
    if (currentGame) TutorialManager.markCompleted(currentGame.id);
    setShowTutorial(false);
    setTutorialData(null);
  };

  const handleTutorialSkip = () => {
    setShowTutorial(false);
    setTutorialData(null);
  };

  if (!gameState || !players || !currentGame || !logic) return null;

  const handleMove = (x: number, y: number, aiSymbol?: string) => {
    if (gameState.gameStatus !== 'active') return;

    const player = players[gameState.currentPlayer - 1];
    let symbol = aiSymbol || player.symbol;

    // Special game logic for symbols/values (if not AI provided)
    if (!aiSymbol) {
      if (currentGame.id === 'word') {
        symbol = wordLetter;
      } else if (currentGame.id === 'numerical') {
        if (numValue === null) return;
        symbol = numValue.toString();
      } else if (currentGame.id === 'sus') {
        symbol = gameState.currentPlayer === 1 ? 'S' : 'U';
      }
    }

    // 4x4 Moving pieces logic
    if (currentGame.id === 'fourbyfour') {
      if (!aiSymbol && !selectedPiece) {
        if (gameState.board[x][y] === player.symbol) {
          setSelectedPiece({ x, y });
        }
        return;
      } else if (aiSymbol || selectedPiece) {
        // AI provides full move info, so it doesn't need "selectedPiece"
        // But for consistency, let's assume if aiSymbol is provided, it might have sourceX/Y if we passed the whole move
        // Actually, let's just use the logic for AI
      }
    }

    // Connect Four gravity
    let targetX = x;
    if (currentGame.id === 'connectfour') {
      targetX = (logic as any).findLowestRow(gameState.board, y);
      if (targetX === -1) return;
    }

    if (!logic.isValidMove(gameState.board, targetX, y, gameState.currentPlayer, gameState.moves)) {
      AudioManager.playError();
      return;
    }

    AudioManager.playMove();
    setLastMove({ x: targetX, y });
    let newBoard = gameState.board.map((row) => [...row]);
    newBoard[targetX][y] = symbol;

    // Obstacles logic: add obstacles every round (2 moves)
    if (currentGame.id === 'obstacles' && (gameState.moves.length + 1) % 2 === 0) {
      newBoard = (logic as any).addObstacles(newBoard);
    }

    completeMove(newBoard, targetX, y);
  };

  // AI Turn Handling
  useEffect(() => {
    if (gameState?.gameStatus === 'active' && players) {
      const currentPlayer = players[gameState.currentPlayer - 1];
      if (currentPlayer.type !== 'human') {
        setIsAIThinking(true);
        const timer = setTimeout(() => {
          let move = null;
          
          if (currentPlayer.type === 'ai') {
            const bookMove = getOpeningMove(
              currentGame.id, gameState.board, gameState.currentPlayer,
              gameState.moves.length, currentPlayer.symbol
            );
            
            const getValidMovesHelper = (b: (string | null)[][], p: number) => {
              const moves: { x: number; y: number }[] = [];
              for (let i = 0; i < b.length; i++) {
                for (let j = 0; j < b[i].length; j++) {
                  if (logic.isValidMove(b, i, j, p, gameState.moves)) {
                    // For Connect Four, only return the lowest row to avoid redundant searches
                    if (currentGame.id === 'connectfour') {
                      const lowest = (logic as any).findLowestRow(b, j);
                      if (i === lowest) moves.push({ x: i, y: j });
                    } else {
                      moves.push({ x: i, y: j });
                    }
                  }
                }
              }
              return moves;
            };

            // Specialized AI logic for complex games
            if (currentGame.id === 'word') {
              move = (logic as any).getBestMove(gameState.board, gameState.currentPlayer);
            } else if (currentGame.id === 'numerical') {
              move = (logic as any).getBestMove(gameState.board, gameState.currentPlayer);
            } else {
              move = bookMove || getSmartMove(
                gameState.board, 
                gameState.currentPlayer, 
                difficulty,
                (board, symbol) => logic.checkWin(board, symbol),
                (board) => logic.isDraw(board),
                getValidMovesHelper
              );
            }

            if (!move && logic.getRandomMove) {
              move = logic.getRandomMove(gameState.board, gameState.currentPlayer, gameState.moves);
            }
          } else if (logic.getRandomMove) {
            move = logic.getRandomMove(gameState.board, gameState.currentPlayer, gameState.moves);
          }

          setIsAIThinking(false);
          
          if (move) {
            const moveWithSource = move as { x: number; y: number; symbol?: string; sourceX?: number; sourceY?: number };
            if (currentGame.id === 'fourbyfour' && moveWithSource.sourceX !== undefined && moveWithSource.sourceY !== undefined) {
              let newBoard = gameState.board.map(row => [...row]);
              newBoard[moveWithSource.x][moveWithSource.y] = currentPlayer.symbol;
              newBoard[moveWithSource.sourceX][moveWithSource.sourceY] = null;
              completeMove(newBoard, moveWithSource.x, moveWithSource.y);
            } else {
              handleMove(move.x, move.y, move.symbol);
            }
          }
        }, getAIDelay());
        return () => {
          clearTimeout(timer);
          setIsAIThinking(false);
        };
      }
    }
  }, [gameState?.currentPlayer, gameState?.gameStatus, difficulty, currentGame?.id, players?.[1]?.type]);

  const completeMove = (newBoard: (string | null)[][], x: number, y: number) => {
    const symbol = newBoard[x][y]!;
    let winner: 1 | 2 | null = null;
    let gameStatus: 'active' | 'won' | 'draw' = 'active';

    // Infinity logic: handle move removal
    let updatedMoves = [...gameState.moves, { x, y, player: gameState.currentPlayer }];
    let finalBoard = newBoard;

    if (currentGame.id === 'infinity') {
      const res = (logic as any).handleHistory({ board: finalBoard, moves: updatedMoves });
      finalBoard = res.board;
      updatedMoves = res.moves;
    }

    // Win check
    if (currentGame.id === 'misere') {
      if ((logic as any).checkLose(finalBoard, symbol)) {
        winner = gameState.currentPlayer === 1 ? 2 : 1;
        updateScore(winner);
        gameStatus = 'won';
      } else if (logic.isDraw(finalBoard)) {
        gameStatus = 'draw';
      }
    } else {
      if (logic.checkWin(finalBoard, symbol)) {
        winner = gameState.currentPlayer;
        updateScore(gameState.currentPlayer);
        gameStatus = 'won';
      } else if (logic.isDraw(finalBoard)) {
        gameStatus = 'draw';
      }
    }

    // Special win check for 5x5 and SUS (at the end)
    if (gameStatus === 'draw') {
      if (currentGame.id === 'fivebyfive') {
        const p1Score = (logic as any).countThreeInARow(finalBoard, players[0].symbol);
        const p2Score = (logic as any).countThreeInARow(finalBoard, players[1].symbol);
        if (p1Score > p2Score) { winner = 1; gameStatus = 'won'; updateScore(1); }
        else if (p2Score > p1Score) { winner = 2; gameStatus = 'won'; updateScore(2); }
      } else if (currentGame.id === 'sus') {
        const scores = (logic as any).calculateScores(finalBoard);
        if (scores.s > scores.u) { winner = 1; gameStatus = 'won'; updateScore(1); }
        else if (scores.u > scores.s) { winner = 2; gameStatus = 'won'; updateScore(2); }
      }
    }

    if (gameStatus === 'won') AudioManager.playWin();
    else if (gameStatus === 'draw') AudioManager.playDraw();

    setGameState({
      board: finalBoard,
      currentPlayer: gameState.currentPlayer === 1 ? 2 : 1,
      gameStatus,
      winner,
      moves: updatedMoves,
    });

    setNumValue(null);
  };

  const restartGame = () => {
    const initialBoard = logic.getInitialBoard(currentGame.boardSize);
    setGameState({
      board: initialBoard,
      currentPlayer: 1,
      gameStatus: 'active',
      winner: null,
      moves: [],
    });
  };

  return (
    <div className="min-h-screen p-6 md:p-4" role="main" aria-label={`${currentGame.name} game`}>
      <div className="max-w-4xl mx-auto">
        <div className="text-center mb-4 md:mb-8">
          <h1 className="text-2xl md:text-4xl font-bold text-white mb-2">{currentGame.name}</h1>
          {players[1].type === 'ai' && (
            <span
              className="inline-block px-3 py-1 rounded-full text-sm font-semibold text-white mb-2"
              style={{
                background: difficulty === 'easy' ? '#22c55e' : difficulty === 'medium' ? '#eab308' : '#ef4444'
              }}
            >
              AI: {typeof difficulty === 'string' ? difficulty.charAt(0).toUpperCase() + difficulty.slice(1) : 'Medium'}
            </span>
          )}
          <div className={`text-lg md:text-2xl ${gameState.gameStatus === 'won' ? 'animate-victory' : ''}`} style={{ color: 'var(--card-text)' }}>
            {gameState.gameStatus === 'active'
              ? `${players[gameState.currentPlayer - 1].name}'s Turn`
              : gameState.gameStatus === 'won'
              ? `🎉 ${players[(gameState.winner as number) - 1].name} Wins!`
              : '🤝 Draw!'}
          </div>
          <div className="sr-only" role="status" aria-live="polite">
            {gameState.gameStatus === 'active'
              ? `It is ${players[gameState.currentPlayer - 1].name}'s turn`
              : gameState.gameStatus === 'won'
              ? `${players[(gameState.winner as number) - 1].name} wins the game`
              : 'The game is a draw'}
          </div>
        </div>

        <div className="grid grid-cols-1 lg:grid-cols-3 gap-4 md:gap-8">
          <div style={{ background: 'var(--card-bg)' }} className="lg:col-span-2 rounded-2xl shadow-2xl p-4 md:p-8 flex flex-col items-center overflow-auto">
            {currentGame.id === 'word' && gameState.gameStatus === 'active' && (
              <div className="mb-6 flex gap-2">
                <span style={{ color: 'var(--card-text)' }} className="font-bold self-center">Pick Letter:</span>
                <input
                  type="text"
                  maxLength={1}
                  value={wordLetter}
                  onChange={(e) => setWordLetter(e.target.value.toUpperCase())}
                  style={{ 
                    background: 'var(--input-bg)', 
                    borderColor: 'var(--input-focus)',
                    color: theme === 'light' ? '#1f2937' : '#f1f5f9'
                  }}
                  className="w-12 h-12 text-center border-2 rounded-lg text-xl font-bold"
                />
              </div>
            )}
            {currentGame.id === 'numerical' && gameState.gameStatus === 'active' && (
              <div className="mb-6 flex flex-wrap gap-2 justify-center">
                <span style={{ color: 'var(--card-text)' }} className="font-bold w-full text-center mb-2">Pick Number:</span>
                {(logic as any).getAvailableNumbers(gameState.board, gameState.currentPlayer).map((n: number) => (
                  <button
                    key={n}
                    onClick={() => setNumValue(n)}
                    className="w-10 h-10 rounded-full font-bold transition"
                    style={{
                      background: numValue === n 
                        ? (theme === 'light' ? '#8b5cf6' : '#7c3aed') 
                        : (theme === 'light' ? '#e5e7eb' : '#475569'),
                      color: numValue === n ? 'white' : (theme === 'light' ? '#1f2937' : '#f1f5f9'),
                    }}
                  >
                    {n}
                  </button>
                ))}
              </div>
            )}

            <div
              role="grid"
              aria-label={`${currentGame.name} game board`}
              style={{
                display: 'grid',
                gridTemplateColumns: `repeat(${gameState.board[0].length}, 1fr)`,
                gap: '6px',
                width: 'fit-content',
                margin: '0 auto 24px',
                maxWidth: '100%',
                overflowX: 'auto',
              }}
              className="touch-manipulation"
            >
              {gameState.board.map((row, x) =>
                row.map((cell, y) => {
                  const isPyramid = currentGame.id === 'pyramid';
                  const isValidPyramid = isPyramid ? (logic as any).isValidSpot(x, y) : true;
                  const isDiamond = currentGame.id === 'diamond';
                  const isValidDiamond = isDiamond ? (logic as any).isValidSpot(x, y) : true;
                  
                  if (!isValidPyramid || !isValidDiamond) return <div key={`${x}-${y}`} className="w-20 h-20" />;

                  const isSelected = selectedPiece?.x === x && selectedPiece?.y === y;
                  const isMemory = currentGame.id === 'memory';
                  const displayCell = isMemory && gameState.gameStatus === 'active' ? '' : cell;

                  // Ultimate Tic-Tac-Toe sector borders
                  const borderClasses = currentGame.id === 'ultimate' 
                    ? `${y % 3 === 2 && y !== 8 ? 'border-r-4 border-r-purple-400' : ''} ${x % 3 === 2 && x !== 8 ? 'border-b-4 border-r-purple-400' : ''}`
                    : '';

                  const isNewMove = lastMove?.x === x && lastMove?.y === y;
                  const cellClasses = [
                    borderClasses,
                    isNewMove ? 'animate-pop-in' : '',
                    cell === null && gameState.gameStatus === 'active' ? 'animate-board-entry' : '',
                  ].filter(Boolean).join(' ');

                  return (
                    <button
                      key={`${x}-${y}`}
                      onClick={() => handleMove(x, y)}
                      disabled={gameState.gameStatus !== 'active' || (cell !== null && currentGame.id !== 'fourbyfour' && cell !== '#')}
                      role="gridcell"
                      aria-label={`Cell ${x + 1}, ${y + 1}${cell ? `, ${cell}` : ', empty'}`}
                      aria-pressed={!!cell}
                      style={{
                        width: '5rem',
                        height: '5rem',
                        borderWidth: '2px',
                        borderRadius: '0.5rem',
                        fontWeight: 'bold',
                        fontSize: '1.875rem',
                        transition: 'all 0.2s',
                        display: 'flex',
                        alignItems: 'center',
                        justifyContent: 'center',
                        ...boardCellStyle(isSelected, cell),
                        animationDelay: cell === null ? `${(x * gameState.board[0].length + y) * 20}ms` : undefined,
                      }}
                      className={cellClasses}
                    >
                      {displayCell}
                    </button>
                  );
                })
              )}
            </div>
            {gameState.gameStatus !== 'active' && (
              <button
                onClick={restartGame}
                className="w-full text-white font-bold py-3 rounded-lg hover:shadow-lg transition"
                style={{ background: theme === 'light' ? '#8b5cf6' : '#7c3aed' }}
              >
                🔄 Play Again
              </button>
            )}
          </div>

          <div className="space-y-6">
            {players.map((player) => (
              <div
                key={player.id}
                role="region"
                aria-label={`${player.name}'s panel`}
                style={{ 
                  background: 'var(--sidebar-bg)',
                  boxShadow: '0 10px 15px -3px rgba(0, 0, 0, 0.1)',
                }}
                className={`rounded-xl p-6 ${
                  gameState.gameStatus === 'active' && gameState.currentPlayer === player.id
                    ? 'ring-4'
                    : ''
                }`}
              >
                <div className="flex items-center justify-between">
                  {player.avatar ? (
                    <img src={player.avatar} alt={player.name} className="w-16 h-16 rounded-full object-cover border-2 border-purple-400" />
                  ) : (
                    <div className="text-4xl">{player.id === 1 ? '👤' : '🤖'}</div>
                  )}
                  {isAIThinking && player.id === 2 && gameState.gameStatus === 'active' && (
                    <span className="text-sm animate-pulse" style={{ color: theme === 'light' ? '#8b5cf6' : '#a78bfa' }}>
                      💭 Thinking...
                    </span>
                  )}
                </div>
                <h3 className="text-xl font-bold mt-2" style={{ color: 'var(--sidebar-text)' }}>{player.name}</h3>
                <p style={{ color: 'var(--card-desc)' }} className="text-sm">
                  {currentGame.id === 'numerical' 
                    ? (player.id === 1 ? 'Odd Numbers' : 'Even Numbers')
                    : currentGame.id === 'sus'
                      ? (player.id === 1 ? 'Symbol S' : 'Symbol U')
                      : player.symbol}
                </p>
                <div className="mt-4 pt-4 border-t" style={{ borderColor: theme === 'light' ? '#e5e7eb' : '#475569' }}>
                  <div style={{ color: 'var(--card-desc)' }}>Score</div>
                  <div className="text-3xl font-bold" style={{ color: theme === 'light' ? '#8b5cf6' : '#a78bfa' }}>
                    {player.id === 1 ? scores.player1 : scores.player2}
                  </div>
                </div>
              </div>
            ))}

            <div style={{ background: 'var(--sidebar-bg)', boxShadow: '0 10px 15px -3px rgba(0, 0, 0, 0.1)' }} className="rounded-xl p-6">
              <h4 className="font-bold mb-2" style={{ color: 'var(--sidebar-text)' }}>📋 Rules</h4>
              <p style={{ color: 'var(--card-desc)' }} className="text-sm">{currentGame.rules}</p>
            </div>

            <button
              onClick={() => {
                AudioManager.playClick();
                const data = TutorialManager.getTutorial(currentGame.id);
                if (data) {
                  setTutorialData(data);
                  setShowTutorial(true);
                }
              }}
              className="w-full font-bold py-3 rounded-lg transition hover:opacity-90"
              style={{
                background: 'var(--cell-bg)',
                color: 'var(--text-primary)',
                border: '1px solid var(--border-color, #555)',
              }}
            >
              ❓ How to Play
            </button>

            {players[1].type === 'ai' && (
              <div style={{ background: 'var(--sidebar-bg)', boxShadow: '0 10px 15px -3px rgba(0, 0, 0, 0.1)' }} className="rounded-xl p-6">
                <h4 className="font-bold mb-2" style={{ color: 'var(--sidebar-text)' }}>⚡ Difficulty</h4>
                <span
                  className="px-3 py-1 rounded-full text-xs font-semibold text-white capitalize"
                  style={{
                    background: difficulty === 'easy' ? '#22c55e' : difficulty === 'medium' ? '#eab308' : '#ef4444'
                  }}
                >
                  {typeof difficulty === 'string' ? difficulty : 'Medium'}
                </span>
              </div>
            )}

            <button
              onClick={resetGame}
              className="w-full text-white font-bold py-3 rounded-lg hover:opacity-90 transition"
              style={{ background: '#ef4444' }}
            >
              ← Back to Menu
            </button>
          </div>
        </div>
      </div>
      {showTutorial && tutorialData && (
        <Tutorial
          tutorial={tutorialData}
          onComplete={handleTutorialComplete}
          onSkip={handleTutorialSkip}
        />
      )}
    </div>
  );
};

export default GameBoard;
