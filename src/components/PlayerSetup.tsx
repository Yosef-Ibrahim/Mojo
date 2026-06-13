import React, { useState, useEffect } from 'react';
import { useGame } from '../context/GameContext';
import { useSocket } from '../context/SocketContext';
import { useAuth } from '../context/AuthContext';
import { PlayerType, Player } from '../types';

const PlayerSetup: React.FC = () => {
  const { currentGame, setPlayers, difficulty, setDifficulty } = useGame();
  const { createRoom, activeRoom, connectionStatus, connectionError, clearError } = useSocket();
  const { user } = useAuth();

  // States for 1v1 / solo
  const [player1Name, setPlayer1Name] = useState(user?.username || 'Agent Alpha');
  const [player2Name, setPlayer2Name] = useState('Cyber Bot');
  const [player2Type, setPlayer2Type] = useState<PlayerType>('ai');
  const [player1Avatar, setPlayer1Avatar] = useState<string | undefined>(undefined);
  const [player2Avatar, setPlayer2Avatar] = useState<string | undefined>(undefined);

  // States for Snakes & Ladders (2-4 players)
  const [snakesPlayerCount, setSnakesPlayerCount] = useState<number>(2);
  const [snakesNames, setSnakesNames] = useState<string[]>([user?.username || 'Agent Alpha', 'Agent Beta', 'Agent Gamma', 'Agent Delta']);
  const [snakesGameMode, setSnakesGameMode] = useState<'friends' | 'computer'>('friends');

  // Online multiplayer state
  const [onlineTab, setOnlineTab] = useState<'local' | 'online'>('local');
  const [isCreatingRoom, setIsCreatingRoom] = useState(false);
  const [shareUrl, setShareUrl] = useState<string | null>(null);
  const [linkCopied, setLinkCopied] = useState(false);

  useEffect(() => {
    if (player2Type === 'ai') {
      setPlayer2Name('Cyber Bot');
    } else {
      setPlayer2Name('Player 2');
    }
  }, [player2Type]);

  // When room is created, generate shareable link
  useEffect(() => {
    if (activeRoom && isCreatingRoom) {
      const url = `${window.location.origin}${window.location.pathname}?roomId=${activeRoom.roomId}`;
      setShareUrl(url);
      setIsCreatingRoom(false);
    }
  }, [activeRoom, isCreatingRoom]);

  const handleCreateOnlineRoom = () => {
    if (!currentGame) return;
    setIsCreatingRoom(true);
    createRoom(currentGame.id);
  };

  const handleCopyLink = () => {
    if (shareUrl) {
      navigator.clipboard.writeText(shareUrl).then(() => {
        setLinkCopied(true);
        setTimeout(() => setLinkCopied(false), 2500);
      });
    }
  };

  const handleImageUpload = (id: number, e: React.ChangeEvent<HTMLInputElement>) => {
    const file = e.target.files?.[0];
    if (file) {
      const reader = new FileReader();
      reader.onloadend = () => {
        if (id === 1) setPlayer1Avatar(reader.result as string);
        else setPlayer2Avatar(reader.result as string);
      };
      reader.readAsDataURL(file);
    }
  };

  const handleStart = () => {
    if (currentGame?.id === 'snakesladders') {
      const snakesPlayers: Player[] = [];
      const symbols = ['🔴', '🔵', '🟢', '🟡'];
      const defaultNames = ['Agent Alpha', 'Cyber Bot Beta', 'Cyber Bot Gamma', 'Cyber Bot Delta'];
      for (let i = 0; i < snakesPlayerCount; i++) {
        const isAI = snakesGameMode === 'computer' && i > 0;
        const nameInput = i === 0
          ? (snakesNames[0]?.trim() || 'Agent Alpha')
          : (snakesGameMode === 'computer' ? defaultNames[i] : (snakesNames[i]?.trim() || `Agent ${String.fromCharCode(66 + i)}`));
        snakesPlayers.push({
          id: i + 1,
          name: nameInput,
          symbol: symbols[i],
          type: isAI ? 'ai' : 'human',
          score: 0,
          avatar: i === 0 ? player1Avatar : undefined
        });
      }
      setPlayers(snakesPlayers);
    } else if (currentGame?.isSinglePlayer) {
      setPlayers([
        { id: 1, name: player1Name.trim() || 'Agent Alpha', symbol: 'X', type: 'human', score: 0, avatar: player1Avatar },
        { id: 2, name: 'SOLO_ENGINE', symbol: 'O', type: 'ai', score: 0, avatar: undefined },
      ]);
    } else {
      if (player1Name.trim() && player2Name.trim()) {
        setPlayers([
          { id: 1, name: player1Name.trim(), symbol: 'X', type: 'human', score: 0, avatar: player1Avatar },
          { id: 2, name: player2Name.trim(), symbol: 'O', type: player2Type, score: 0, avatar: player2Avatar },
        ]);
      }
    }
  };

  const updateSnakesName = (index: number, val: string) => {
    setSnakesNames(prev => {
      const next = [...prev];
      next[index] = val;
      return next;
    });
  };

  const isStartDisabled = () => {
    if (currentGame?.id === 'snakesladders') {
      if (snakesGameMode === 'computer') {
        return !snakesNames[0]?.trim();
      }
      for (let i = 0; i < snakesPlayerCount; i++) {
        if (!snakesNames[i]?.trim()) return true;
      }
      return false;
    }
    if (currentGame?.isSinglePlayer) {
      return !player1Name.trim();
    }
    return !player1Name.trim() || !player2Name.trim();
  };

  return (
    <div className="min-h-screen flex items-center justify-center p-6 md:p-12 text-[var(--card-text)]">
      <div className="cyber-panel neon-glow-border-cyan max-w-lg w-full p-8 md:p-12 relative overflow-hidden bg-[var(--card-bg)]">
        <div className="absolute top-0 right-0 bg-[#d946ef] text-black font-mono px-3 py-0.5 text-xs border-b border-l border-white/10 font-bold uppercase tracking-wider">
          INIT_SYSTEM
        </div>

        <div className="text-center mb-8 border-b border-white/10 pb-6">
          <div className="text-6xl mb-3">{currentGame?.icon}</div>
          <h1 className="text-2xl md:text-3xl font-black font-display text-[var(--card-text)] uppercase tracking-wide">
            ENTER THE ARENA
          </h1>
          <p className="mt-2 text-xs text-[var(--card-desc)] uppercase font-mono tracking-widest">
            INITIALIZATION PROTOCOL 01-{currentGame?.id.toUpperCase()}
          </p>
        </div>

        {/* Mode Tab — LOCAL vs ONLINE (only for non-singleplayer games) */}
        {!currentGame?.isSinglePlayer && (
          <div className="flex gap-2 mb-6">
            <button
              onClick={() => { setOnlineTab('local'); clearError(); }}
              className={`flex-1 py-2 font-bold font-mono text-[10px] uppercase brutal-border transition ${
                onlineTab === 'local'
                  ? 'bg-[#00f0ff] text-black shadow-brutal-black'
                  : 'bg-[#161a23] text-white hover:bg-black/30'
              }`}
            >
              🖥️ LOCAL PLAY
            </button>
            <button
              onClick={() => { setOnlineTab('online'); clearError(); }}
              className={`flex-1 py-2 font-bold font-mono text-[10px] uppercase brutal-border transition ${
                onlineTab === 'online'
                  ? 'bg-[#d946ef] text-black shadow-brutal-black'
                  : 'bg-[#161a23] text-white hover:bg-black/30'
              }`}
            >
              🌐 ONLINE FRIEND
            </button>
          </div>
        )}

        {/* ══════════════════ ONLINE LOBBY PANEL ══════════════════ */}
        {onlineTab === 'online' && !currentGame?.isSinglePlayer && (
          <div className="space-y-4">
            {connectionError && (
              <div className="cyber-panel p-3 border border-red-500/50 bg-red-900/20 text-red-400 font-mono text-xs uppercase">
                ⚠ {connectionError}
              </div>
            )}

            {/* No room yet → show create button */}
            {!activeRoom && (
              <div className="cyber-panel p-6 border border-white/5 bg-[#0a0d14] text-center space-y-4">
                <div className="text-4xl">🔗</div>
                <p className="text-xs font-mono text-[#94a3b8] uppercase tracking-wider">
                  Host a private room and share the link with your friend to play together in real-time.
                </p>
                <div className="text-[10px] font-mono text-white/30 uppercase">
                  Server: <span className={connectionStatus === 'connected' ? 'text-[#10b981]' : 'text-[#fbbf24]'}>{connectionStatus.toUpperCase()}</span>
                </div>
                <button
                  onClick={handleCreateOnlineRoom}
                  disabled={isCreatingRoom || connectionStatus !== 'connected'}
                  className="w-full py-3 font-black font-display text-sm uppercase brutal-border bg-[#d946ef] text-black hover:translate-x-0.5 hover:translate-y-0.5 transition-all disabled:opacity-50 disabled:cursor-not-allowed"
                >
                  {isCreatingRoom ? '⟳ CREATING ROOM...' : '⚡ CREATE PRIVATE ROOM'}
                </button>
              </div>
            )}

            {/* Room created, waiting for opponent */}
            {activeRoom && activeRoom.status === 'waiting' && shareUrl && (
              <div className="cyber-panel p-6 border border-[#d946ef]/30 bg-[#0a0d14] space-y-5">
                <div className="text-center">
                  <div className="w-10 h-10 border-4 border-[#d946ef] border-t-transparent rounded-full animate-spin mx-auto mb-3"></div>
                  <p className="text-xs font-bold font-mono text-[#d946ef] uppercase tracking-widest">WAITING FOR OPPONENT...</p>
                  <p className="text-[10px] text-[#94a3b8] font-mono mt-1 uppercase">
                    Room: {activeRoom.roomId.substring(0, 8).toUpperCase()}...
                  </p>
                </div>
                <div className="space-y-2">
                  <label className="text-[10px] font-mono text-white/40 uppercase tracking-wider block">
                    Share this link with your friend:
                  </label>
                  <div className="flex gap-2">
                    <input
                      readOnly
                      value={shareUrl}
                      onClick={(e) => (e.target as HTMLInputElement).select()}
                      className="flex-1 px-2 py-2 brutal-border bg-black/30 text-[#00f0ff] font-mono text-[10px] focus:outline-none cursor-text"
                    />
                    <button
                      onClick={handleCopyLink}
                      className={`px-3 py-2 font-bold font-mono text-[10px] uppercase brutal-border transition shrink-0 ${
                        linkCopied ? 'bg-[#10b981] text-black' : 'bg-[#00f0ff] text-black hover:opacity-90'
                      }`}
                    >
                      {linkCopied ? '✓ COPIED' : 'COPY'}
                    </button>
                  </div>
                </div>
              </div>
            )}

            {/* Both players present → game starting */}
            {activeRoom && activeRoom.status === 'active' && (
              <div className="cyber-panel p-5 border border-[#10b981]/40 bg-[#0a0d14] text-center space-y-3">
                <div className="text-3xl">🎮</div>
                <p className="text-xs font-bold font-mono text-[#10b981] uppercase tracking-widest animate-pulse">
                  ✓ OPPONENT JOINED — LAUNCHING GAME...
                </p>
                <div className="flex items-center justify-center gap-4 text-[10px] font-mono text-white/60 uppercase">
                  {activeRoom.players.map((p, i) => (
                    <span key={i} className={i === 0 ? 'text-[#00f0ff]' : 'text-[#d946ef]'}>
                      {i === 0 ? '🔵' : '🔴'} {p.username}
                    </span>
                  ))}
                </div>
              </div>
            )}
          </div>
        )}

        {/* ══════════════════ LOCAL GAME CONFIG ══════════════════ */}
        {(onlineTab === 'local' || currentGame?.isSinglePlayer) && (
          <div>
            {currentGame?.id === 'snakesladders' ? (
              /* Snakes & Ladders Party Config */
              <div className="space-y-6">
                {/* Match Protocol Selector */}
                <div className="cyber-panel p-4 border border-white/5 bg-[#0a0d14]">
                  <label className="block text-xs font-mono font-bold text-white mb-3 uppercase tracking-wider">
                    MATCH PROTOCOL
                  </label>
                  <div className="flex gap-2">
                    <button
                      onClick={() => setSnakesGameMode('friends')}
                      className={`flex-1 py-2.5 font-bold font-mono text-xs uppercase brutal-border transition ${
                        snakesGameMode === 'friends'
                          ? 'bg-[#d946ef] text-black shadow-brutal-black'
                          : 'bg-[#161a23] text-white hover:bg-black/30'
                      }`}
                    >
                      👥 LOCAL FRIENDS
                    </button>
                    <button
                      onClick={() => setSnakesGameMode('computer')}
                      className={`flex-1 py-2.5 font-bold font-mono text-xs uppercase brutal-border transition ${
                        snakesGameMode === 'computer'
                          ? 'bg-[#fbbf24] text-black shadow-brutal-black'
                          : 'bg-[#161a23] text-white hover:bg-black/30'
                      }`}
                    >
                      🤖 VS COMPUTER
                    </button>
                  </div>
                </div>

                <div className="cyber-panel p-4 border border-white/5 bg-[#0a0d14]">
                  <label className="block text-xs font-mono font-bold text-white mb-3 uppercase tracking-wider">
                    PLAYER_COUNT // INTENSITY
                  </label>
                  <div className="flex gap-2">
                    {[2, 3, 4].map(count => (
                      <button
                        key={count}
                        onClick={() => setSnakesPlayerCount(count)}
                        className={`flex-1 py-2 font-bold font-mono text-xs uppercase brutal-border transition ${
                          snakesPlayerCount === count
                            ? 'bg-[#00f0ff] text-black shadow-brutal-black'
                            : 'bg-[#161a23] text-white hover:bg-black/30'
                        }`}
                      >
                        {count} PLAYERS
                      </button>
                    ))}
                  </div>
                </div>

                <div className="space-y-4">
                  {Array.from({ length: snakesPlayerCount }).map((_, i) => {
                    const isAI = snakesGameMode === 'computer' && i > 0;
                    const aiNames = ['Agent Alpha', 'Cyber Bot Beta', 'Cyber Bot Gamma', 'Cyber Bot Delta'];
                    return (
                      <div key={i} className="cyber-panel p-4 border border-white/5 relative">
                        <span className={`absolute top-0 left-0 text-black font-mono text-[9px] px-2 py-0.5 border-b border-r border-white/10 font-bold uppercase ${
                          i === 0 ? 'bg-[#00f0ff]' : i === 1 ? 'bg-[#d946ef]' : i === 2 ? 'bg-[#10b981]' : 'bg-[#fbbf24]'
                        }`}>
                          {isAI ? `COMPUTER_0${i + 1}_AGENT` : `AGENT_0${i + 1}_ALIAS`}
                        </span>
                        <div className="flex gap-3 items-center mt-2">
                          <span className="text-2xl shrink-0">
                            {i === 0 ? '🔴' : i === 1 ? '🔵' : i === 2 ? '🟢' : '🟡'}
                          </span>
                          {isAI ? (
                            <div className="flex-1 px-3 py-2 brutal-border bg-[#161a23]/50 text-white/40 font-mono text-xs uppercase select-none">
                              {aiNames[i]} (AUTO_BOT)
                            </div>
                          ) : (
                            <input
                              type="text"
                              value={snakesNames[i] || ''}
                              onChange={(e) => updateSnakesName(i, e.target.value)}
                              placeholder={i === 0 ? "Enter your name" : `Enter name for player ${i + 1}`}
                              className="flex-1 px-3 py-2 brutal-border bg-[var(--input-bg)] text-[var(--card-text)] font-mono text-xs focus:outline-none focus:border-[#00f0ff] transition uppercase"
                            />
                          )}
                        </div>
                      </div>
                    );
                  })}
                </div>
              </div>
            ) : (
              /* Solo & Dual Game Config */
              <div>
                {/* Player 1 Entry */}
                <div className="mb-6 cyber-panel p-4 border border-white/5 relative">
                  <span className="absolute top-0 left-0 bg-[#00f0ff] text-black font-mono text-[9px] px-2 py-0.5 border-b border-r border-white/10 font-bold uppercase">
                    AGENT_01_CREDENTIALS
                  </span>
                  <label className="block text-[10px] font-mono font-bold text-[var(--card-text)] mb-2 mt-2 uppercase">Your Identifier</label>
                  <div className="flex gap-3 items-center">
                    <div className="relative w-12 h-12 brutal-border bg-[var(--input-bg)] flex items-center justify-center cursor-pointer hover:bg-black/10 transition shrink-0">
                      {player1Avatar ? (
                        <img src={player1Avatar} alt="Avatar" className="w-full h-full object-cover" />
                      ) : (
                        <span className="text-[9px] font-mono font-bold text-[var(--card-text)]">AVATAR</span>
                      )}
                      <input
                        type="file"
                        accept="image/*"
                        onChange={(e) => handleImageUpload(1, e)}
                        className="absolute inset-0 opacity-0 cursor-pointer"
                        title="Upload Avatar"
                      />
                    </div>
                    <input
                      type="text"
                      value={player1Name}
                      onChange={(e) => setPlayer1Name(e.target.value)}
                      placeholder="Enter identifier"
                      className="flex-1 px-3 py-2 brutal-border bg-[var(--input-bg)] text-[var(--card-text)] font-mono text-xs focus:outline-none focus:border-[#00f0ff] transition uppercase"
                    />
                  </div>
                </div>

                {/* Player 2 Entry */}
                {!currentGame?.isSinglePlayer && (
                  <div className="mb-6 cyber-panel p-4 border border-white/5 relative">
                    <span className="absolute top-0 left-0 bg-[#d946ef] text-black font-mono text-[9px] px-2 py-0.5 border-b border-r border-white/10 font-bold uppercase">
                      AGENT_02_SETTINGS
                    </span>
                    <div className="flex gap-2 mb-3 mt-3">
                      {(['ai', 'human'] as const).map((type) => (
                        <button
                          key={type}
                          onClick={() => setPlayer2Type(type)}
                          className={`flex-1 py-1.5 font-bold font-mono text-[10px] uppercase brutal-border transition ${
                            player2Type === type
                              ? 'bg-[#d946ef] text-black shadow-brutal-black'
                              : 'bg-[#161a23] text-white hover:bg-black/15'
                          }`}
                        >
                          {type === 'ai' ? 'CYBER AI' : 'LOCAL MATE'}
                        </button>
                      ))}
                    </div>

                    <label className="block text-[10px] font-mono font-bold text-[var(--card-text)] mb-2 uppercase">Opponent Name</label>
                    <div className="flex gap-3 items-center">
                      <div className="relative w-12 h-12 brutal-border bg-[var(--input-bg)] flex items-center justify-center cursor-pointer hover:bg-black/10 transition shrink-0">
                        {player2Avatar ? (
                          <img src={player2Avatar} alt="Avatar" className="w-full h-full object-cover" />
                        ) : (
                          <span className="text-[9px] font-mono font-bold text-[var(--card-text)]">{player2Type === 'human' ? 'MATE' : 'BOT'}</span>
                        )}
                        {player2Type === 'human' && (
                          <input
                            type="file"
                            accept="image/*"
                            onChange={(e) => handleImageUpload(2, e)}
                            className="absolute inset-0 opacity-0 cursor-pointer"
                            title="Upload Avatar"
                          />
                        )}
                      </div>
                      <input
                        type="text"
                        value={player2Name}
                        onChange={(e) => setPlayer2Name(e.target.value)}
                        placeholder={player2Type === 'human' ? 'Player 2 name' : 'Computer name'}
                        className="flex-1 px-3 py-2 brutal-border bg-[var(--input-bg)] text-[var(--card-text)] font-mono text-xs focus:outline-none focus:border-[#d946ef] transition uppercase"
                      />
                    </div>
                  </div>
                )}

                {/* Difficulty Selection */}
                {(player2Type === 'ai' || currentGame?.isSinglePlayer) && (
                  <div className="mb-6 cyber-panel p-4 border border-white/5 relative">
                    <span className="absolute top-0 left-0 bg-[#fbbf24] text-black font-mono text-[9px] px-2 py-0.5 border-b border-r border-white/10 font-bold uppercase">
                      SECTOR_DIFFICULTY
                    </span>
                    <label className="block text-[10px] font-mono font-bold text-[var(--card-text)] mb-2 mt-2 uppercase">Grid Complexity</label>
                    <div className="flex gap-2">
                      {(['easy', 'medium', 'hard'] as const).map((level) => (
                        <button
                          key={level}
                          onClick={() => setDifficulty(level)}
                          className={`flex-1 py-1.5 font-bold font-mono text-[10px] uppercase brutal-border transition ${
                            difficulty === level
                              ? 'bg-[#fbbf24] text-black shadow-brutal-black'
                              : 'bg-[#161a23] text-white hover:bg-black/15'
                          }`}
                        >
                          {level}
                        </button>
                      ))}
                    </div>
                  </div>
                )}
              </div>
            )}

            {/* Launch Button */}
            <button
              onClick={handleStart}
              disabled={isStartDisabled()}
              className="w-full text-black font-black py-3 uppercase text-sm font-display tracking-wider brutal-border bg-[#00f0ff] shadow-brutal-black hover:translate-x-0.5 hover:translate-y-0.5 hover:shadow-[1px_1px_0px_0px_rgba(0,0,0,1)] active:translate-x-1 active:translate-y-1 active:shadow-none transition-all duration-150 disabled:opacity-50 disabled:cursor-not-allowed disabled:transform-none disabled:shadow-none mt-4"
            >
              INITIALIZE SIMULATION →
            </button>
          </div>
        )}
      </div>
    </div>
  );
};

export default PlayerSetup;
