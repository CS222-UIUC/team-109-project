// game_server.cc
#include "httplib.h"
#include "nlohmann/json.hpp"
#include "BlackJack.hpp"
#include <iostream>

using json = nlohmann::json;


// serialize a card
static json cardToJson(const Deck::Card &c) {
    return {{"rank", c.rank}, {"suit", c.suit}};
}

// serialize a hand
static json handToJson(const std::vector<Deck::Card> &hand) {
    json j = json::array();
    for (auto &c : hand) j.push_back(cardToJson(c));
    return j;
}

int main() {
    BlackJack game;
    httplib::Server svr;

    // Allow cross-origin requests from your React app
    svr.set_pre_routing_handler(
    [](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "http://localhost:3000");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
        // if it's a preflight OPTIONS, short-circuit with 200
        if (req.method == "OPTIONS") {
        res.status = 200;
        return httplib::Server::HandlerResponse::Handled;
        }
        return httplib::Server::HandlerResponse::Unhandled;
    }
    );

    // START
    svr.Post("/start", [&](auto&, auto &res) {
        game.reset();
        game.dealInitialCards();
        json resp;
        resp["player"]    = handToJson(game.getPlayerHand());
        // partial dealer
        json d = json::array();
        auto dh = game.getDealerHand();
        if (!dh.empty()) {
            d.push_back(cardToJson(dh[0]));
            d.push_back({{"rank","?"},{"suit","Hidden"}});
        }
        resp["dealer"]    = d;
        resp["gameState"] = "playing";
        res.set_content(resp.dump(), "application/json");
    });

    // HIT
    svr.Post("/hit", [&](auto&, auto &res) {
        if (game.isGameOver()) {
            res.status = 400;
            res.set_content(R"({"error":"Game is over, cannot hit."})",
                            "application/json");
            return;
        }
        game.playerHit();
        json resp;
        resp["player"] = handToJson(game.getPlayerHand());
        if (game.isPlayerBusted()) {
            resp["gameState"] = "player_bust";
            resp["dealer"]    = handToJson(game.getDealerHand());
        } else {
            resp["gameState"] = "playing";
            json d = json::array();
            auto dh = game.getDealerHand();
            if (!dh.empty()) {
                d.push_back(cardToJson(dh[0]));
                d.push_back({{"rank","?"},{"suit","Hidden"}});
            }
            resp["dealer"] = d;
        }
        res.set_content(resp.dump(), "application/json");
    });

    // STAND
    svr.Post("/stand", [&](auto&, auto &res) {
        if (game.isGameOver()) {
            res.status = 400;
            res.set_content(R"({"error":"Game is over, cannot stand."})",
                            "application/json");
            return;
        }
        if (!game.isPlayerBusted()) game.dealerPlays();
        else                         game.setGameOver(true);

        json resp;
        resp["player"]    = handToJson(game.getPlayerHand());
        resp["dealer"]    = handToJson(game.getDealerHand());
        resp["gameState"] = game.determineOutcome();
        res.set_content(resp.dump(), "application/json");
    });

    std::cout << "Server listening on port 5000\n";
    svr.listen("0.0.0.0", 5002);
}
