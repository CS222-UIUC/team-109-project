/* 
prospective usage of BlackJack class
*/
#include <iostream>
#include "BlackJack.hpp"
int main() {
    BlackJack game;
    char again;

    do {
        game.playGame();
        std::cout << "Play again? (y/n): ";
        std::cin >> again;
    } while (again == 'y' || again == 'Y');

    return 0;
}
