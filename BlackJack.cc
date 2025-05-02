#include "BlackJack.hpp"
#include <iostream>
#include <limits>

BlackJack::BlackJack() : deck(6), gameOver(false) {}

void BlackJack::reset() {
    playerHand.clear();
    dealerHand.clear();
    gameOver = false;
    deck.reset();
}

int BlackJack::getCardValue(const Deck::Card& card) const {
    if (card.rank == "J" || card.rank == "Q" || card.rank == "K") return 10;
    if (card.rank == "A") return 11;
    return std::stoi(card.rank);
}


int BlackJack::calculateHandValue(const std::vector<Deck::Card>& hand) const {
  int total = 0, aceCount = 0;
  for (auto& c : hand) {
    int v = getCardValue(c);
    total += v;
    if (c.rank == "A") aceCount++;
  }
  while (total > 21 && aceCount--) total -= 10;
  return total;
}


void BlackJack::playerHit() {
    playerHand.push_back(deck.draw());
    if (isBust(playerHand)) gameOver = true;
}

void BlackJack::dealerPlays() {
    // reuse existing dealerTurn logic, then end the game
    dealerTurn();
    gameOver = true;
}

bool BlackJack::isBust(const std::vector<Deck::Card>& hand) const {
    return calculateHandValue(hand) > 21;
}

void BlackJack::displayHand(const std::vector<Deck::Card>& hand, const std::string& name) const {
    std::cout << name << "'s Hand: ";
    for (const auto& card : hand) {
        std::cout << card.Value_Card() << " | ";
    }
    std::cout << "Value: " << calculateHandValue(hand) << std::endl;
}

void BlackJack::dealInitialCards() {
    playerHand.push_back(deck.draw());
    dealerHand.push_back(deck.draw());
    playerHand.push_back(deck.draw());
    dealerHand.push_back(deck.draw());
}

void BlackJack::playerTurn() {
    std::string choice;

    while (true) {
        displayHand(playerHand, "Player");
        if (isBust(playerHand)) {
            std::cout << "Player busts!" << std::endl;
            return;
        }

        std::cout << "Do you want to hit or stand? (h/s): ";
        std::cin >> choice;
        if (choice == "h") {
            playerHand.push_back(deck.draw());
        } else if (choice == "s") {
            break;
        }
    }
}

void BlackJack::dealerTurn() {
    std::cout << "Dealer reveals hand:" << std::endl;
    displayHand(dealerHand, "Dealer");

    while (calculateHandValue(dealerHand) < 17) {
        std::cout << "Dealer hits." << std::endl;
        dealerHand.push_back(deck.draw());
        displayHand(dealerHand, "Dealer");
    }

    if (isBust(dealerHand)) {
        std::cout << "Dealer busts!" << std::endl;
    }
}

std::string BlackJack::determineWinner() const {
    int playerScore = calculateHandValue(playerHand);
    int dealerScore = calculateHandValue(dealerHand);

    if (playerScore > 21) return "Dealer wins!";
    if (dealerScore > 21) return "Player wins!";
    if (playerScore > dealerScore) return "Player wins!";
    if (dealerScore > playerScore) return "Dealer wins!";
    return "Push (tie)!";
}

void BlackJack::playGame() {
    reset();
    dealInitialCards();

    std::cout << "Dealer show: " << dealerHand[0].Value_Card() << " | ?" << std::endl;
    playerTurn();

    if (!isBust(playerHand)) {
        dealerTurn();
    }

    std::cout << determineWinner() << std::endl;
}


const std::vector<Deck::Card>& BlackJack::getPlayerHand() const {
    return playerHand;
}

const std::vector<Deck::Card>& BlackJack::getDealerHand() const {
    return dealerHand;
}

int BlackJack::getPlayerScore() const {
    return calculateHandValue(playerHand);
}

bool BlackJack::isPlayerBusted() const {
    return isBust(playerHand);
}

bool BlackJack::isGameOver() const {
    return gameOver;
}

std::string BlackJack::determineOutcome() const {
    return determineWinner();
}

void BlackJack::setGameOver(bool over) {
    gameOver = over;
}