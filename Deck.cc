#include "deck.hoo"
#include <algorithm>
#include <random>
#include <stdexcept>

//non jocker constructor
Deck::Card::Card(const std::string& r, const  std::string& s) : rank(r), suit(s), Joker(false) {}
//jocker constructor
Deck::Card::Card() : rank("Joker"), suit(""), Joker(true) {}


std::string Deck::Card::Value_Card() const {
	return isJoker ? "Joker" : (rank + ' ' +suit)
}

void Deck::Card::print() const {
	std::cout << Value_Card();
}

Deck::Deck(int numDecks, bool useJoker) : numDecks_(numDecks), useJoker_(useJoker) {
	if (numDecks_ <= 0) throw std::invalide_argument("Number of Decks must be 1 or more.");
	//have to check games incorporated but for our blackjack version, 6 decks
	if (numDecks_ > 6) throw std::invalide_argument("Number of Deck must be less than 6,");
	makeDeck();
}

void Deck::makeDeck() {
	static const std::vector<std::string> suits = {"Spades", "Clubs", "Diamonds", "Hearts"};
	static const std::vector<std::string> ranks = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};

	cards_.clear();
	discarded_.clear();

	for (int i = 0; i <numDecks_; ++i) {
		for (const auto& suit : suits) {
			for (const auto& rank : ranks) {
				cards_.emplace_back(rank, suit);
			}
		}
		if (useJoker_) {
			cards_.emplace_back();
			cards_.emplace_back();
		}
	}
	shuffle();
}

void Deck::shuffle() {
	std::shuffle(cards_.begin(), cards_.end(), std::random_device{}());
}


Deck::Card Deck::draw() {
	if (cards_.empty()) {
		reset();
	}
	Card nextCard = cards_.back();
	cards_.pop_back();
	discarded_.push_back(nextCard);
	return nextCard;
}

bool Deck::empty() const {
	return cards_.empty();
}

void Deck::reset(bool shuffleUsedCards) {
	if (shuffleUsedCards) {
		cards_.insert(cards_.end(), discarded_.begin(), discarded_.end());
		shuffle();
		discarded_.clear();
	}
	else {
		discarded_.clear();
		makeDeck();
		shuffle();
	}
}

