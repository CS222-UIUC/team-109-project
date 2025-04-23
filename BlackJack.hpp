#ifndef BLACKJACK_HPP
#define BLACKJACK_HPP

#include "Deck.hpp"
#include <vector>
#include <string>

class BlackJack {
public:
    BlackJack();
    void playGame();
    void reset();

    // — Public API for game_server.cc —
    void dealInitialCards();
    void playerHit();
    void dealerPlays();
    void setGameOver(bool over);

    const std::vector<Deck::Card>& getPlayerHand()  const;
    const std::vector<Deck::Card>& getDealerHand()  const;
    int   getPlayerScore() const;
    bool  isPlayerBusted() const;
    bool  isGameOver()     const;
    std::string determineOutcome() const;  // alias for determineWinner()


private:
    Deck deck;
    std::vector<Deck::Card> playerHand;
    std::vector<Deck::Card> dealerHand;
    bool gameOver;

    // Internal helpers
    int calculateHandValue(const std::vector<Deck::Card>& hand) const;
    void playerTurn();
    void dealerTurn();
    void displayHand(const std::vector<Deck::Card>& hand, const std::string& name) const;
    bool isBust(const std::vector<Deck::Card>& hand) const;
    std::string determineWinner() const;
    int getCardValue(const Deck::Card& card) const;
};

#endif
