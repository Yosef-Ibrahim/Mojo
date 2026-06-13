import React, { useEffect, useState } from 'react';
import { useGame } from '../context/GameContext';
import { AudioManager } from '../games/audio';
import { Difficulty } from '../types';

interface SettingsProps {
  isOpen: boolean;
  onClose: () => void;
  onSoundChange?: (muted: boolean) => void;
}

const Settings: React.FC<SettingsProps> = ({ isOpen, onClose, onSoundChange }) => {
  const { theme, setTheme, difficulty, setDifficulty } = useGame();
  const [soundOn, setSoundOn] = useState(!AudioManager.isMuted);
  const [localTheme, setLocalTheme] = useState(theme);
  const [localDifficulty, setLocalDifficulty] = useState(difficulty);

  useEffect(() => {
    if (isOpen) {
      setLocalTheme(theme);
      setLocalDifficulty(difficulty);
      setSoundOn(!AudioManager.isMuted);
    }
  }, [isOpen, theme, difficulty]);

  if (!isOpen) return null;

  const apply = () => {
    if (localTheme !== theme) setTheme(localTheme);
    if (localDifficulty !== difficulty) setDifficulty(localDifficulty);
    
    const newMuted = !soundOn;
    AudioManager.setMuted(newMuted);
    if (onSoundChange) onSoundChange(newMuted);

    try {
      localStorage.setItem('boardGamesSettings', JSON.stringify({
        theme: localTheme,
        difficulty: localDifficulty,
        sound: soundOn,
      }));
    } catch {}
    AudioManager.playClick();
    onClose();
  };

  const difficulties: { value: Difficulty; label: string; color: string }[] = [
    { value: 'easy', label: 'EASY', color: '#10b981' },
    { value: 'medium', label: 'MEDIUM', color: '#fbbf24' },
    { value: 'hard', label: 'HARD', color: '#ef4444' },
  ];

  return (
    <div className="fixed inset-0 z-[100] flex items-center justify-center bg-black/75 backdrop-blur-sm animate-fade-in p-6">
      <div
        className="brutal-card shadow-brutal-pink p-8 w-full max-w-sm relative bg-[#131722]"
      >
        <div className="absolute top-0 right-0 bg-[#fbbf24] text-black font-mono px-3 py-0.5 text-xs border-b-2 border-l-2 border-black font-bold uppercase">
          CFG_SYS
        </div>

        <h2 className="text-2xl font-black font-display mb-8 text-center text-white tracking-wide uppercase">
          SETTINGS
        </h2>

        <div className="space-y-6">
          {/* Theme Segment */}
          <div>
            <label className="block text-xs font-mono font-bold text-white mb-2 uppercase">Theme Protocol</label>
            <div className="flex gap-2">
              {(['light', 'dark'] as const).map((t) => (
                <button
                  key={t}
                  onClick={() => setLocalTheme(t)}
                  className={`flex-1 py-2 font-bold font-mono text-xs uppercase brutal-border transition ${
                    localTheme === t 
                      ? 'bg-[#00f0ff] text-black shadow-brutal-black' 
                      : 'bg-[#161a23] text-white hover:bg-black/45'
                  }`}
                >
                  {t === 'light' ? 'LIGHT' : 'DARK'}
                </button>
              ))}
            </div>
          </div>

          {/* AI Difficulty Segment */}
          <div>
            <label className="block text-xs font-mono font-bold text-white mb-2 uppercase">AI Complexity</label>
            <div className="flex gap-2">
              {difficulties.map((d) => (
                <button
                  key={d.value}
                  onClick={() => setLocalDifficulty(d.value)}
                  className={`flex-1 py-2 font-bold font-mono text-xs uppercase brutal-border transition ${
                    localDifficulty === d.value 
                      ? 'bg-[#fbbf24] text-black shadow-brutal-black' 
                      : 'bg-[#161a23] text-white hover:bg-[#1f2937]'
                  }`}
                >
                  {d.label}
                </button>
              ))}
            </div>
          </div>

          {/* Sound Segment */}
          <div>
            <label className="block text-xs font-mono font-bold text-white mb-2 uppercase">Sound Systems</label>
            <button
              onClick={() => setSoundOn(!soundOn)}
              className={`w-full py-2 font-bold font-mono text-xs uppercase brutal-border transition ${
                soundOn 
                  ? 'bg-[#d946ef] text-black shadow-brutal-black' 
                  : 'bg-[#161a23] text-white hover:bg-black/45'
              }`}
            >
              {soundOn ? 'SOUND_ON' : 'SOUND_MUTED'}
            </button>
          </div>
        </div>

        {/* Buttons Segment */}
        <div className="flex gap-4 mt-8 pt-4 border-t border-black/40">
          <button
            onClick={() => { AudioManager.playClick(); onClose(); }}
            className="flex-1 brutal-btn bg-[#ef4444] text-white py-2 text-xs font-bold font-mono uppercase"
          >
            CANCEL
          </button>
          <button
            onClick={apply}
            className="flex-1 brutal-btn bg-[#00f0ff] text-black py-2 text-xs font-bold font-mono uppercase"
          >
            APPLY
          </button>
        </div>
      </div>
    </div>
  );
};

export default Settings;