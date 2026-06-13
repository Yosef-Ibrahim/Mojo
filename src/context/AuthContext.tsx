import React, { createContext, useContext, useState, useEffect, ReactNode } from "react";

const API_BASE_URL = `${(import.meta.env.VITE_API_URL as string) || "http://localhost:5000"}/api`;

export interface User {
  id: string;
  email: string;
  username: string;
  avatar: string;
  xp: number;
  level: number;
  wins: number;
  losses: number;
  draws: number;
}

export interface MatchHistoryEntry {
  id: string;
  gameType: string;
  status: string;
  player1: { username: string; avatar: string };
  player2: { username: string; avatar: string };
  winner: string | null;
  startedAt: string;
  endedAt: string;
}

interface AuthContextType {
  user: User | null;
  token: string | null;
  loading: boolean;
  matchHistory: MatchHistoryEntry[];
  login: (identifier: string, password: string) => Promise<{ success: boolean; error?: string }>;
  register: (email: string, username: string, password: string, avatar?: string) => Promise<{ success: boolean; error?: string }>;
  logout: () => void;
  refreshProfile: () => Promise<void>;
}

const AuthContext = createContext<AuthContextType | undefined>(undefined);

export const AuthProvider: React.FC<{ children: ReactNode }> = ({ children }) => {
  const [user, setUser] = useState<User | null>(null);
  const [token, setToken] = useState<string | null>(localStorage.getItem("cyber_arena_token"));
  const [matchHistory, setMatchHistory] = useState<MatchHistoryEntry[]>([]);
  const [loading, setLoading] = useState<boolean>(true);

  // Initialize and fetch profile if token exists
  useEffect(() => {
    const initAuth = async () => {
      if (token) {
        try {
          const res = await fetch(`${API_BASE_URL}/auth/profile`, {
            headers: {
              Authorization: `Bearer ${token}`,
            },
          });
          const data = await res.json();
          if (res.ok) {
            setUser(data.user);
            setMatchHistory(data.matchHistory);
          } else {
            // Token expired or invalid
            logout();
          }
        } catch (err) {
          console.error("Failed to load user profile:", err);
        }
      }
      setLoading(false);
    };

    initAuth();
  }, [token]);

  const login = async (identifier: string, password: string) => {
    try {
      const res = await fetch(`${API_BASE_URL}/auth/login`, {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ identifier, password }),
      });
      const data = await res.json();
      if (res.ok) {
        localStorage.setItem("cyber_arena_token", data.token);
        setToken(data.token);
        setUser(data.user);
        return { success: true };
      } else {
        return { success: false, error: data.error || "Login failed" };
      }
    } catch (err) {
      return { success: false, error: "Network error. Please try again." };
    }
  };

  const register = async (email: string, username: string, password: string, avatar?: string) => {
    try {
      const res = await fetch(`${API_BASE_URL}/auth/register`, {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ email, username, password, avatar }),
      });
      const data = await res.json();
      if (res.ok) {
        localStorage.setItem("cyber_arena_token", data.token);
        setToken(data.token);
        setUser(data.user);
        return { success: true };
      } else {
        return { success: false, error: data.error || "Registration failed" };
      }
    } catch (err) {
      return { success: false, error: "Network error. Please try again." };
    }
  };

  const logout = () => {
    localStorage.removeItem("cyber_arena_token");
    setToken(null);
    setUser(null);
    setMatchHistory([]);
  };

  const refreshProfile = async () => {
    if (!token) return;
    try {
      const res = await fetch(`${API_BASE_URL}/auth/profile`, {
        headers: {
          Authorization: `Bearer ${token}`,
        },
      });
      const data = await res.json();
      if (res.ok) {
        setUser(data.user);
        setMatchHistory(data.matchHistory);
      }
    } catch (err) {
      console.error("Failed to refresh profile:", err);
    }
  };

  return (
    <AuthContext.Provider
      value={{
        user,
        token,
        loading,
        matchHistory,
        login,
        register,
        logout,
        refreshProfile,
      }}
    >
      {children}
    </AuthContext.Provider>
  );
};

export const useAuth = () => {
  const context = useContext(AuthContext);
  if (context === undefined) {
    throw new Error("useAuth must be used within an AuthProvider");
  }
  return context;
};
