# Multiplayer Backend & Database Implementation Plan

This plan outlines the roadmap to transition the local React board games suite into a real-time multiplayer application with user authentication, profile statistics, and invite link sharing.

---

## 📋 TODO Checklist

### Phase 1: Server Initialization & Setup
- [x] Create `server/` directory and initialize `package.json`
- [x] Install Express, Socket.io, TypeScript, ts-node, nodemon, cors, dotenv
- [x] Configure `tsconfig.json` for the server
- [x] Set up the entry point (`src/index.ts`) with a basic HTTP server and CORS configuration
- [x] Verify server runs locally (`npm run dev`)

### Phase 2: Database & Prisma ORM Setup
- [x] Install Prisma CLI and client (`@prisma/client`)
- [x] Initialize Prisma (`npx prisma init`)
- [x] Define the schemas in `schema.prisma`:
  - `User` (id, email, username, passwordHash, avatar, xp, level, wins, losses, draws)
  - `Match` (id, gameType, status, player1, player2, winner, startedAt, endedAt, movesJson)
- [x] Configure SQLite database connection string in `.env` (Zero-Setup SQLite)
- [x] Run initial migrations (`npx prisma migrate dev`)

### Phase 3: Auth & Profile API Endpoints
- [x] Install `bcryptjs`, `jsonwebtoken`, and their `@types`
- [x] Implement middleware for JWT authentication (`authMiddleware.ts`)
- [x] Create API routes:
  - [x] `POST /api/auth/register` (user registration with hashed password)
  - [x] `POST /api/auth/login` (generate JWT for authenticated sessions)
  - [x] `GET /api/auth/profile` (fetch current user stats, level, history)
  - [x] `GET /api/leaderboard` (fetch global top players sorted by XP/wins)

### Phase 4: Socket.io Real-Time Game Engine
- [x] Set up Socket.io connection handlers
- [x] Implement in-memory room store logic:
  - `rooms` map: `roomId -> ActiveRoomState`
- [x] Implement WebSocket event handlers:
  - [x] `room:create` (Generate UUID room, assign player 1 role)
  - [x] `room:join` (Join UUID room, assign player 2 role, broadcast start state)
  - [x] `game:move` (Receive move, validate, update state, broadcast to room)
  - [x] `game:resign` / `disconnect` (Handle forfeit, update DB, announce winner)

### Phase 5: Frontend Integration & Link Sharing ← **COMPLETE**
- [x] Install `socket.io-client` in the React frontend
- [x] Create `SocketContext.tsx` (socket client, room state, create/join/sendMove)
- [x] Create `AuthContext.tsx` (JWT login/register, profile fetch, match history)
- [x] Wire `AuthProvider` + `SocketProvider` in `main.tsx`
- [x] Show `AuthForm` when user is not logged in (`App.tsx`)
- [x] Display user profile stats from backend in sidebar (`App.tsx`)
- [x] **Online Lobby UI** — LOCAL/ONLINE tabs in `PlayerSetup.tsx`; creates room, displays shareable URL, shows waiting spinner
- [x] **Auto-join via URL** — reads `?roomId=xxx` from URL on load, or falls back to `sessionStorage` (for users who weren't logged in yet)
- [x] **Online GameBoard bridge** — `isMyTurn` gate blocks opponent moves; `sendMove()` fires after each local action; LIVE banner shows turn status; forfeit detection banner

### Phase 6: Testing & Verification ← **NEXT STEP**
- [ ] Test User registration and login flow end-to-end
- [ ] Verify profile stats and leaderboard updates after completed games
- [ ] Open two browser windows to test real-time synchronization of dice rolling, piece movements, and turn cycling
- [ ] Verify connection recovery and handling of sudden disconnections
- [x] `npm run build` → **✓ Built successfully in 10s — 335KB JS, 45KB CSS, 0 errors**

---

## 🚀 How to Run the Program

### Step 1 — Start the Backend Server
```bash
cd "d:\games\Board_Games-Assignment_3 web\server"
npm run dev
```
> Server starts on **http://localhost:5000**
> You should see: `🚀 Cyber Gaming Arena Server running on port 5000`

### Step 2 — Start the Frontend (new terminal)
```bash
cd "d:\games\Board_Games-Assignment_3 web"
npm run dev
```
> Frontend starts on **http://localhost:5173**

### Step 3 — First Use
1. Open `http://localhost:5173` in your browser
2. Register a new account (email, username, password)
3. You land directly in the **Arena** (games page)
4. Pick any game → Click **"Play with Friend (Online)"** → Copy the share link → Open in another browser window → paste the link → both players join the same room → game starts!

### Step 4 — Sharing Links
- Host room → A URL like `http://localhost:5173/?roomId=abc-123-xyz` is generated
- Send this URL to your friend → they open it, log in, and are automatically joined to your room

---

## ⚠️ What's Still Missing (Next Actions)

| # | Task | File(s) | Priority |
|---|------|---------|----------|
| 1 | Online Lobby Modal (share link UI) | `PlayerSetup.tsx` | 🔴 High |
| 2 | Auto-join room from URL `?roomId=` | `App.tsx` | 🔴 High |
| 3 | Online turn gating in GameBoard | `GameBoard.tsx` | 🔴 High |
| 4 | Show forfeit/game-over socket events | `GameBoard.tsx` | 🟡 Medium |
| 5 | Full E2E test (2 windows) | manual | 🟡 Medium |
