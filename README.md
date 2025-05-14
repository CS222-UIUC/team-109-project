# Team 109: HunchAI BlackJack
A C++ adaptation of the classic six-deck Blackjack game featuring an AI opponent that combines statistical analysis with intuitive decisions.

# Team 109 Members

| Name | Role | Contributions |
|------|------|--------------|
| Affan Faisal | C++ Game Server Developer | Core game logic implementation |
| Advait Yadav | Python AI Bot & Static Server Developer | Monte Carlo simulation, Flask server |
| Numair Hajyani | Frontend Developer | React UI, Tailwind CSS implementation |
| Amr Elayyan | C++ Game Server Developer | Game state management |

# Project summary
HunchAI BlackJack is a modern implementation of the classic casino game with a unique twist. Unlike traditional Blackjack AI opponents that rely solely on statistical optimization, our system incorporates a "Hunch Module" that occasionally makes gut-feel plays instead of the mathematically optimal move.

Key features include:
Standard six-deck Blackjack gameplay
Web-based user interface
AI opponent with dual decision-making layers:
- **Statistical Engine:** Calculates mathematically optimal plays based on deck composition
- **Hunch Module:** Introduces deviations from purely statistical decisions
HTTP server for web UI connectivity

### Why HunchAI BlackJack?

Our project was motivated by several goals:
- Combine our passions for game design and artificial intelligence
- Create an unpredictable opponent that mimics human decision-making
- Study player responses to unconventional AI moves
- Enhance our skills in C++, REST API design, and machine learning integration

## Technical Architecture

The application consists of four main components:

### 1. Frontend (React)
- Serves as the user interface where players interact with the game
- Built with React, Tailwind CSS, and Framer Motion
- Provides intuitive controls for game actions (hit, stand, etc.)

### 2. Static Server (Python/Flask)
- Hosts and delivers the compiled React application
- Connects the AI Bot to the frontend
- Manages HTTP requests and responses between components

### 3. C++ Game Server
- Implements the core Blackjack logic
- Maintains game state
- Processes player actions and determines outcomes
- Uses the nlohmann/json library for API communication

### 4. Python AI Bot Server
- Runs Monte Carlo simulations to suggest optimal moves
- Maintains a memory system that records games in each session to learn from past decisions


# Installation Instructions
**Prerequisites**

C++17 compatible compiler (GCC 8+, Clang 7+, or MSVC 19.14+)

Python 3.8+

Node.js 14+

npm 6+

**Clone the Repository**

git clone https://github.com/CS222-UIUC/team-109-project

**Install C++ Dependencies**

We use the following libraries:

httplib - HTTP server

nlohmann/json - JSON handling

### Run the Servers

In three separate terminals:

**Terminal 1 (C++ Game Server):**

Build c++ server using make

./blackjack_server

**Terminal 2 (Python AI Bot Server):**

python server.py

**Terminal 3 (Frontend Static Server):**

cd blackjack-ui

npm start

### **Play the Game**

Open your browser and navigate to: 

http://localhost:3000
