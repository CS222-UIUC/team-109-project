import random

class BlackjackBot:
    def __init__(self, simulation_threshold=0.5, epsilon=0.1, alpha=0.1):
        #initialization
        self.simulation_threshold = simulation_threshold
        self.epsilon = epsilon
        self.alpha = alpha
        self.q_table = {}
        self.round_history = []

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
    
    def choose_action(self, state, hand):
        """
        chooses an action using an epsilon-greedy policy.
        if the state is new (no Q-values yet), it falls back to the simulation-based decision: if bust probability is lower than the threshold, choose hit, if not, stand.
        """
        # Check if Q-values exist for this state.
        if ((state, "hit") not in self.q_table) and ((state, "stand") not in self.q_table):
            bust_probability = self.simulate_bust_probability(hand)
            if bust_probability < self.simulation_threshold:
                self.q_table[(state, "hit")] = 1 - bust_probability
                self.q_table[(state, "stand")] = 0
                decision = "hit"
            else:
                self.q_table[(state, "hit")] = 0
                self.q_table[(state, "stand")] = 1 - bust_probability
                decision = "stand"
            print(f"New state {state}: using simulation baseline, decision: {decision}")
            return decision

        # Epsilon-greedy exploration.
        if random.random() < self.epsilon:
            decision = random.choice(["hit", "stand"])
            print(f"Exploration: randomly chosen decision: {decision}")
            return decision
        else:
            q_hit = self.q_table.get((state, "hit"), 0)
            q_stand = self.q_table.get((state, "stand"), 0)
            decision = "hit" if q_hit > q_stand else "stand" if q_stand > q_hit else random.choice(["hit", "stand"])
            print(f"Exploitation: state {state} Q-values: hit={q_hit:.2f}, stand={q_stand:.2f}, decision: {decision}")
            return decision

    def decide_action(self, hand):
        """
        determines the bot's action based on its current hand.
        it uses a Monte Carlo-like simulation to estimate the chance of busting if it hits.
        if the bust probability is lower than the simulation_threshold, the bot chooses to hit; otherwise, it stands.
        """
        state = self.calculate_hand_value(hand)
        
        if state >= 21:
            decision = "stand"
            print(f"Hand value {state} is terminal. Decision: {decision}")
            return decision

        decision = self.choose_action(state, hand)
        self.round_history.append((state, decision))
        print(f"State: {state}, decision recorded: {decision}")
        return decision

    def update_memory(self, reward):
        """
        updates the Q-table for each (state, action) pair taken during the round.
        uses a simple update rule:
            Q(s, a) = Q(s, a) + alpha * (reward - Q(s, a))
        then clears the round history for the next round.
        """
        print(f"Updating memory with reward: {reward}")
        for state, action in self.round_history:
            old_value = self.q_table.get((state, action), 0)
            new_value = old_value + self.alpha * (reward - old_value)
            self.q_table[(state, action)] = new_value
            print(f"Updated Q({state}, {action}): {old_value:.2f} -> {new_value:.2f}")
        self.round_history.clear()

if __name__ == "__main__":
    # simulated hand: list of cards represented as dictionaries.
    sample_hand = [
        {"rank": "10", "suit": "Hearts"},
        {"rank": "6", "suit": "Diamonds"}
    ]

    bot = BlackjackBot(simulation_threshold=0.5, epsilon=0.2, alpha=0.1)
    action = bot.decide_action(sample_hand)
    print(f"Bot recommends: {action}")

    # simulate end of round
    # eg, assume the round ended with a win (+1 reward).
    final_reward = 1
    bot.update_memory(final_reward)
