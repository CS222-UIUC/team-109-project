// clang-format off
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
//  Written By : Student Name                    Environment : ubuntu:bionic               //
//  Date ......: 2021/02/10                      Compiler ...: clang-10                    //
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
// clang-format on
/////////////////////////////////////////////////////////////////////////////////////////////
//                             Framework Set-up //
/////////////////////////////////////////////////////////////////////////////////////////////
#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch2/catch_test_macros.hpp>


/////////////////////////////////////////////////////////////////////////////////////////////
//                                 Includes //
/////////////////////////////////////////////////////////////////////////////////////////////
#include <stdexcept>

#include "Deck.hpp"
#include "BlackJack.hpp"

// #include "Deck.cc"


/////////////////////////////////////////////////////////////////////////////////////////////
//                             Helpers/Constants //
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
//                                Test Cases //
/////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("True == True", "") { REQUIRE(true == true); }

/////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("Deck is initialized with correct number of cards", "[deck]") {
    Deck deck(1, false);
    int expectedCardCount = 52; // Standard deck without jokers
    REQUIRE(deck.empty() == false);

    int count = 0;
    while (!deck.empty()) {
        deck.draw();
        count++;
    }

    REQUIRE(count == expectedCardCount);
}

TEST_CASE("Deck properly handles multiple decks of 6 for blackjack", "[deck]") {
    Deck deck(2, false);
    int expectedCardCount = 52 * 6;
    
    int count = 0;
    while (!deck.empty()) {
        deck.draw();
        count++;
    }

    REQUIRE(count == expectedCardCount);
}

TEST_CASE("Deck correctly includes jokers if enabled", "[deck]") {
    Deck deck(1, true);
    int expectedCardCount = 54; // 52 standard + 2 jokers

    int count = 0;
    int jokerCount = 0;
    while (!deck.empty()) {
        Deck::Card c = deck.draw();
        if (c.Value_Card() == "Joker") {
            jokerCount++;
        }
        count++;
    }

    REQUIRE(count == expectedCardCount);
    REQUIRE(jokerCount == 2);
}

TEST_CASE("Drawing a card removes it from the deck", "[deck]") {
    Deck deck(1, false);
    int initialSize = 52;

    Deck::Card drawnCard = deck.draw();
    REQUIRE(deck.empty() == false);
    
    int remainingCards = 0;
    while (!deck.empty()) {
        deck.draw();
        remainingCards++;
    }

    REQUIRE(remainingCards == initialSize - 1);
}

TEST_CASE("Deck resets properly without used cards", "[deck]") {
    Deck deck(1, false);
    
    for (int i = 0; i < 10; ++i) {
        deck.draw();
    }

    deck.reset(false);
    REQUIRE(deck.empty() == false);

    int count = 0;
    while (!deck.empty()) {
        deck.draw();
        count++;
    }

    REQUIRE(count == 52);
}

TEST_CASE("Deck resets properly with used cards", "[deck]") {
    Deck deck(1, false);

    for (int i = 0; i < 10; ++i) {
        deck.draw();
    }

    deck.reset(true);
    REQUIRE(deck.empty() == false);

    int count = 0;
    while (!deck.empty()) {
        deck.draw();
        count++;
    }

    REQUIRE(count == 52);
}

TEST_CASE("Deck constructor throws exceptions for invalid deck count", "[deck]") {
    REQUIRE_THROWS_AS(Deck(0, false), std::invalid_argument);
    REQUIRE_THROWS_AS(Deck(7, false), std::invalid_argument);
}

TEST_CASE("Card value representation works correctly", "[card]") {
    Deck::Card card1("K", "Hearts");
    Deck::Card jokerCard;

    REQUIRE(card1.Value_Card() == "K Hearts");
    REQUIRE(jokerCard.Value_Card() == "Joker");
}


//black jack class


/////


void test_getCardValue() {
    BlackJack game;
    assert(game.getCardValue({"2", "Hearts"}) == 2);
    assert(game.getCardValue({"10", "Spades"}) == 10);
    assert(game.getCardValue({"K", "Clubs"}) == 10);
    assert(game.getCardValue({"A", "Diamonds"}) == 11);
    std::cout << "test_getCardValue passed\n";
}

void test_calculateHandValue() {
    BlackJack game;

    std::vector<Deck::Card> hand1 = { {"A", "Hearts"}, {"9", "Diamonds"} };
    assert(game.calculateHandValue(hand1) == 20);

    std::vector<Deck::Card> hand2 = { {"A", "Hearts"}, {"9", "Diamonds"}, {"A", "Spades"} };
    assert(game.calculateHandValue(hand2) == 21);

    std::vector<Deck::Card> hand3 = { {"A", "Hearts"}, {"9", "Diamonds"}, {"A", "Spades"}, {"5", "Clubs"} };
    assert(game.calculateHandValue(hand3) == 16);

    std::cout << "test_calculateHandValue passed\n";
}

void test_isBust() {
    BlackJack game;

    std::vector<Deck::Card> busted = { {"K", "Spades"}, {"9", "Hearts"}, {"5", "Diamonds"} };
    assert(game.isBust(busted) == true);

    std::vector<Deck::Card> safe = { {"A", "Spades"}, {"9", "Clubs"} };
    assert(game.isBust(safe) == false);

    std::cout << "test_isBust passed\n";
}

void test_determineWinner() {
    BlackJack game;

    game.reset();
    game.playerHand = { {"10", "Spades"}, {"9", "Diamonds"} };
    game.dealerHand = { {"10", "Clubs"}, {"7", "Hearts"} };
    assert(game.determineWinner() == "Player wins!");

    game.playerHand = { {"8", "Spades"}, {"8", "Hearts"} };
    game.dealerHand = { {"10", "Clubs"}, {"8", "Diamonds"} };
    assert(game.determineWinner() == "Dealer wins!");

    game.playerHand = { {"10", "Spades"}, {"7", "Hearts"} };
    game.dealerHand = { {"10", "Clubs"}, {"7", "Diamonds"} };
    assert(game.determineWinner() == "Push (tie)!");

    game.playerHand = { {"K", "Spades"}, {"9", "Hearts"}, {"5", "Clubs"} };
    game.dealerHand = { {"2", "Diamonds"}, {"5", "Spades"} };
    assert(game.determineWinner() == "Dealer wins!");

    game.playerHand = { {"10", "Hearts"}, {"9", "Diamonds"} };
    game.dealerHand = { {"K", "Clubs"}, {"9", "Spades"}, {"5", "Diamonds"} };
    assert(game.determineWinner() == "Player wins!");

    std::cout << "test_determineWinner passed\n";
}

void test_reset() {
    BlackJack game;
    game.playerHand = { {"10", "Hearts"} };
    game.dealerHand = { {"8", "Clubs"} };

    game.reset();
    assert(game.playerHand.empty());
    assert(game.dealerHand.empty());

    std::cout << "test_reset passed\n";
}



