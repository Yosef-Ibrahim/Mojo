import React from 'react';
import { useGame } from '../context/GameContext';
import { GAMES } from '../games';

const GameMenu: React.FC = () => {
  const { setCurrentGame } = useGame();
  const gameList = Object.values(GAMES);

  // Group games
  const singlePlayerGames = gameList.filter((g) => g.isSinglePlayer);
  const partyGames = gameList.filter((g) => g.category === 'party');
  const multiplayerGames = gameList.filter((g) => !g.isSinglePlayer && g.category !== 'party');

  return (
    <div className="p-6 md:p-10 text-[var(--card-text)]">
      {/* Title Header */}
      <div className="cyber-panel neon-glow-border-cyan p-8 mb-12 relative overflow-hidden">
        <div className="absolute top-0 right-0 bg-[#00f0ff] text-black font-mono px-3 py-1 text-xs border-b border-l border-white/10 font-bold uppercase tracking-widest">
          SYS // ACTIVE_ARENA
        </div>
        <h1 className="text-3xl md:text-5xl font-black font-display tracking-tight text-[var(--card-text)] mb-2 uppercase">
          ENTER THE <span className="text-[#00f0ff]">ARENA</span>
        </h1>
        <p className="text-xs font-mono text-[#fbbf24] uppercase tracking-widest">
          SIMULATION_PROTOCOL: MULTI_AGENT // CHOOSE YOUR MODULE
        </p>
      </div>

      <div className="space-y-16">
        {/* Section 1: Solo Missions */}
        {singlePlayerGames.length > 0 && (
          <div>
            <div className="border-b border-white/10 mb-8 pb-3">
              <h2 className="text-xl md:text-2xl font-black font-display text-[#fbbf24] uppercase tracking-wide">
                SOLO_MISSIONS.SYS
              </h2>
              <p className="text-[10px] font-mono text-[var(--card-desc)] uppercase mt-1">Single player diagnostics & malware cleaning</p>
            </div>
            <div className="grid grid-cols-1 sm:grid-cols-2 lg:grid-cols-3 gap-6">
              {singlePlayerGames.map((game) => (
                <button
                  key={game.id}
                  onClick={() => setCurrentGame(game)}
                  className="cyber-panel cyber-card-interactive text-left p-6 relative overflow-hidden flex flex-col justify-between h-56 cursor-pointer bg-[var(--card-bg)] text-[var(--card-text)]"
                >
                  <div>
                    <div className="flex justify-between items-start mb-3">
                      <span className="text-4xl">{game.icon}</span>
                      <span className="font-mono text-[10px] text-[#fbbf24] border border-[#fbbf24]/20 px-2 py-0.5 font-bold uppercase">
                        SOLO
                      </span>
                    </div>
                    <h3 className="text-lg md:text-xl font-bold font-display text-[var(--card-text)] mb-2 uppercase tracking-wide">
                      {game.name}
                    </h3>
                    <p className="text-xs text-[var(--card-desc)] line-clamp-2">
                      {game.description}
                    </p>
                  </div>
                  <div className="mt-4 flex items-center justify-between">
                    <span className={`px-2 py-0.5 rounded text-[10px] font-bold text-black ${
                      game.difficulty === 'easy' ? 'bg-[#10b981]' : game.difficulty === 'medium' ? 'bg-[#fbbf24]' : 'bg-[#ef4444]'
                    }`}>
                      DIFF: {game.difficulty.toUpperCase()}
                    </span>
                    <span className="font-mono text-xs font-bold text-[#fbbf24] hover:underline">
                      LOAD_SIM →
                    </span>
                  </div>
                </button>
              ))}
            </div>
          </div>
        )}

        {/* Section 2: Party Protocol (Section Alone) */}
        {partyGames.length > 0 && (
          <div>
            <div className="border-b border-white/10 mb-8 pb-3">
              <h2 className="text-xl md:text-2xl font-black font-display text-[#d946ef] uppercase tracking-wide">
                PARTY_PROTOCOL.SYS
              </h2>
              <p className="text-[10px] font-mono text-[var(--card-desc)] uppercase mt-1">Multiplayer local party simulations for multiple players</p>
            </div>
            <div className="grid grid-cols-1 sm:grid-cols-2 lg:grid-cols-3 gap-6">
              {partyGames.map((game) => (
                <button
                  key={game.id}
                  onClick={() => setCurrentGame(game)}
                  className="cyber-panel cyber-card-interactive text-left p-6 relative overflow-hidden flex flex-col justify-between h-56 cursor-pointer bg-[var(--card-bg)] text-[var(--card-text)] border border-[#d946ef]/20"
                >
                  <div>
                    <div className="flex justify-between items-start mb-3">
                      <span className="text-4xl">{game.icon}</span>
                      <span className="font-mono text-[10px] text-[#d946ef] border border-[#d946ef]/20 px-2 py-0.5 font-bold uppercase">
                        PARTY
                      </span>
                    </div>
                    <h3 className="text-lg md:text-xl font-bold font-display text-[var(--card-text)] mb-2 uppercase tracking-wide">
                      {game.name}
                    </h3>
                    <p className="text-xs text-[var(--card-desc)] line-clamp-2">
                      {game.description}
                    </p>
                  </div>
                  <div className="mt-4 flex items-center justify-between">
                    <span className={`px-2 py-0.5 rounded text-[10px] font-bold text-black ${
                      game.difficulty === 'easy' ? 'bg-[#10b981]' : game.difficulty === 'medium' ? 'bg-[#fbbf24]' : 'bg-[#ef4444]'
                    }`}>
                      DIFF: {game.difficulty.toUpperCase()}
                    </span>
                    <span className="font-mono text-xs font-bold text-[#d946ef] hover:underline">
                      LOAD_SIM →
                    </span>
                  </div>
                </button>
              ))}
            </div>
          </div>
        )}

        {/* Section 3: Arena Protocol */}
        {multiplayerGames.length > 0 && (
          <div>
            <div className="border-b border-white/10 mb-8 pb-3">
              <h2 className="text-xl md:text-2xl font-black font-display text-[#00f0ff] uppercase tracking-wide">
                ARENA_DUAL_PROTOCOL.SYS
              </h2>
              <p className="text-[10px] font-mono text-[var(--card-desc)] uppercase mt-1">Challenge human agents or AI tactical algorithms</p>
            </div>
            <div className="grid grid-cols-1 sm:grid-cols-2 lg:grid-cols-3 gap-6">
              {multiplayerGames.map((game) => (
                <button
                  key={game.id}
                  onClick={() => setCurrentGame(game)}
                  className="cyber-panel cyber-card-interactive text-left p-6 relative overflow-hidden flex flex-col justify-between h-56 cursor-pointer bg-[var(--card-bg)] text-[var(--card-text)]"
                >
                  <div>
                    <div className="flex justify-between items-start mb-3">
                      <span className="text-4xl">{game.icon}</span>
                      <span className="font-mono text-[10px] text-[#00f0ff] border border-[#00f0ff]/20 px-2 py-0.5 font-bold uppercase">
                        DUAL
                      </span>
                    </div>
                    <h3 className="text-lg md:text-xl font-bold font-display text-[var(--card-text)] mb-2 uppercase tracking-wide">
                      {game.name}
                    </h3>
                    <p className="text-xs text-[var(--card-desc)] line-clamp-2">
                      {game.description}
                    </p>
                  </div>
                  <div className="mt-4 flex items-center justify-between">
                    <span className={`px-2 py-0.5 rounded text-[10px] font-bold text-black ${
                      game.difficulty === 'easy' ? 'bg-[#10b981]' : game.difficulty === 'medium' ? 'bg-[#fbbf24]' : 'bg-[#ef4444]'
                    }`}>
                      DIFF: {game.difficulty.toUpperCase()}
                    </span>
                    <span className="font-mono text-xs font-bold text-[#00f0ff] hover:underline">
                      LOAD_SIM →
                    </span>
                  </div>
                </button>
              ))}
            </div>
          </div>
        )}
      </div>
    </div>
  );
};

export default GameMenu;
