import express from "express";
import http from "http";
import { Server } from "socket.io";
import cors from "cors";
import dotenv from "dotenv";

dotenv.config();

const app = express();
const server = http.createServer(app);

const allowedOrigins = process.env.FRONTEND_URL
  ? [process.env.FRONTEND_URL, "http://localhost:5173"]
  : ["*"];

const io = new Server(server, {
  cors: {
    origin: allowedOrigins,
    methods: ["GET", "POST"],
  },
});

app.use(cors({
  origin: allowedOrigins.includes("*") ? "*" : (origin: string | undefined, cb: Function) => {
    if (!origin || allowedOrigins.includes(origin)) cb(null, true);
    else cb(new Error("Not allowed by CORS"));
  }
}));
app.use(express.json());

import authRoutes from "./routes/authRoutes";

app.use("/api/auth", authRoutes);
app.use("/api", authRoutes);

// Health check endpoint
app.get("/health", (req, res) => {
  res.json({ status: "healthy", timestamp: new Date() });
});

import { roomManager } from "./game/roomManager";

// Real-time communication socket connection handler
io.on("connection", (socket) => {
  console.log(`[Socket] User connected: ${socket.id}`);

  // 1. Create room
  socket.on("room:create", ({ userId, username, avatar, gameType }) => {
    const room = roomManager.createRoom(socket.id, userId, username, avatar, gameType);
    socket.join(room.roomId);
    socket.emit("room:created", room);
  });

  // 2. Join room
  socket.on("room:join", ({ userId, username, avatar, roomId }) => {
    const room = roomManager.joinRoom(socket.id, userId, username, avatar, roomId);
    if (!room) {
      socket.emit("room:error", { message: "Room not found, finished, or already full." });
      return;
    }

    socket.join(roomId);
    socket.emit("room:joined", room);
    
    // Broadcast updated room state (both players present now)
    io.to(roomId).emit("room:update", room);
    
    // Broadcast start game event to both players
    io.to(roomId).emit("game:start", room);
  });

  // 3. Make move
  socket.on("game:move", async ({ roomId, userId, board, moves, nextPlayerId, isGameOver, winnerSymbol }) => {
    const result = await roomManager.makeMove(roomId, userId, board, moves, nextPlayerId, isGameOver, winnerSymbol);
    if (!result) return; // invalid turn or room

    // Broadcast move update to both players in the room
    io.to(roomId).emit("game:update", result.room);

    if (isGameOver) {
      io.to(roomId).emit("game:over", result.room);
    }
  });

  // 4. Handle player disconnection
  socket.on("disconnect", async () => {
    console.log(`[Socket] User disconnected: ${socket.id}`);
    const result = await roomManager.handleDisconnect(socket.id);
    if (result) {
      // Notify the remaining player that they won by forfeit
      io.to(result.remainingPlayerSocket).emit("game:forfeit", result.room);
    }
  });
});

const PORT = process.env.PORT || 5000;
server.listen(PORT, () => {
  console.log(`===============================================`);
  console.log(`🚀 Cyber Gaming Arena Server running on port ${PORT}`);
  console.log(`===============================================`);
});
