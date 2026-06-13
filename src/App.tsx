import React, { useEffect, useState } from 'react';
import { useGame } from './context/GameContext';
import { useAuth } from './context/AuthContext';
import { useSocket } from './context/SocketContext';
import AuthForm from './components/AuthForm';
import GameMenu from './components/GameMenu';
import PlayerSetup from './components/PlayerSetup';
import GameBoard from './components/GameBoard';
import Settings from './components/Settings';
import { AudioManager } from './games/audio';
import { Difficulty } from './types';
import { GAMES } from './games';

const App: React.FC = () => {
  const { currentGame, setCurrentGame, players, setPlayers, theme, setTheme, setDifficulty } = useGame();
  const { user, token, loading, logout, matchHistory } = useAuth();
  const { activeRoom, joinRoom, onlineUsers } = useSocket();
  const [settingsOpen, setSettingsOpen] = useState(false);
  const [sidebarOpen, setSidebarOpen] = useState(true);
  const [isMuted, setIsMuted] = useState(AudioManager.isMuted);
  const [activeView, setActiveView] = useState<'dashboard' | 'arena' | 'history' | 'stats' | 'friends' | 'inventory'>('arena');
  const [globalLeaderboard, setGlobalLeaderboard] = useState<any[]>([]);
  const [autoJoinAttempted, setAutoJoinAttempted] = useState(false);

  // Real Friends feature states
  const [friends, setFriends] = useState<any[]>([]);
  const [friendUsernameInput, setFriendUsernameInput] = useState('');
  const [friendError, setFriendError] = useState<string | null>(null);
  const [friendSuccess, setFriendSuccess] = useState<string | null>(null);

  // Auto-join room from URL query param or sessionStorage (set before login)
  useEffect(() => {
    if (user && !autoJoinAttempted) {
      const params = new URLSearchParams(window.location.search);
      const roomId = params.get('roomId') || sessionStorage.getItem('pendingRoomId');
      if (roomId) {
        setAutoJoinAttempted(true);
        setActiveView('arena');
        joinRoom(roomId);
        sessionStorage.removeItem('pendingRoomId');
        window.history.replaceState({}, document.title, window.location.pathname);
      }
    }
  }, [user, autoJoinAttempted, joinRoom]);

  useEffect(() => {
    if (activeView === 'stats') {
      fetch(`${(import.meta.env.VITE_API_URL as string) || 'http://localhost:5000'}/api/leaderboard`)
        .then(res => res.json())
        .then(data => setGlobalLeaderboard(data))
        .catch(err => console.error("Error fetching leaderboard:", err));
    }
  }, [activeView]);

  const fetchFriends = async () => {
    if (!token) return;
    try {
      const res = await fetch(`${(import.meta.env.VITE_API_URL as string) || 'http://localhost:5000'}/api/friends`, {
        headers: {
          Authorization: `Bearer ${token}`
        }
      });
      if (res.ok) {
        const data = await res.json();
        setFriends(data);
      }
    } catch (err) {
      console.error("Error fetching friends:", err);
    }
  };

  useEffect(() => {
    if (token) {
      fetchFriends();
    } else {
      setFriends([]);
    }
  }, [token]);

  useEffect(() => {
    if (activeView === 'friends' || activeView === 'dashboard') {
      fetchFriends();
    }
  }, [activeView]);

  const handleAddFriend = async (e: React.FormEvent) => {
    e.preventDefault();
    if (!friendUsernameInput.trim() || !token) return;
    setFriendError(null);
    setFriendSuccess(null);

    try {
      const res = await fetch(`${(import.meta.env.VITE_API_URL as string) || 'http://localhost:5000'}/api/friends/add`, {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
          Authorization: `Bearer ${token}`
        },
        body: JSON.stringify({ username: friendUsernameInput.trim() })
      });
      const data = await res.json();
      if (res.ok) {
        setFriendSuccess(`Successfully connected with ${data.friend.username}!`);
        setFriends(prev => [...prev, data.friend]);
        setFriendUsernameInput('');
      } else {
        setFriendError(data.error || 'Failed to add friend.');
      }
    } catch (err) {
      setFriendError('Network error. Please try again.');
    }
  };

  const handleRemoveFriend = async (friendId: string) => {
    if (!token) return;
    if (!confirm("Are you sure you want to delete this direct-link?")) return;

    try {
      const res = await fetch(`${(import.meta.env.VITE_API_URL as string) || 'http://localhost:5000'}/api/friends/remove`, {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
          Authorization: `Bearer ${token}`
        },
        body: JSON.stringify({ friendId })
      });
      if (res.ok) {
        setFriends(prev => prev.filter(f => f.id !== friendId));
      }
    } catch (err) {
      console.error("Error removing friend:", err);
    }
  };

  // Synchronize activeRoom with GameContext for online multiplayer
  useEffect(() => {
    if (activeRoom && activeRoom.status === 'active') {
      const roomGame = GAMES[activeRoom.gameType];
      if (roomGame && (!currentGame || currentGame.id !== roomGame.id)) {
        setCurrentGame(roomGame);
      }

      const host = activeRoom.players.find(p => p.symbol === "1");
      const guest = activeRoom.players.find(p => p.symbol === "2");
      
      if (host && guest) {
        const gamePlayers = [
          {
            id: 1,
            name: host.username,
            symbol: roomGame?.id === 'snakesladders' ? '🔴' : 'X',
            type: 'human' as const,
            score: 0,
            avatar: host.avatar
          },
          {
            id: 2,
            name: guest.username,
            symbol: roomGame?.id === 'snakesladders' ? '🔵' : 'O',
            type: 'human' as const,
            score: 0,
            avatar: guest.avatar
          }
        ];
        
        if (!players || players.length !== 2 || players[0].name !== host.username || players[1].name !== guest.username) {
          setPlayers(gamePlayers);
        }
      }
    }
  }, [activeRoom, currentGame, players, setCurrentGame, setPlayers]);

  useEffect(() => {
    document.documentElement.setAttribute('data-theme', theme);
  }, [theme]);

  // Sync settings and start procedural background music
  useEffect(() => {
    try {
      const saved = localStorage.getItem('boardGamesSettings');
      if (saved) {
        const parsed = JSON.parse(saved);
        if (parsed.theme) setTheme(parsed.theme);
        if (parsed.difficulty) setDifficulty(parsed.difficulty as Difficulty);
        if (parsed.sound !== undefined) {
          AudioManager.setMuted(!parsed.sound);
          setIsMuted(!parsed.sound);
        }
      }
    } catch {}
    
    // Launch ambient synthesizer sequence
    AudioManager.startMusic();

    return () => {
      AudioManager.stopMusic();
    };
  }, []);

  const toggleTheme = () => {
    AudioManager.playClick();
    setTheme(theme === 'light' ? 'dark' : 'light');
  };

  const handleMuteToggle = () => {
    const newMuted = AudioManager.toggle();
    setIsMuted(newMuted);
  };

  // If loading user profile, show premium loading screen
  if (loading) {
    return (
      <div className="min-h-screen flex items-center justify-center bg-[#070b13] font-mono text-white">
        <div className="text-center">
          <div className="w-12 h-12 border-4 border-[#00f0ff] border-t-transparent rounded-full animate-spin mx-auto mb-4"></div>
          <div className="text-xs font-bold uppercase tracking-widest text-[#00f0ff] animate-pulse">
            ESTABLISHING SECURE CONNECTION LINK.SYS...
          </div>
        </div>
      </div>
    );
  }

  // If not logged in, preserve ?roomId in sessionStorage and show Auth form
  if (!user) {
    const params = new URLSearchParams(window.location.search);
    const pendingRoom = params.get('roomId');
    if (pendingRoom) {
      sessionStorage.setItem('pendingRoomId', pendingRoom);
      window.history.replaceState({}, document.title, window.location.pathname);
    }
    return <AuthForm />;
  }

  const currentUsername = user.username;

  // Calculate dynamic level & XP based on authenticated user data
  const totalXp = user.xp;
  const playerLevel = user.level;
  const currentXpInLevel = totalXp % 500;
  const xpPercentage = Math.min(100, Math.max(0, (currentXpInLevel / 500) * 100));

  const getLevelTitle = (lvl: number) => {
    if (lvl === 1) return 'BRONZE APPRENTICE';
    if (lvl === 2) return 'SILVER RECRUIT';
    if (lvl === 3) return 'GOLD INITIATE';
    if (lvl === 4) return 'PLATINUM SPECIALIST';
    if (lvl === 5) return 'DIAMOND ENFORCER';
    return 'CYBER GRANDMASTER';
  };

  // Mock data for Dashboard
  const dailyQuests = [
    { id: 1, title: 'DECRYPT CELL NODES', desc: 'Scan 20 safety zones in Minesweeper', xp: '+150 XP', progress: 0.65, completed: false },
    { id: 2, title: 'TACTICAL MATE', desc: 'Perform checkmate in Grandmaster Chess', xp: '+300 XP', progress: 0.0, completed: false },
    { id: 3, title: 'COMPRESS DATA', desc: 'Slide grid to 1024 packet size in 2048', xp: '+200 XP', progress: 1.0, completed: true },
    { id: 4, title: 'NEURAL CLIMBER', desc: 'Ascend 2 ladders in Snakes & Ladders', xp: '+100 XP', progress: 0.5, completed: false },
  ];

  // Real friends list state is used instead of mock friendsList.

  const marketItems = [
    { id: 1, name: 'Plasma Edge', type: 'LEGENDARY', cost: '450 NP', desc: 'Custom laser pointer cell highlight skin for Chess.', rarityColor: '#fbbf24', avatar: '⚔️' },
    { id: 2, name: 'Volt Reactor', type: 'RARE', cost: '1200 NP', desc: 'Animated background grid particle skin for 2048.', rarityColor: '#d946ef', avatar: '⚡' },
    { id: 3, name: 'Grip Master', type: 'UNCOMMON', cost: '300 NP', desc: 'Anti-slip tactile cell styling pack for Sudoku.', rarityColor: '#00f0ff', avatar: '🛡️' },
    { id: 4, name: 'Shard Core', type: 'LEGENDARY', cost: '1500 NP', desc: 'Crystalline skin set for Solitaire card stack decks.', rarityColor: '#fbbf24', avatar: '💎' },
  ];

  const renderActiveView = () => {
    switch (activeView) {
      case 'dashboard':
        return (
          <div className="space-y-8 animate-fade-in">
            {/* Active Expedition Campaign Banner */}
            <div className="cyber-panel neon-glow-border-cyan p-8 relative overflow-hidden bg-gradient-to-r from-[#0d1527] to-[#121c38]">
              <div className="absolute top-0 right-0 bg-[#00f0ff] text-black font-mono px-3 py-1 text-[10px] border-b border-l border-white/10 font-bold uppercase tracking-widest">
                PLAYER_RANK
              </div>
              <h2 className="text-[#00f0ff] font-mono text-xs font-bold tracking-widest mb-1 uppercase">LEVEL {playerLevel} // {getLevelTitle(playerLevel)}</h2>
              <h1 className="text-3xl font-black font-display text-white uppercase tracking-wide mb-2">{currentUsername}</h1>
              <p className="text-xs text-[#94a3b8] font-mono mb-6 uppercase tracking-wider">XP PROGRESSION: {currentXpInLevel} / 500 XP TO LEVEL UP</p>
              
              <div className="flex flex-col sm:flex-row gap-4 items-center justify-between">
                <div className="w-full max-w-md bg-black/40 border border-white/5 h-4 rounded-full overflow-hidden p-0.5">
                  <div className="bg-[#00f0ff] h-full rounded-full pulse-neon-cyan" style={{ width: `${xpPercentage}%` }}></div>
                </div>
                <button 
                  onClick={() => { AudioManager.playClick(); setActiveView('arena'); }}
                  className="cyber-btn bg-[#00f0ff] text-black px-6 py-2.5 text-xs font-bold uppercase hover:scale-105 transition"
                >
                  ENGAGE PROTOCOL →
                </button>
              </div>
            </div>

            {/* Daily Quests and Friends */}
            <div className="grid grid-cols-1 lg:grid-cols-2 gap-8">
              {/* Daily Quests Widget */}
              <div className="cyber-panel p-6 border border-white/5 bg-[var(--card-bg)]">
                <div className="border-b border-white/10 pb-3 mb-5 flex justify-between items-center">
                  <h3 className="font-bold font-display text-sm text-white uppercase tracking-wider">DAILY_QUESTS.SYS</h3>
                  <span className="text-[10px] font-mono text-[#fbbf24] uppercase">RESET IN 14H</span>
                </div>
                <div className="space-y-4">
                  {dailyQuests.map(q => (
                    <div key={q.id} className="cyber-panel p-3 border border-white/5 bg-black/20 flex items-center justify-between">
                      <div className="flex-1 mr-4">
                        <div className="flex items-center gap-2 mb-1">
                          <span className={`text-[10px] font-mono font-bold ${q.completed ? 'text-[#10b981]' : 'text-[#fbbf24]'}`}>
                            {q.completed ? 'COMPLETED' : 'IN_PROGRESS'}
                          </span>
                          <span className="text-xs font-bold text-white uppercase tracking-wide">{q.title}</span>
                        </div>
                        <p className="text-[10px] text-[#94a3b8] mb-2">{q.desc}</p>
                        <div className="w-full bg-black/40 h-1.5 rounded-full overflow-hidden">
                          <div className={`h-full rounded-full ${q.completed ? 'bg-[#10b981]' : 'bg-[#fbbf24]'}`} style={{ width: `${q.progress * 100}%` }}></div>
                        </div>
                      </div>
                      <span className="text-xs font-mono font-bold text-[#00f0ff] shrink-0">{q.xp}</span>
                    </div>
                  ))}
                </div>
              </div>

              {/* Friends list Widget */}
              <div className="cyber-panel p-6 border border-white/5 bg-[var(--card-bg)]">
                <div className="border-b border-white/10 pb-3 mb-5 flex justify-between items-center">
                  <h3 className="font-bold font-display text-sm text-white uppercase tracking-wider">ACTIVE_LOBBY.SYS</h3>
                  <span className="text-[10px] font-mono text-[#00f0ff] uppercase">
                    {friends.filter(f => onlineUsers.includes(f.id)).length} ONLINE
                  </span>
                </div>
                <div className="space-y-4">
                  {friends.length === 0 ? (
                    <div className="text-center py-4 font-mono text-[10px] text-[#94a3b8] uppercase font-bold">
                      No active connections.<br />Add friends in the Friends tab.
                    </div>
                  ) : (
                    friends.map((f) => {
                      const isOnline = onlineUsers.includes(f.id);
                      const color = isOnline ? '#00f0ff' : '#94a3b8';
                      return (
                        <div key={f.id} className="flex items-center justify-between p-2.5 hover:bg-white/5 rounded transition">
                          <div className="flex items-center gap-3">
                            <div className="w-8 h-8 rounded-full border border-white/10 bg-black/30 flex items-center justify-center font-mono font-bold text-xs uppercase" style={{ color }}>
                              {f.avatar || f.username.substring(0, 2)}
                            </div>
                            <div>
                              <div className="text-xs font-bold text-white uppercase tracking-wide">{f.username}</div>
                              <div className="text-[9px] text-[#94a3b8] font-mono uppercase mt-0.5">LVL {f.level} // {f.xp} XP</div>
                            </div>
                          </div>
                          <span className="text-[9px] font-mono px-2 py-0.5 rounded border font-bold" style={{ borderColor: `${color}33`, color }}>
                            {isOnline ? 'ONLINE' : 'OFFLINE'}
                          </span>
                        </div>
                      );
                    })
                  )}
                </div>
              </div>
            </div>

            {/* Trending Marketplace */}
            <div className="cyber-panel p-6 border border-white/5 bg-[var(--card-bg)]">
              <div className="border-b border-white/10 pb-3 mb-5 flex justify-between items-center">
                <h3 className="font-bold font-display text-sm text-white uppercase tracking-wider">TRENDING_IN_MARKET.SYS</h3>
                <span className="text-[10px] font-mono text-[#d946ef] uppercase">REFRESHING_ITEMS</span>
              </div>
              <div className="grid grid-cols-1 sm:grid-cols-2 lg:grid-cols-4 gap-6">
                {marketItems.map(item => (
                  <div key={item.id} className="cyber-panel p-4 border border-white/5 bg-black/10 flex flex-col justify-between h-44 hover:scale-[1.02] transition">
                    <div>
                      <div className="flex justify-between items-start mb-2">
                        <span className="text-2xl">{item.avatar}</span>
                        <span className="text-[8px] font-mono font-bold px-1.5 py-0.5 rounded border uppercase" style={{ borderColor: `${item.rarityColor}33`, color: item.rarityColor }}>
                          {item.type}
                        </span>
                      </div>
                      <h4 className="text-xs font-bold text-white uppercase tracking-wider mb-1">{item.name}</h4>
                      <p className="text-[9px] text-[#94a3b8] line-clamp-2">{item.desc}</p>
                    </div>
                    <div className="mt-4 flex items-center justify-between border-t border-white/5 pt-3">
                      <span className="text-xs font-mono font-bold text-[#00f0ff]">{item.cost}</span>
                      <button className="text-[9px] font-mono font-bold text-black bg-white px-2.5 py-1 rounded hover:bg-[#00f0ff] transition uppercase">
                        ACQUIRE
                      </button>
                    </div>
                  </div>
                ))}
              </div>
            </div>
          </div>
        );

      case 'arena':
        return (
          <div className="animate-fade-in">
            {/* Joining via link — room joined but game not started yet */}
            {activeRoom && activeRoom.status === 'waiting' && !players && (
              <div className="min-h-screen flex items-center justify-center">
                <div className="cyber-panel neon-glow-border-cyan max-w-md w-full p-10 text-center space-y-5 bg-[var(--card-bg)]">
                  <div className="w-12 h-12 border-4 border-[#d946ef] border-t-transparent rounded-full animate-spin mx-auto"></div>
                  <p className="text-sm font-bold font-mono text-[#d946ef] uppercase tracking-widest">JOINING ROOM...</p>
                  <p className="text-[10px] font-mono text-[#94a3b8] uppercase">
                    Room: {activeRoom.roomId.substring(0,8).toUpperCase()}...
                  </p>
                  <p className="text-[10px] font-mono text-white/40 uppercase">Waiting for host to be ready</p>
                </div>
              </div>
            )}
            {(!activeRoom || activeRoom.status !== 'waiting' || players) && (
              !currentGame ? <GameMenu /> : !players ? <PlayerSetup /> : <GameBoard />
            )}
          </div>
        );

      case 'history':
        return (
          <div className="space-y-6 animate-fade-in">
            <div className="cyber-panel p-6 border border-white/5 bg-[var(--card-bg)]">
              <div className="border-b border-white/10 pb-3 mb-6">
                <h3 className="font-bold font-display text-sm text-white uppercase tracking-wider">MATCH_HISTORY_LOG.SYS</h3>
              </div>

              {matchHistory.length === 0 ? (
                <div className="py-20 text-center">
                  <span className="text-5xl">📡</span>
                  <h4 className="text-sm font-bold text-white uppercase tracking-widest mt-4">NO ARCHIVE RECORDS FOUND</h4>
                  <p className="text-xs text-[#94a3b8] font-mono uppercase mt-2">Initialize simulations in the Arena to record game files.</p>
                  <button 
                    onClick={() => { AudioManager.playClick(); setActiveView('arena'); }}
                    className="cyber-btn bg-[#00f0ff] text-black px-6 py-2 mt-6 text-xs font-bold uppercase"
                  >
                    ENTER SIMULATION ARENA
                  </button>
                </div>
              ) : (
                <div className="space-y-4">
                  {matchHistory.map((entry, _i) => {
                    const isWin = entry.winner === currentUsername;
                    const isDraw = entry.winner === null;
                    return (
                      <div key={entry.id} className="cyber-panel p-4 border border-white/5 bg-black/10 flex flex-col sm:flex-row items-start sm:items-center justify-between gap-4">
                        <div className="flex items-center gap-4">
                          <span className="text-2xl">🏆</span>
                          <div>
                            <div className="text-xs font-bold text-white uppercase tracking-wider">{entry.gameType.toUpperCase()}</div>
                            <div className="text-[10px] text-[#94a3b8] font-mono uppercase mt-0.5">
                              {entry.player1.avatar} {entry.player1.username} VS {entry.player2.avatar} {entry.player2.username}
                            </div>
                          </div>
                        </div>

                        <div className="flex items-center gap-6 self-stretch sm:self-auto justify-between sm:justify-start border-t sm:border-t-0 border-white/5 pt-3 sm:pt-0">
                          <div className="text-right">
                            <span className="text-[9px] text-[#94a3b8] font-mono block uppercase">{new Date(entry.startedAt).toLocaleDateString()}</span>
                            <span className="text-xs font-mono font-bold text-white block mt-0.5">
                              WINNER: {entry.winner ? entry.winner.toUpperCase() : 'DRAW'}
                            </span>
                          </div>

                          <div className="shrink-0">
                            {isDraw ? (
                              <span className="px-3 py-1 rounded text-[10px] font-mono font-bold bg-[#fbbf24]/10 text-[#fbbf24] border border-[#fbbf24]/20 uppercase">
                                DRAW +50 XP
                              </span>
                            ) : isWin ? (
                              <span className="px-3 py-1 rounded text-[10px] font-mono font-bold bg-[#10b981]/10 text-[#10b981] border border-[#10b981]/20 uppercase">
                                WIN +150 XP
                              </span>
                            ) : (
                              <span className="px-3 py-1 rounded text-[10px] font-mono font-bold bg-[#ef4444]/10 text-[#ef4444] border border-[#ef4444]/20 uppercase">
                                LOSS +30 XP
                              </span>
                            )}
                          </div>
                        </div>
                      </div>
                    );
                  })}
                </div>
              )}
            </div>
          </div>
        );

      case 'stats':
        return (
          <div className="space-y-6 animate-fade-in">
            <div className="cyber-panel p-6 border border-white/5 bg-[var(--card-bg)]">
              <div className="border-b border-white/10 pb-3 mb-6">
                <h3 className="font-bold font-display text-sm text-white uppercase tracking-wider">SYSTEM_LEADERBOARD_RECORDS</h3>
                <p className="text-[10px] font-mono text-[#94a3b8] uppercase mt-1">Cross-agent scores rank database</p>
              </div>

              {globalLeaderboard.length === 0 ? (
                <div className="py-20 text-center">
                  <span className="text-5xl">🏆</span>
                  <h4 className="text-sm font-bold text-white uppercase tracking-widest mt-4">LEADERBOARD IS VACANT</h4>
                  <p className="text-xs text-[#94a3b8] font-mono uppercase mt-2">Rank index updates will load when matches are saved.</p>
                </div>
              ) : (
                <div className="overflow-x-auto">
                  <table className="w-full text-left font-mono text-xs border-collapse">
                    <thead>
                       <tr className="border-b border-white/10 text-[#94a3b8] uppercase">
                        <th className="py-3 px-4">RANK</th>
                        <th className="py-3 px-4">AGENT</th>
                        <th className="py-3 px-4">LEVEL</th>
                        <th className="py-3 px-4">XP</th>
                        <th className="py-3 px-4">RECORD</th>
                      </tr>
                    </thead>
                    <tbody>
                      {globalLeaderboard.map((entry, idx) => (
                        <tr key={entry.id} className="border-b border-white/5 hover:bg-white/5 transition">
                          <td className="py-3 px-4 font-bold text-[#fbbf24]">#{idx + 1}</td>
                          <td className="py-3 px-4 text-white font-bold flex items-center gap-2">
                            <span className="text-base">{entry.avatar}</span>
                            <span className="uppercase tracking-wider">{entry.username}</span>
                          </td>
                          <td className="py-3 px-4 uppercase font-bold text-[#d946ef]">LVL {entry.level}</td>
                          <td className="py-3 px-4 text-[#00f0ff] font-bold font-mono">{entry.xp} XP</td>
                          <td className="py-3 px-4 font-mono font-bold text-[#94a3b8]">
                            {entry.wins}W - {entry.losses}L - {entry.draws}D
                          </td>
                        </tr>
                      ))}
                    </tbody>
                  </table>
                </div>
              )}
            </div>
          </div>
        );

      case 'friends':
        return (
          <div className="space-y-6 animate-fade-in">
            {/* Add Friend Form */}
            <div className="cyber-panel p-6 border border-white/5 bg-[var(--card-bg)]">
              <div className="border-b border-white/10 pb-3 mb-4">
                <h3 className="font-bold font-display text-sm text-white uppercase tracking-wider">ADD_NEW_CONNECTION</h3>
                <p className="text-[10px] font-mono text-[#94a3b8] uppercase mt-1 font-bold">Establish direct-link with another active agent username</p>
              </div>
              <form onSubmit={handleAddFriend} className="flex gap-3">
                <input
                  type="text"
                  placeholder="ENTER AGENT CODENAME..."
                  value={friendUsernameInput}
                  onChange={(e) => setFriendUsernameInput(e.target.value)}
                  className="cyber-input px-4 py-2 bg-black/40 border border-white/10 text-white font-mono text-xs rounded focus:outline-none focus:border-[#00f0ff] transition flex-grow"
                />
                <button
                  type="submit"
                  className="px-5 py-2 bg-[#d946ef] text-white font-mono text-xs font-bold rounded hover:bg-[#c084fc] transition uppercase"
                >
                  CONNECT_AGENT
                </button>
              </form>
              {friendError && (
                <div className="text-red-500 font-mono text-[10px] mt-2 uppercase font-bold">⚠️ ERROR: {friendError}</div>
              )}
              {friendSuccess && (
                <div className="text-emerald-500 font-mono text-[10px] mt-2 uppercase font-bold">✓ SUCCESS: {friendSuccess}</div>
              )}
            </div>

            {/* Friends List */}
            <div className="cyber-panel p-6 border border-white/5 bg-[var(--card-bg)]">
              <div className="border-b border-white/10 pb-3 mb-6">
                <h3 className="font-bold font-display text-sm text-white uppercase tracking-wider">ALL_COMMUNICATIONS_LOG</h3>
                <p className="text-[10px] font-mono text-[#94a3b8] uppercase mt-1 font-bold">Secure direct-link connections with other agents</p>
              </div>

              {friends.length === 0 ? (
                <div className="text-center py-8 font-mono text-xs text-[#94a3b8] uppercase font-bold">
                  No active connections. Search and add agents above.
                </div>
              ) : (
                <div className="grid grid-cols-1 md:grid-cols-2 gap-6">
                  {friends.map((f) => {
                    const isOnline = onlineUsers.includes(f.id);
                    const color = isOnline ? '#00f0ff' : '#94a3b8';
                    return (
                      <div key={f.id} className="cyber-panel p-4 border border-white/5 bg-black/10 flex items-center justify-between">
                        <div className="flex items-center gap-3">
                          <div className="w-10 h-10 rounded-full border border-white/10 bg-black/30 flex items-center justify-center font-mono font-bold text-sm uppercase" style={{ color }}>
                            {f.avatar || f.username.substring(0, 2)}
                          </div>
                          <div>
                            <div className="text-xs font-bold text-white uppercase tracking-wide">{f.username}</div>
                            <div className="text-[10px] text-[#94a3b8] font-mono uppercase mt-0.5">
                              LVL {f.level} // {f.xp} XP
                            </div>
                            <div className="text-[8px] text-white/50 font-mono mt-0.5">
                              RECORD: {f.wins}W - {f.losses}L - {f.draws}D
                            </div>
                          </div>
                        </div>
                        <div className="flex flex-col items-end gap-2">
                          <span className="text-[8px] font-mono px-2 py-0.5 rounded border font-bold" style={{ borderColor: `${color}33`, color }}>
                            {isOnline ? 'ONLINE' : 'OFFLINE'}
                          </span>
                          <button
                            onClick={() => handleRemoveFriend(f.id)}
                            className="text-[8px] font-mono font-bold text-red-400 border border-red-500/20 hover:bg-red-500/10 px-2 py-1 rounded transition uppercase"
                          >
                            TERMINATE LINK
                          </button>
                        </div>
                      </div>
                    );
                  })}
                </div>
              )}
            </div>
          </div>
        );

      case 'inventory':
        return (
          <div className="space-y-6 animate-fade-in">
            <div className="cyber-panel p-6 border border-white/5 bg-[var(--card-bg)]">
              <div className="border-b border-white/10 pb-3 mb-6">
                <h3 className="font-bold font-display text-sm text-white uppercase tracking-wider">DIGITAL_INVENTORY_SYSTEM</h3>
                <p className="text-[10px] font-mono text-[#94a3b8] uppercase mt-1">Unlocked skins, card decks, and audio themes</p>
              </div>

              <div className="grid grid-cols-2 sm:grid-cols-3 lg:grid-cols-4 gap-6">
                {/* Equipped items */}
                <div className="cyber-panel p-4 border border-[#10b981]/30 bg-black/20 relative">
                  <span className="absolute top-0 right-0 bg-[#10b981] text-black font-mono text-[8px] px-1.5 py-0.5 rounded-bl font-bold uppercase">
                    EQUIPPED
                  </span>
                  <div className="text-3xl mb-2">⚡</div>
                  <h4 className="text-xs font-bold text-white uppercase tracking-wider">Volt Reactor Skin</h4>
                  <p className="text-[9px] text-[#94a3b8] mt-1 font-mono">2048 SYSTEM TILE SKIN</p>
                </div>

                <div className="cyber-panel p-4 border border-[#10b981]/30 bg-black/20 relative">
                  <span className="absolute top-0 right-0 bg-[#10b981] text-black font-mono text-[8px] px-1.5 py-0.5 rounded-bl font-bold uppercase">
                    EQUIPPED
                  </span>
                  <div className="text-3xl mb-2">🎴</div>
                  <h4 className="text-xs font-bold text-white uppercase tracking-wider">Plasma Card Frame</h4>
                  <p className="text-[9px] text-[#94a3b8] mt-1 font-mono">SOLITAIRE CARDS THEME</p>
                </div>

                <div className="cyber-panel p-4 border border-[#10b981]/30 bg-black/20 relative">
                  <span className="absolute top-0 right-0 bg-[#10b981] text-black font-mono text-[8px] px-1.5 py-0.5 rounded-bl font-bold uppercase">
                    EQUIPPED
                  </span>
                  <div className="text-3xl mb-2">🎲</div>
                  <h4 className="text-xs font-bold text-white uppercase tracking-wider">Neon Dice Set</h4>
                  <p className="text-[9px] text-[#94a3b8] mt-1 font-mono">SNAKES & LADDERS SKIN</p>
                </div>

                {/* Locked items */}
                {Array.from({ length: 9 }).map((_, i) => (
                  <div key={i} className="cyber-panel p-4 border border-white/5 bg-black/40 flex flex-col items-center justify-center text-center opacity-40">
                    <span className="text-xl">🔒</span>
                    <h4 className="text-[10px] font-bold text-[#94a3b8] uppercase tracking-wider mt-2">Locked Slot 0{i + 4}</h4>
                    <p className="text-[8px] text-[#94a3b8] mt-0.5 font-mono">LOCKED_IN_DATABASE</p>
                  </div>
                ))}
              </div>
            </div>
          </div>
        );
    }
  };

  return (
    <div className="min-h-screen flex flex-col md:flex-row bg-[var(--bg-gradient)] text-white">
      {/* Left Navigation Sidebar */}
      <aside className={`${sidebarOpen ? 'w-full md:w-64 p-6 border-b md:border-b-0 md:border-r' : 'w-0 h-0 overflow-hidden p-0 border-0 flex-none'} bg-[#0a0c12] border-white/5 flex flex-col justify-between shrink-0 z-40 transition-all duration-300`}>
        <div className="space-y-8">
          {/* Logo & Profile Header */}
          <div className="flex items-center gap-3 border-b border-white/5 pb-5">
            <div className="w-10 h-10 rounded-full border-2 border-[#00f0ff] bg-black/40 shrink-0 flex items-center justify-center text-xl">
              {user.avatar || "👾"}
            </div>
            <div>
              <div className="text-xs font-bold uppercase tracking-wider text-white truncate max-w-[130px]">{currentUsername}</div>
              <div className="flex items-center gap-1.5 mt-0.5">
                <span className="w-1.5 h-1.5 rounded-full bg-[#10b981] animate-pulse"></span>
                <span className="text-[9px] font-mono text-[#94a3b8] uppercase">LVL {playerLevel} // {getLevelTitle(playerLevel)}</span>
              </div>
            </div>
          </div>

          {/* Navigation Links */}
          <nav className="space-y-1">
            <button
              onClick={() => { AudioManager.playClick(); setActiveView('dashboard'); }}
              className={`w-full flex items-center gap-3 px-4 py-2.5 rounded-lg text-xs font-bold uppercase tracking-wider transition ${
                activeView === 'dashboard' ? 'active-nav-item' : 'text-[#94a3b8] hover:text-white hover:bg-white/5'
              }`}
            >
              <span className="text-sm">🏠</span> Dashboard
            </button>
            <button
              onClick={() => { AudioManager.playClick(); setActiveView('arena'); }}
              className={`w-full flex items-center gap-3 px-4 py-2.5 rounded-lg text-xs font-bold uppercase tracking-wider transition ${
                activeView === 'arena' ? 'active-nav-item' : 'text-[#94a3b8] hover:text-white hover:bg-white/5'
              }`}
            >
              <span className="text-sm">🎮</span> Arena
            </button>
            <button
              onClick={() => { AudioManager.playClick(); setActiveView('history'); }}
              className={`w-full flex items-center gap-3 px-4 py-2.5 rounded-lg text-xs font-bold uppercase tracking-wider transition ${
                activeView === 'history' ? 'active-nav-item' : 'text-[#94a3b8] hover:text-white hover:bg-white/5'
              }`}
            >
              <span className="text-sm">🕒</span> History
            </button>
            <button
              onClick={() => { AudioManager.playClick(); setActiveView('stats'); }}
              className={`w-full flex items-center gap-3 px-4 py-2.5 rounded-lg text-xs font-bold uppercase tracking-wider transition ${
                activeView === 'stats' ? 'active-nav-item' : 'text-[#94a3b8] hover:text-white hover:bg-white/5'
              }`}
            >
              <span className="text-sm">🏆</span> Stats
            </button>
            <button
              onClick={() => { AudioManager.playClick(); setActiveView('friends'); }}
              className={`w-full flex items-center gap-3 px-4 py-2.5 rounded-lg text-xs font-bold uppercase tracking-wider transition ${
                activeView === 'friends' ? 'active-nav-item' : 'text-[#94a3b8] hover:text-white hover:bg-white/5'
              }`}
            >
              <span className="text-sm">👥</span> Friends
            </button>
            <button
              onClick={() => { AudioManager.playClick(); setActiveView('inventory'); }}
              className={`w-full flex items-center gap-3 px-4 py-2.5 rounded-lg text-xs font-bold uppercase tracking-wider transition ${
                activeView === 'inventory' ? 'active-nav-item' : 'text-[#94a3b8] hover:text-white hover:bg-white/5'
              }`}
            >
              <span className="text-sm">📦</span> Inventory
            </button>
          </nav>
        </div>

        {/* Start Queue button */}
        <div className="mt-8 pt-5 border-t border-white/5 space-y-4">
          <button
            onClick={() => { AudioManager.playClick(); setActiveView('arena'); }}
            className="w-full py-2.5 rounded-lg text-xs font-bold font-mono uppercase bg-gradient-to-r from-[#d946ef] to-[#00f0ff] text-black shadow-lg hover:scale-105 active:scale-95 transition tracking-widest cursor-pointer text-center block"
          >
            START QUEUE
          </button>
          <div className="flex gap-2 text-[10px] font-mono text-[#94a3b8] justify-center">
            <span className="hover:text-white cursor-pointer">SUPPORT</span>
            <span>•</span>
            <span 
              onClick={() => { AudioManager.playClick(); logout(); }}
              className="hover:text-white cursor-pointer hover:underline"
            >
              LOGOUT
            </span>
          </div>
        </div>
      </aside>

      {/* Main Content Area */}
      <div className="flex-1 flex flex-col min-h-0">
        {/* Global Top Header Bar */}
        <header className="h-16 border-b border-white/5 bg-[#0a0c12] px-6 md:px-8 flex items-center justify-between z-30 shrink-0">
          <div className="flex items-center gap-4">
            <button
              onClick={() => { AudioManager.playClick(); setSidebarOpen(!sidebarOpen); }}
              className="text-white hover:text-[#00f0ff] focus:outline-none flex items-center justify-center p-1.5 rounded hover:bg-white/5 transition"
              title="Toggle Sidebar"
            >
              <svg className="w-5 h-5" fill="none" stroke="currentColor" viewBox="0 0 24 24" xmlns="http://www.w3.org/2000/svg">
                <path strokeLinecap="round" strokeLinejoin="round" strokeWidth={2.5} d="M4 6h16M4 12h16M4 18h16" />
              </svg>
            </button>
            <span className="text-base font-bold font-mono tracking-widest text-[#00f0ff]">NEON_REALM</span>
            <span className="text-xs font-mono text-[#94a3b8] hidden sm:inline">({activeView.toUpperCase()})</span>
          </div>

          <div className="flex items-center gap-3">
            {/* System Audio Toggle */}
            <button
              onClick={handleMuteToggle}
              className={`w-8 h-8 rounded border flex items-center justify-center text-xs font-mono transition font-bold shrink-0 ${
                isMuted 
                  ? 'border-[#ef4444]/20 text-[#ef4444] bg-[#ef4444]/5 hover:bg-[#ef4444]/10' 
                  : 'border-[#10b981]/20 text-[#10b981] bg-[#10b981]/5 hover:bg-[#10b981]/10'
              }`}
              title={isMuted ? 'UNMUTE AUDIO' : 'MUTE AUDIO'}
            >
              {isMuted ? '🔇' : '🔊'}
            </button>

            {/* Dark/Light mode toggle */}
            <button
              onClick={toggleTheme}
              className="w-8 h-8 rounded border border-white/10 text-white flex items-center justify-center text-xs hover:bg-white/5 transition font-bold shrink-0"
              title={theme === 'light' ? 'SWITCH TO DARK MODE' : 'SWITCH TO LIGHT MODE'}
            >
              {theme === 'light' ? '🌙' : '☀️'}
            </button>

            {/* Settings trigger gear */}
            <button
              onClick={() => { AudioManager.playClick(); setSettingsOpen(true); }}
              className="w-8 h-8 rounded border border-white/10 text-white flex items-center justify-center text-xs hover:bg-white/5 transition font-bold shrink-0"
              title="SYS CONFIGURATION"
            >
              ⚙️
            </button>

            {/* Notifications Bell */}
            <button
              className="w-8 h-8 rounded border border-white/10 text-white flex items-center justify-center text-xs hover:bg-white/5 transition font-bold relative shrink-0"
              title="NOTIFICATIONS"
            >
              🔔
              <span className="absolute top-1 right-1 w-2 h-2 rounded-full bg-[#d946ef] animate-pulse"></span>
            </button>
          </div>
        </header>

        {/* Dynamic Inner Tab panel */}
        <main className="flex-1 overflow-y-auto p-6 md:p-8">
          {renderActiveView()}
        </main>
      </div>

      {/* Settings Modal */}
      <Settings isOpen={settingsOpen} onClose={() => setSettingsOpen(false)} onSoundChange={(muted) => setIsMuted(muted)} />
    </div>
  );
};

export default App;
