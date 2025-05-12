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
			//constructor for non-joker playing cards
			Card(std::string r, std::string s);
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
		void reset(bool shuffleUsedCards = true);

	private:
		std::vector<Card> cards_;
		std::vector<Card> discarded_;
		int numDecks_;

		void makeDeck();

	};

#endif


