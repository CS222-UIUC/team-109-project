import random

class BlackjackBot:
    def __init__(self, simulation_threshold=0.5):
        #initialization
        self.simulation_threshold = simulation_threshold

    def get_card_value(self, card):
        """
        returns the base value for a given card.
        cards 'J', 'Q', 'K' are worth 10,
        ace is initially counted as 11, and numeric cards are converted to integers.
        """
        rank = card["rank"]
        if rank in ["J", "Q", "K"]:
            return 10
        elif rank == "A":
            return 11
        else:
            return int(rank)

    def calculate_hand_value(self, hand):
        """
        calculates the total value of a hand.
        adjusts for Aces (which are initially 11) by subtracting 10 as long as the total is over 21.
        """
        total = 0
        ace_count = 0

        for card in hand:
            value = self.get_card_value(card)
            total += value
            if card["rank"] == "A":
                ace_count += 1

        # Adjust for Aces if the total value is over 21.
        while total > 21 and ace_count:
            total -= 10
            ace_count -= 1

        return total
    
    def simulate_bust_probability(self, hand):
        """
        simulates the probability of busting if the bot draws one additional card.
        the function calculates the new total for each rank and sums the probabilities of those outcomes that would cause the hand to exceed 21.
        """
        current_total = self.calculate_hand_value(hand)
        
        # Define the probability distribution for drawing a card.
        rank_probabilities = {
            "A": 1/13,
            "2": 1/13,
            "3": 1/13,
            "4": 1/13,
            "5": 1/13,
            "6": 1/13,
            "7": 1/13,
            "8": 1/13,
            "9": 1/13,
            "10": 4/13  
        }

        bust_probability = 0.0

        for rank, prob in rank_probabilities.items():
            simulated_card = {"rank": rank, "suit": "Any"}  # suit is irrelevant for value
            drawn_value = self.get_card_value(simulated_card) 
            new_total = current_total + drawn_value
            if new_total > 21:
                bust_probability += prob

        # Debug output
        print(f"Simulated bust probability for current total {current_total}: {bust_probability:.2f}")
        return bust_probability

    def decide_action(self, hand):
        """
        determines the bot's action based on its current hand.
        it uses a Monte Carlo-like simulation to estimate the chance of busting if it hits.
        if the bust probability is lower than the simulation_threshold, the bot chooses to hit; otherwise, it stands.
        """
        current_total = self.calculate_hand_value(hand)
        
        if current_total >= 21:
            decision = "stand"
            print(f"Current total is {current_total}, so decision is: {decision}")
            return decision
        
        # Estimate bust probability for one more card
        bust_probability = self.simulate_bust_probability(hand)
        
        if bust_probability < self.simulation_threshold:
            decision = "hit"
        else:
            decision = "stand"
        
        print(f"Current hand value: {current_total}")
        print(f"Decision based on simulation: {decision}")
        return decision


if __name__ == "__main__":
    # Simulated hand: list of cards represented as dictionaries.
    sample_hand = [
        {"rank": "10", "suit": "Hearts"},
        {"rank": "6", "suit": "Diamonds"}
    ]

    bot = BlackjackBot(simulation_threshold=0.5)
    action = bot.decide_action(sample_hand)
    print(f"Bot recommends: {action}")
