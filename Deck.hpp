#ifndef DECK_HPP
#define DECK_HPP


#include	 <iostream>
#include <vector>
#include <string>

class Deck {
	public:

		struct Card {
			std::string rank;
			std::string suit;
			//Joker use depends on game. for Blackjack most version don't, but few do
			// bool isJoker;

			//constructor for non-joker playing cards
			Card(std::string r, std::string s);
			//constructor for joker 
			// Card();
			//return card value as string
			std::string Value_Card() const;
			//print card value
			void print() const;

		};

		//Deck constructor with 1
		Deck(int numDecks = 1);

		void shuffle();
		Card draw();
		bool empty() const;
		//some card games reshuffle dicarded while some cards are in use, for future games No version of blackjack
		void reset(bool shuffleUsedCards = true);

	private:
		std::vector<Card> cards_;
		std::vector<Card> discarded_;
		int numDecks_;
		// bool useJocker_;

		void makeDeck();

	};

#endif


