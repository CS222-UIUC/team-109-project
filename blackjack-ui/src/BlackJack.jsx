import React, { useState } from "react";
import Card from './components/Card';

const BlackJack = () => {
    const [playerHand, setPlayerHand] = useState([]);
    const [dealerHand, setDealerHand] = useState([]);
    const [gameState, setGameState] = useState("waiting");
    const [message, setMessage] = useState("Click Start Game to begin!");
    const [aiSuggestion, setAiSuggestion] = useState('');

    // API interaction (C++ server now on port 5002)
    const BASE_URL = "http://localhost:5002";
    const AI_BOT_URL = "http://localhost:5001";

    const startGame = async () => {
        try {
            setAiSuggestion('');
            setMessage('Starting game...');
            const response = await fetch(`${BASE_URL}/start`, {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({}),
            });
            if (!response.ok) throw new Error(`HTTP error! status: ${response.status}`);
            const data = await response.json();

            setPlayerHand(data.player);
            setDealerHand(data.dealer);
            setGameState(data.gameState);
            setMessage(data.gameState === 'playing'
                ? "Game started! Your turn."
                : `Game Over: ${data.gameState}`);
        } catch (error) {
            console.error("Error starting game:", error);
            setMessage(`Error starting game: ${error.message}. Is the backend running?`);
            setGameState("error");
        }
    };

    const hit = async () => {
        try {
            setAiSuggestion('');
            setMessage('Hitting...');
            const response = await fetch(`${BASE_URL}/hit`, {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({}),
            });
            if (!response.ok) throw new Error(`HTTP error! status: ${response.status}`);
            const data = await response.json();

            setPlayerHand(data.player);
            setDealerHand(data.dealer);
            setGameState(data.gameState);
            
            if (data.gameState === 'playing') {
                setMessage("Your turn.");
            } else {
                // Game over after hit (player busted)
                setMessage(`Game Over: ${data.gameState}`);
            }
        } catch (error) {
            console.error("Error hitting:", error);
            setMessage(`Error hitting: ${error.message}`);
            setGameState("error");
        }
    };

    const stand = async () => {
        try {
            setAiSuggestion('');
            setMessage('Standing... Dealer plays.');
            const response = await fetch(`${BASE_URL}/stand`, {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({}),
            });
            if (!response.ok) throw new Error(`HTTP error! status: ${response.status}`);
            const data = await response.json();

            setDealerHand(data.dealer);
            setGameState(data.gameState);
            setMessage(`Game Over: ${data.gameState}`);
        } catch (error) {
            console.error("Error standing:", error);
            setMessage(`Error standing: ${error.message}`);
            setGameState("error");
        }
    };

    const getAiHint = async () => {
        if (gameState !== 'playing' || !playerHand.length) {
            setAiSuggestion('Hint not available now.');
            return;
        }
        try {
            setAiSuggestion('Getting hint...');
            const response = await fetch(`${AI_BOT_URL}/api/bot_decision`, {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ playerHand }),
            });
            if (!response.ok) {
                const errorData = await response.json().catch(() => ({ error: 'Unknown error' }));
                throw new Error(`${response.status} - ${errorData.error}`);
            }
            const data = await response.json();
            setAiSuggestion(data.decision
                ? `AI Suggests: ${data.decision.toUpperCase()}`
                : 'Received unclear suggestion.');
        } catch (error) {
            console.error("Error getting AI hint:", error);
            setAiSuggestion(`Error getting hint: ${error.message}`);
        }
    };

    // Check if the game is in a terminal state
    const isGameOver = () => {
        return gameState !== "playing" && gameState !== "waiting";
    };

    return (
        <div className="p-4 text-center bg-gray-800 text-white min-h-screen">
            <h1 className="text-3xl font-bold mb-6 text-yellow-400">Black Jack</h1>
            
            {/* Game Status Banner */}
            {isGameOver() && (
                <div className="mb-4 p-2 bg-yellow-600 text-white font-bold rounded">
                    Game Over: {gameState}
                </div>
            )}

            <div className="mb-6 p-4 bg-gray-700 rounded shadow">
                <h2 className="text-xl font-semibold mb-2 text-gray-300">Dealer's Hand</h2>
                <div className="flex justify-center items-center space-x-2">
                    {dealerHand.map((card, idx) => (
                        <Card
                            key={idx}
                            card={card}
                            hidden={gameState === 'playing' && idx > 0}
                        />
                    ))}
                </div>
            </div>

            <div className="mb-6 p-4 bg-gray-700 rounded shadow">
                <h2 className="text-xl font-semibold mb-2 text-gray-300">Your Hand</h2>
                <div className="flex justify-center items-center space-x-2">
                    {playerHand.map((card, idx) => (
                        <Card key={idx} card={card} hidden={false} />
                    ))}
                </div>
            </div>

            <div className="mb-4 h-6 text-yellow-300 font-semibold">
                {message}
            </div>
            <div className="mb-4 h-6 text-cyan-400 font-semibold">
                {aiSuggestion}
            </div>

            <div style={{
                display: 'flex',
                gap: '15px',
                flexWrap: 'wrap',
                justifyContent: 'center',
            }}>
                {(gameState === "waiting" || isGameOver()) && (
                    <button onClick={startGame} style={{
                        backgroundColor: '#4ade80',
                        padding: '12px 24px',
                        border: 'none',
                        borderRadius: '8px',
                        color: 'white',
                        fontWeight: 'bold',
                        fontSize: '16px',
                        cursor: 'pointer',
                        alignItems: 'center',
                        justifyContent: 'center',
                    }}>
                        🎲 {isGameOver() ? 'Play Again' : 'Start Game'}
                    </button>
                )}
                {gameState === "playing" && (
                    <>
                        <button onClick={hit} style={{
                            backgroundColor: '#ef4444',
                            padding: '12px 24px',
                            border: 'none',
                            borderRadius: '8px',
                            color: 'white',
                            fontWeight: 'bold',
                            fontSize: '16px',
                            cursor: 'pointer',
                            alignItems: 'center',
                            display: 'flex',
                            justifyContent: 'center',
                        }}>
                            ❤️ Hit
                        </button>
                        <button
                            onClick={stand}
                            style={{
                                backgroundColor: '#3b82f6',
                                padding: '12px 24px',
                                border: 'none',
                                borderRadius: '8px',
                                color: 'white',
                                fontWeight: 'bold',
                                fontSize: '16px',
                                cursor: 'pointer',
                                display: 'flex',
                                alignItems: 'center',
                                justifyContent: 'center',
                            }}
                        >
                            ✋ Stand
                        </button>
                        <button
                            onClick={getAiHint}
                            style={{
                                backgroundColor: '#a855f7',
                                padding: '12px 24px',
                                border: 'none',
                                borderRadius: '8px',
                                color: 'white',
                                fontWeight: 'bold',
                                fontSize: '16px',
                                cursor: 'pointer',
                                display: 'flex',
                                alignItems: 'center',
                                justifyContent: 'center',
                            }}
                        >
                            💡 Get AI Hint
                        </button>
                    </>
                )}
            </div>
        </div>
    );
};

export default BlackJack;