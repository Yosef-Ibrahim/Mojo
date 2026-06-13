# 🌌 CYBER_ARENA.SYS — FCAI Board Games Suite

[![React](https://img.shields.io/badge/React-20232A?style=for-the-badge&logo=react&logoColor=61DAFB)](https://reactjs.org/)
[![Node.js](https://img.shields.io/badge/Node.js-339933?style=for-the-badge&logo=nodedotjs&logoColor=white)](https://nodejs.org/)
[![Express](https://img.shields.io/badge/Express-000000?style=for-the-badge&logo=express&logoColor=white)](https://expressjs.com/)
[![Socket.io](https://img.shields.io/badge/Socket.io-010101?style=for-the-badge&logo=socketdotio&logoColor=white)](https://socket.io/)
[![Prisma](https://img.shields.io/badge/Prisma-2D3748?style=for-the-badge&logo=prisma&logoColor=white)](https://www.prisma.io/)
[![SQLite](https://img.shields.io/badge/SQLite-07405E?style=for-the-badge&logo=sqlite&logoColor=white)](https://sqlite.org/)

A premium, full-stack, real-time multiplayer board games suite wrapped in a high-fidelity cyberpunk/sci-fi interface. This project transitions the classic Cairo University CS213 C++ board games collection into a modern web-based arena featuring user accounts, real-time matchmaking, live statistics, and a persistent friends lobby.

---

## 🚀 Key Features

*   **🔒 Secure Agent Authentication**: Registration & login using JWT (JSON Web Tokens) and cryptographically secure passwords (`bcryptjs`).
*   **🎭 Custom Avatar Module**: Distinct cybernetic avatars representing user profiles in the arena.
*   **🌐 Real-Time Multiplayer Lobby**: Private room hosting with instant invitation links (`?roomId=...`) for playing with friends.
*   **🤖 Smart Game Engine**: Custom turn-gating, real-time board sync over WebSockets, and automatic forfeit/disconnection win assignment.
*   **👥 Real-Time Friends List**: Add/remove friends using their unique Agent Codenames. Displays real-time **Online/Offline** badges via active WebSocket sessions.
*   **📊 Persistent Stats & Leaderboard**: Global leaderboard ranking players by XP and wins. Player levels dynamically calculated from XP points.
*   **🔊 Immersive Sci-Fi UI**: Synthwave/cyberpunk aesthetics, premium animations, customizable themes (Dark/Light/Neon), and ambient procedural synthesizer sound effects.

---

## 🛠️ Technology Stack

### Frontend (Client)
*   **Framework**: React (TypeScript) + Vite
*   **Real-time Communication**: `socket.io-client`
*   **Routing**: State-based client router with URL parameter auto-join
*   **Aesthetics**: Custom Vanilla CSS with Cyber-Panel components and glassmorphism

### Backend (Server)
*   **Framework**: Express.js (TypeScript) + Node.js
*   **Database ORM**: Prisma ORM
*   **Database Engine**: SQLite (Zero-Configuration, file-based)
*   **Sockets**: `socket.io` for event-driven real-time coordination
*   **Security**: `jsonwebtoken` (JWT) + `bcryptjs` (password hashing)

---

## 📁 Repository Structure

```
Board_Games-Assignment_3/
├── src/                      # Frontend source code (React + Vite)
│   ├── components/           # UI Components (Lobby, Auth, GameBoard, Settings)
│   ├── context/              # Global React States (Auth, Sockets, Game Context)
│   ├── games/                # Client-side game configurations & audio assets
│   ├── styles/               # Cyberpunk global CSS styling system
│   └── App.tsx               # Main application layout and view controller
│
├── server/                   # Backend source code (Express + Node.js)
│   ├── src/
│   │   ├── game/             # In-memory Matchmaking & Room Management
│   │   ├── middleware/       # JWT Auth verification middleware
│   │   ├── routes/           # REST API Endpoints (Auth, Friends, Leaderboard)
│   │   └── index.ts          # Main HTTP & Socket Server entrypoint
│   ├── prisma/               # Database Schemas & Migrations
│   └── package.json          # Backend dependencies
│
├── Board_Game_A3/            # [Legacy] Original C++ Console Version
└── README.md                 # This documentation file
```

---

## ⚙️ Environment Variables

Before running, configure your environment variables:

### Backend (`server/.env`)
```env
DATABASE_URL="file:./dev.db"
PORT=5000
JWT_SECRET="your_secure_cyber_secret_key"
FRONTEND_URL="https://mojo-three-teal.vercel.app" # Your deployed client URL (optional, defaults to allow all)
```

### Frontend (Root `.env.local` for local development)
```env
VITE_API_URL="http://localhost:5000"
```

---

## 💻 Local Setup Guide

### Prerequisites
*   Node.js (v18 or newer)
*   npm (v9 or newer)

### 1. Launch the Backend Server
```bash
# Navigate to the server folder
cd server

# Install dependencies
npm install

# Run database migrations to initialize SQLite
npx prisma migrate deploy

# Start the server in development mode (watches for changes)
npm run dev
```
> Server will boot on **http://localhost:5000**. You should see:  
> `🚀 Cyber Gaming Arena Server running on port 5000`

### 2. Launch the Frontend Client (New Terminal)
```bash
# Navigate back to the root folder
cd ..

# Install dependencies
npm install

# Start the React frontend
npm run dev
```
> Client will boot on **http://localhost:5173**. Open this URL in your browser to begin!

---

## ☁️ Deployment Guidelines

### Frontend (Vercel)
1.  Connect your GitHub repository to **Vercel**.
2.  Add the following **Environment Variable** in the Vercel project settings:
    *   `VITE_API_URL` = `<your-deployed-backend-url>` (e.g. `https://your-backend.onrender.com` or `https://your-backend.up.railway.app`)
3.  Deploy/Redeploy the project to bake the environment variable into the production build.

### Backend (Render / Koyeb / Railway)
1.  Deploy the server using your preferred cloud provider.
2.  Configure the environment variables in the provider dashboard:
    *   `DATABASE_URL` = `file:./dev.db`
    *   `JWT_SECRET` = `some_cyberpunk_secret_key`
    *   `PORT` = `5000`
3.  Set the **Build Command** to:
    ```bash
    cd server && npm install && npx prisma generate && npx prisma migrate deploy && npm run build
    ```
4.  Set the **Start Command** to:
    ```bash
    cd server && node dist/index.js
    ```

---

## 🛡️ License

This project was built for academic evaluation under Cairo University, Faculty of Computers and Artificial Intelligence.  
**Course**: CS213 — Object Oriented Programming.  
**Instructors**: Cairo University FCAI Faculty.