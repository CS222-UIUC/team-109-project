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

private:
    Deck deck;
    std::vector<Deck::Card> playerHand;
    std::vector<Deck::Card> dealerHand;
    bool gameOver;

    int calculateHandValue(const std::vector<Deck::Card>& hand) const;
    void dealInitialCards();
    void playerTurn();
    void dealerTurn();
    void displayHand(const std::vector<Deck::Card>& hand, const std::string& name) const;
    bool isBust(const std::vector<Deck::Card>& hand) const;
    std::string determineWinner() const;
    int getCardValue(const Deck::Card& card) const;
};

#endif
