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

// TEST_CASE("True == True", "") { REQUIRE(true == true); }

/////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("Deck is initialized with correct number of cards", "[deck]") {
    Deck deck(1);
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
    Deck deck(6);
    int expectedCardCount = 52 * 6;
    
    int count = 0;
    while (!deck.empty()) {
        deck.draw();
        count++;
    }

    REQUIRE(count == expectedCardCount);
}



TEST_CASE("Drawing a card removes it from the deck", "[deck]") {
    Deck deck(1);
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
    Deck deck(1);
    
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
    Deck deck(1);

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
    REQUIRE_THROWS_AS(Deck(0), std::invalid_argument);
    REQUIRE_THROWS_AS(Deck(7), std::invalid_argument);
}

TEST_CASE("Card value representation works correctly", "[card]") {
    Deck::Card card1("K", "Hearts");

    REQUIRE(card1.Value_Card() == "K Hearts");
}



//black jack class


/////


TEST_CASE("Card values are retrieved correctly", "[blackjack]") {
    BlackJack game;
    SECTION("Face cards and numbers") {
        REQUIRE(game.getCardValue({"2", "Hearts"}) == 2);
        REQUIRE(game.getCardValue({"10", "Spades"}) == 10);
        REQUIRE(game.getCardValue({"K", "Clubs"}) == 10);
    }
    SECTION("Ace value") {
        REQUIRE(game.getCardValue({"A", "Diamonds"}) == 11);
    }
}

TEST_CASE("Hand value is calculated correctly", "[blackjack]") {
    BlackJack game;
    SECTION("No Aces") {
        std::vector<Deck::Card> hand1 = { {"A", "Hearts"}, {"9", "Diamonds"} };
        REQUIRE(game.calculateHandValue(hand1) == 20);
    }
    SECTION("One Ace resulting in 21") {
        std::vector<Deck::Card> hand2 = { {"A", "Hearts"}, {"9", "Diamonds"}, {"A", "Spades"} };
        REQUIRE(game.calculateHandValue(hand2) == 21);
    }
    SECTION("One Ace resulting in 11, other Ace as 1 to avoid bust") {
        std::vector<Deck::Card> hand3 = { {"A", "Hearts"}, {"9", "Diamonds"}, {"A", "Spades"}, {"5", "Clubs"} };
        REQUIRE(game.calculateHandValue(hand3) == 16);
    }
}

TEST_CASE("Bust condition is correctly identified", "[blackjack]") {
    BlackJack game;
    SECTION("Hand value over 21 is a bust") {
        std::vector<Deck::Card> busted = { {"K", "Spades"}, {"9", "Hearts"}, {"5", "Diamonds"} };
        REQUIRE(game.isBust(busted) == true);
    }
    SECTION("Hand value 21 or under is not a bust") {
        std::vector<Deck::Card> safe = { {"A", "Spades"}, {"9", "Clubs"} };
        REQUIRE(game.isBust(safe) == false);
    }
}

TEST_CASE("Winner is determined correctly", "[blackjack]") {
    BlackJack game;
    game.reset(); 

    SECTION("Player has a higher hand value") {
        game.playerHand = { {"10", "Spades"}, {"9", "Diamonds"} };
        game.dealerHand = { {"10", "Clubs"}, {"7", "Hearts"} };
        REQUIRE(game.determineWinner() == "Player wins!");
    }

    SECTION("Dealer has a higher hand value") {
        game.playerHand = { {"8", "Spades"}, {"8", "Hearts"} };
        game.dealerHand = { {"10", "Clubs"}, {"8", "Diamonds"} };
        REQUIRE(game.determineWinner() == "Dealer wins!");
    }

    SECTION("Player and dealer have the same hand value (push)") {
        game.playerHand = { {"10", "Spades"}, {"7", "Hearts"} };
        game.dealerHand = { {"10", "Clubs"}, {"7", "Diamonds"} };
        REQUIRE(game.determineWinner() == "Push (tie)!");
    }

    SECTION("Player busts, dealer doesn't") {
        game.playerHand = { {"K", "Spades"}, {"9", "Hearts"}, {"5", "Clubs"} };
        game.dealerHand = { {"2", "Diamonds"}, {"5", "Spades"} };
        REQUIRE(game.determineWinner() == "Dealer wins!");
    }

    SECTION("Dealer busts, player doesn't") {
        game.playerHand = { {"10", "Hearts"}, {"9", "Diamonds"} };
        game.dealerHand = { {"K", "Clubs"}, {"9", "Spades"}, {"5", "Diamonds"} };
        REQUIRE(game.determineWinner() == "Player wins!");
    }
}

