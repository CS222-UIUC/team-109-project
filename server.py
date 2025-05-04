from flask import Flask, request, jsonify
from flask_cors import CORS
from AIBlackJack import BlackjackBot  

app = Flask(__name__)
CORS(app)

blackjack_bot = BlackjackBot(simulation_threshold=0.45) 

@app.route('/api/bot_decision', methods=['POST'])
def get_bot_decision():
    """
    API endpoint to get the AI bot's decision for a given hand.
    """
    data = request.get_json()
    if not data or 'playerHand' not in data:
        return jsonify({"error": "Missing playerHand in request"}), 400

    player_hand = data['playerHand']
    
    # Basic validation 
    if not isinstance(player_hand, list):
         return jsonify({"error": "playerHand should be a list"}), 400
        
    try:
        # Get the decision from the bot
        decision = blackjack_bot.decide_action(player_hand)
        # Log the decision server-side (optional)
        print(f"Bot received hand: {player_hand}, decided: {decision}") 
        return jsonify({"decision": decision})
    except Exception as e:
        # Log the error for debugging
        print(f"Error during bot decision: {e}") 
        return jsonify({"error": "Internal server error processing bot decision"}), 500

# Run server
if __name__ == '__main__':
    print("Starting Flask server for Blackjack AI...")
    app.run(host='0.0.0.0', port=5001, debug=True) 