#include "crow_all.h" 
#include "nlohmann/json.hpp" 
#include "BlackJack.hpp" 
#include <iostream>
#include <string>
#include <vector>
#include <mutex> 

using json = nlohmann::json;

json cardToJson(const Deck::Card& card) {
    return json{{"rank", card.rank}, {"suit", card.suit}};
}

json handToJson(const std::vector<Deck::Card>& hand) {
    json jsonHand = json::array();
    for (const auto& card : hand) {
        jsonHand.push_back(cardToJson(card));
    }
    return jsonHand;
}

BlackJack blackjack_game;
std::mutex game_mutex;

int main() {
    crow::SimpleApp app;

    // Enable CORS
    auto& cors = app.get_middleware<crow::CorsMiddleware>();
    cors
      .global() // Apply to all routes
        .origin("http://localhost:3000") // Allow React frontend
        .methods("POST"_method, "GET"_method)
        .headers("Content-Type"); 

    CROW_ROUTE(app, "/start").methods(crow::HTTPMethod::POST)
    ([]() {
        std::lock_guard<std::mutex> lock(game_mutex); // Lock the game

        try {
            blackjack_game.reset();
            blackjack_game.dealInitialCards();

            json response;
            response["player"] = handToJson(blackjack_game.getPlayerHand());
            
            json dealerHandPartial = json::array();
            if (!blackjack_game.getDealerHand().empty()) {
                 dealerHandPartial.push_back(cardToJson(blackjack_game.getDealerHand()[0]));
                 dealerHandPartial.push_back({{"rank", "?"}, {"suit", "Hidden"}}); 
            }
            response["dealer"] = dealerHandPartial;

            // Determine initial game state
            if (blackjack_game.getPlayerScore() == 21) {
                 response["gameState"] = "playing"; 
            } else {
                 response["gameState"] = "playing";
            }
             
            std::cout << "/start called. Player: " << response["player"].dump() 
                      << ", Dealer Partial: " << response["dealer"].dump() << std::endl;

            return crow::response(200, response.dump());
        } catch (const std::exception& e) {
            std::cerr << "Error in /start: " << e.what() << std::endl;
            json error_resp = {{"error", e.what()}};
            return crow::response(500, error_resp.dump());
        }
    });

    CROW_ROUTE(app, "/hit").methods(crow::HTTPMethod::POST)
    ([]() {
        std::lock_guard<std::mutex> lock(game_mutex);

        if (blackjack_game.isGameOver()) {
             json error_resp = {{"error", "Game is over, cannot hit."}};
             return crow::response(400, error_resp.dump()); 
        }

        try {
            blackjack_game.playerHit(); // Player draws a card

            json response;
            response["player"] = handToJson(blackjack_game.getPlayerHand());
            
            if (blackjack_game.isPlayerBusted()) {
                response["gameState"] = "player_bust";
                // Include final dealer hand when player busts
                response["dealer"] = handToJson(blackjack_game.getDealerHand()); 
            } else {
                response["gameState"] = "playing";
                 json dealerHandPartial = json::array();
                 if (!blackjack_game.getDealerHand().empty()) {
                      dealerHandPartial.push_back(cardToJson(blackjack_game.getDealerHand()[0]));
                      dealerHandPartial.push_back({{"rank", "?"}, {"suit", "Hidden"}}); 
                 }
                 response["dealer"] = dealerHandPartial; 
            }
             
            std::cout << "/hit called. Player: " << response["player"].dump() 
                      << ", State: " << response["gameState"] << std::endl;

            return crow::response(200, response.dump());
        } catch (const std::exception& e) {
            std::cerr << "Error in /hit: " << e.what() << std::endl;
            json error_resp = {{"error", e.what()}};
            return crow::response(500, error_resp.dump());
        }
    });

    CROW_ROUTE(app, "/stand").methods(crow::HTTPMethod::POST)
    ([]() {
        std::lock_guard<std::mutex> lock(game_mutex); // Lock the game instance

        if (blackjack_game.isGameOver()) {
             json error_resp = {{"error", "Game is over, cannot stand."}};
             return crow::response(400, error_resp.dump());
        }

        try {
            if (!blackjack_game.isPlayerBusted()) {
                 blackjack_game.dealerPlays();
            } else {
                 // Player already busted
                 blackjack_game.setGameOver(true); 
            }
             
            json response;
            response["player"] = handToJson(blackjack_game.getPlayerHand()); 
            response["dealer"] = handToJson(blackjack_game.getDealerHand()); 
            // Determine the final outcome
            response["gameState"] = blackjack_game.determineOutcome(); 

            std::cout << "/stand called. Player: " << response["player"].dump() 
                      << ", Dealer Final: " << response["dealer"].dump() 
                      << ", Outcome: " << response["gameState"] << std::endl;

            return crow::response(200, response.dump());
        } catch (const std::exception& e) {
            std::cerr << "Error in /stand: " << e.what() << std::endl;
            json error_resp = {{"error", e.what()}};
            return crow::response(500, error_resp.dump());
        }
    });
    
    app.loglevel(crow::LogLevel::Info); 

    // Run on port 5000
    std::cout << "Starting Blackjack Game Server on port 5000..." << std::endl;
    app.port(5000).run(); 

    return 0;
}