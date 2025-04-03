import React, { useState } from "react";

const BlackJack = () => {
    const [playerHand, setPlayerHand] = useState([]);
    const [dealerHand, setDealerHand] = useState([]);
    const [deck, setDeck] = useState([]);
    const [gameState, setGameState] = useState("waiting");

    const startGame = async () => {
        const response = await fetch("http://localhost:5000/start");
        const data = await response.json();
        setPlayerHand(data.player);
        setDealerHand(data.dealer);
        setDeck(data.deck);
        setGameState("playing");
    };

    const hit = async () => {
        const response = await fetch("http://localhost:5000/hit");
        const data = await response.json();
        setPlayerHand(data.player);
        setGameState(data.gameState);
    };

    const stand = async () => {
        const response = await fetch("http://localhost:5000/stand");
        const data = await response.json();
        setDealerHand(data.dealer);
        setGameState(data.gameState);
    };

    return (
        <div className="p-4 text-center">
            <h1 className="text-xl font-bold">HunchAI - Black Jack</h1>
            <div className="mt-4">
                <h2>Dealer's Hand</h2>
                <p>{dealerHand.join(", ")}</p>
            </div>
            <div className="mt-4">
                <h2>Your Hand</h2>
                <p>{playerHand.join(", ")}</p>
            </div>
            {gameState === "waiting" && (
                <button onClick={startGame}>Start Game</button>
            )}
            {gameState === "playing" && (
                <div>
                    <button onClick={hit} className="mr-2">Hit</button>
                    <button onClick={stand}>Stand</button>
                </div>
            )}
            {gameState !== "playing" && gameState !== "waiting" && (
                <p className="mt-4">Game Over: {gameState}</p>
            )}
        </div>
    );
};

export default BlackJack;