TEST_CASE("Game state is reset correctly", "[blackjack]") {
    BlackJack game;
    game.playerHand = { {"10", "Hearts"} };
    game.dealerHand = { {"8", "Clubs"} };

    game.reset();
    REQUIRE(game.playerHand.empty());
    REQUIRE(game.dealerHand.empty());
}

TEST_CASE("Split functionality works correctly", "[blackjack][split]") {
    BlackJack game;

    // Set up a hand for split, two 8s
    game.reset();
    game.playerHand = { {"8", "Hearts"}, {"8", "Diamonds"} };

    SECTION("Can split with two cards of same value") {
        REQUIRE(game.canSplit() == true);
    }

    SECTION("Splitting creates two valid hands") {
        REQUIRE(game.canSplit());
        game.split();

        // After split, there should be two hands
        REQUIRE(game.playerHands.size() == 2);

        // Each hand should have 2 cards: one original 8 and one new drawn card
        for (const auto& hand : game.playerHands) {
            REQUIRE(hand.size() == 2);
            REQUIRE((hand[0].rank == "8")); // First card in each hand is one of the original 8s
        }
    }
}

// void test_getCardValue() {
//     BlackJack game;
//     assert(game.getCardValue({"2", "Hearts"}) == 2);
//     assert(game.getCardValue({"10", "Spades"}) == 10);
//     assert(game.getCardValue({"K", "Clubs"}) == 10);
//     assert(game.getCardValue({"A", "Diamonds"}) == 11);
//     std::cout << "test_getCardValue passed\n";
// }


// void test_calculateHandValue() {
//     BlackJack game;

//     std::vector<Deck::Card> hand1 = { {"A", "Hearts"}, {"9", "Diamonds"} };
//     assert(game.calculateHandValue(hand1) == 20);

//     std::vector<Deck::Card> hand2 = { {"A", "Hearts"}, {"9", "Diamonds"}, {"A", "Spades"} };
//     assert(game.calculateHandValue(hand2) == 21);

//     std::vector<Deck::Card> hand3 = { {"A", "Hearts"}, {"9", "Diamonds"}, {"A", "Spades"}, {"5", "Clubs"} };
//     assert(game.calculateHandValue(hand3) == 16);

//     std::cout << "test_calculateHandValue passed\n";
// }

// void test_isBust() {
//     BlackJack game;

//     std::vector<Deck::Card> busted = { {"K", "Spades"}, {"9", "Hearts"}, {"5", "Diamonds"} };
//     assert(game.isBust(busted) == true);

//     std::vector<Deck::Card> safe = { {"A", "Spades"}, {"9", "Clubs"} };
//     assert(game.isBust(safe) == false);

//     std::cout << "test_isBust passed\n";
// }

// void test_determineWinner() {
//     BlackJack game;

//     game.reset();
//     game.playerHand = { {"10", "Spades"}, {"9", "Diamonds"} };
//     game.dealerHand = { {"10", "Clubs"}, {"7", "Hearts"} };
//     assert(game.determineWinner() == "Player wins!");

//     game.playerHand = { {"8", "Spades"}, {"8", "Hearts"} };
//     game.dealerHand = { {"10", "Clubs"}, {"8", "Diamonds"} };
//     assert(game.determineWinner() == "Dealer wins!");

//     game.playerHand = { {"10", "Spades"}, {"7", "Hearts"} };
//     game.dealerHand = { {"10", "Clubs"}, {"7", "Diamonds"} };
//     assert(game.determineWinner() == "Push (tie)!");

//     game.playerHand = { {"K", "Spades"}, {"9", "Hearts"}, {"5", "Clubs"} };
//     game.dealerHand = { {"2", "Diamonds"}, {"5", "Spades"} };
//     assert(game.determineWinner() == "Dealer wins!");

//     game.playerHand = { {"10", "Hearts"}, {"9", "Diamonds"} };
//     game.dealerHand = { {"K", "Clubs"}, {"9", "Spades"}, {"5", "Diamonds"} };
//     assert(game.determineWinner() == "Player wins!");

//     std::cout << "test_determineWinner passed\n";
// }

// void test_reset() {
//     BlackJack game;
//     game.playerHand = { {"10", "Hearts"} };
//     game.dealerHand = { {"8", "Clubs"} };

//     game.reset();
//     assert(game.playerHand.empty());
//     assert(game.dealerHand.empty());

//     std::cout << "test_reset passed\n";
// }
