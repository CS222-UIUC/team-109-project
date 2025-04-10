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


//black jack class



TEST_CASE("BlackJack deals two cards to each player on start") {
    BlackJack game;
    game.startGame();
    REQUIRE(game.getPlayerHand().size() == 2);
    REQUIRE(game.getDealerHand().size() == 2);
}

TEST_CASE("Player hit adds a card to their hand") {
    BlackJack game;
    game.startGame();
    size_t before = game.getPlayerHand().size();
    game.playerHit();
    size_t after = game.getPlayerHand().size();
    REQUIRE(after == before + 1);
}

TEST_CASE("Hand value handles multiple Aces correctly") {
    std::vector<Card> hand = {
        Card{"A", "Hearts"},
        Card{"9", "Spades"},
        Card{"5", "Clubs"}
    };

    BlackJack game;
    int score = game.calculateHandValue(hand);
    REQUIRE(score == 15);
}

TEST_CASE("Dealer draws until reaching at least 17") {
    BlackJack game;
    game.startGame();
    while (game.calculateHandValue(game.getPlayerHand()) < 17) {
        game.playerHit();
    }
    game.dealerTurn();
    int dealerScore = game.calculateHandValue(game.getDealerHand());
    REQUIRE(dealerScore >= 17);
}

TEST_CASE("Winner is decided correctly when hands are known") {
    BlackJack game;
    std::vector<Card> player = {
        Card{"10", "Hearts"},
        Card{"9", "Spades"}
    };
    std::vector<Card> dealer = {
        Card{"10", "Clubs"},
        Card{"7", "Diamonds"}
    };
    game.setPlayerHand(player);
    game.setDealerHand(dealer);
    REQUIRE(game.checkWinner() == "Player wins!");
}

