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
#include "catch.hpp"

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