import React, { useState } from "react";
import { useAuth } from "../context/AuthContext";
import { AudioManager } from "../games/audio";

const AVATARS = ["👾", "⚔️", "🛡️", "⚡", "💎", "🚀", "👑", "🛸", "🤖", "🌀", "🔥", "🔮"];

const AuthForm: React.FC = () => {
  const { login, register } = useAuth();
  const [isLogin, setIsLogin] = useState(true);
  
  // Fields
  const [email, setEmail] = useState("");
  const [username, setUsername] = useState("");
  const [password, setPassword] = useState("");
  const [avatar, setAvatar] = useState("👾");
  
  const [error, setError] = useState<string | null>(null);
  const [loading, setLoading] = useState(false);

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault();
    setError(null);
    setLoading(true);
    AudioManager.playClick();

    if (isLogin) {
      const result = await login(username, password);
      if (!result.success) {
        setError(result.error || "Login failed");
        setLoading(false);
      }
    } else {
      if (!email) {
        setError("Email is required");
        setLoading(false);
        return;
      }
      const result = await register(email, username, password, avatar);
      if (!result.success) {
        setError(result.error || "Registration failed");
        setLoading(false);
      }
    }
  };

  const toggleMode = () => {
    AudioManager.playClick();
    setIsLogin(!isLogin);
    setError(null);
  };

  return (
    <div className="min-h-screen flex items-center justify-center bg-[#070b13] bg-[radial-gradient(ellipse_at_center,_var(--tw-gradient-stops))] from-[#0f1931] via-[#070b13] to-[#030509] p-4 font-mono select-none">
      {/* Scanline overlay for cyber effect */}
      <div className="absolute inset-0 pointer-events-none bg-[linear-gradient(rgba(18,16,16,0)_50%,_rgba(0,0,0,0.25)_50%),_linear-gradient(90deg,_rgba(255,0,0,0.06),_rgba(0,255,0,0.02),_rgba(0,0,255,0.06))] bg-[size:100%_4px,_6px_100%] opacity-20"></div>

      <div className="w-full max-w-md cyber-panel neon-glow-border-cyan bg-[#0d1527]/90 backdrop-blur-md p-8 relative overflow-hidden border border-[#00f0ff]/30 shadow-[0_0_30px_rgba(0,240,255,0.1)]">
        {/* Decorative corner tag */}
        <div className="absolute top-0 right-0 bg-[#00f0ff] text-black font-bold px-3 py-1 text-[9px] uppercase tracking-widest">
          SYSTEM_ACCESS
        </div>

        {/* Title */}
        <div className="text-center mb-8">
          <h1 className="text-3xl font-black font-display text-white tracking-wider mb-2 drop-shadow-[0_0_10px_rgba(255,255,255,0.2)]">
            CYBER_ARENA<span className="text-[#00f0ff]">.SYS</span>
          </h1>
          <p className="text-xs text-[#94a3b8] uppercase tracking-widest">
            {isLogin ? "INITIALIZE NEURAL CONNECT" : "REGISTER NEW AGENT ID"}
          </p>
        </div>

        {/* Error Alert */}
        {error && (
          <div className="cyber-panel border border-[#ef4444]/30 bg-[#ef4444]/10 text-[#f87171] p-3 text-xs mb-6 text-center tracking-wide uppercase animate-pulse">
            ⚠️ ERROR: {error}
          </div>
        )}

        {/* Form */}
        <form onSubmit={handleSubmit} className="space-y-6">
          {!isLogin && (
            <div className="space-y-2">
              <label className="text-[10px] text-[#00f0ff] font-bold uppercase tracking-wider block">
                EMAIL_ADDRESS
              </label>
              <input
                type="email"
                required
                value={email}
                onChange={(e) => setEmail(e.target.value)}
                placeholder="agent@nexus.net"
                className="w-full bg-black/40 border border-white/10 rounded px-4 py-2 text-sm text-white focus:outline-none focus:border-[#00f0ff] transition"
              />
            </div>
          )}

          <div className="space-y-2">
            <label className="text-[10px] text-[#00f0ff] font-bold uppercase tracking-wider block">
              {isLogin ? "USERNAME_OR_EMAIL" : "CHOOSE_CODENAME"}
            </label>
            <input
              type="text"
              required
              value={username}
              onChange={(e) => setUsername(e.target.value)}
              placeholder="e.g. AGENT_ALPHA"
              className="w-full bg-black/40 border border-white/10 rounded px-4 py-2 text-sm text-white focus:outline-none focus:border-[#00f0ff] transition"
            />
          </div>

          <div className="space-y-2">
            <label className="text-[10px] text-[#00f0ff] font-bold uppercase tracking-wider block">
              ACCESS_KEYPASSWORD
            </label>
            <input
              type="password"
              required
              value={password}
              onChange={(e) => setPassword(e.target.value)}
              placeholder="••••••••"
              className="w-full bg-black/40 border border-white/10 rounded px-4 py-2 text-sm text-white focus:outline-none focus:border-[#00f0ff] transition"
            />
          </div>

          {!isLogin && (
            <div className="space-y-3">
              <label className="text-[10px] text-[#d946ef] font-bold uppercase tracking-wider block">
                SELECT_AVATAR_MODULE
              </label>
              <div className="grid grid-cols-6 gap-2 bg-black/30 p-3 rounded border border-white/5">
                {AVATARS.map((emoji) => (
                  <button
                    key={emoji}
                    type="button"
                    onClick={() => { AudioManager.playClick(); setAvatar(emoji); }}
                    className={`text-xl p-2 rounded transition flex items-center justify-center hover:bg-white/5 ${
                      avatar === emoji
                        ? "bg-[#d946ef]/20 border border-[#d946ef] shadow-[0_0_10px_rgba(217,70,239,0.3)]Scale-110"
                        : "border border-transparent"
                    }`}
                  >
                    {emoji}
                  </button>
                ))}
              </div>
            </div>
          )}

          {/* Action Button */}
          <button
            type="submit"
            disabled={loading}
            className={`w-full cyber-btn py-3 text-xs font-bold uppercase tracking-widest transition duration-300 ${
              isLogin
                ? "bg-[#00f0ff] text-black hover:shadow-[0_0_15px_rgba(0,240,255,0.4)]"
                : "bg-[#d946ef] text-white hover:shadow-[0_0_15px_rgba(217,70,239,0.4)]"
            } ${loading ? "opacity-50 cursor-not-allowed" : ""}`}
          >
            {loading ? "INITIALIZING SECURE LINK..." : isLogin ? "AUTHORIZE ACCESS →" : "REGISTER AGENT →"}
          </button>
        </form>

        {/* Toggle Mode */}
        <div className="mt-8 text-center border-t border-white/10 pt-4">
          <button
            onClick={toggleMode}
            className="text-[10px] text-[#94a3b8] hover:text-white uppercase tracking-wider transition"
          >
            {isLogin ? "CREATE NEW AGENT CREDENTIALS [SIGN UP]" : "EXISTING AGENT? RETRIEVE CONNECTION [LOG IN]"}
          </button>
        </div>
      </div>
    </div>
  );
};

export default AuthForm;
