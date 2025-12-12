#include <iostream>
#include "game.h"
#include <windows.h>

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    
    hamurabi::Game game;
    game.start();

    return 0;
}
