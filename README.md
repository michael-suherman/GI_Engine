# 🃏 Genshin Impact TCG Engine

A personal project to simulate and evaluate positions in *Genshin Impact's Trading Card Game (GITCG)* — combining my love for the game with my passion for optimization and decision-making AI.

---

## 🎯 Project Goal

To build an engine that:

- **Simulates GITCG mechanics**, including complex interactions like elemental reactions, character statuses, and action sequencing.
- **Evaluates board states** and determines the best move, inspired by chess engines like *Stockfish*.

---

## 💡 Motivation

I've always been fascinated by how chess engines calculate optimal moves by looking **30+ plies** ahead. I wanted to bring that level of depth and precision to a game I love — GITCG — even though it's significantly more complex due to:

- 🌀 **Asymmetrical turns**: Players can end their turn early, allowing the opponent to chain multiple actions.
- 🧪 **Complex mechanics**: Elemental reactions, dice randomness, combat statuses, and summon timers all affect decision-making.
- 🎲 **Hidden information**: Unlike chess, you can't see your opponent’s hand or deck — introducing **probabilistic reasoning**.

Despite these challenges, I wanted to see how far I could go in modeling the game and exploring engine logic — a mix of game simulation, algorithm design, and AI-inspired search.

---

## 🔧 Features (In Progress)

- ✅ Turn system with action-chaining and pass mechanics  
- ✅ Character switching, dice spending, and elemental reactions  
- ✅ Statuses: Frozen, Stun, Shield, Burning, etc.  
- ⚙️ Minimax engine with position evaluation (WIP)  
- 📊 Probabilistic reasoning (planned)

---

## 🚧 Challenges

> "From the start, I knew I wouldn't be able to reach perfect play like Stockfish — but I took on this project to challenge myself."

- Implementing **partial information** (opponent’s hand/deck)  
- Designing a **heuristic evaluation function** for highly dynamic board states  
- Simulating turns where players don't alternate cleanly  
- Modeling **game-ending conditions** with interrupts and bursts

---
