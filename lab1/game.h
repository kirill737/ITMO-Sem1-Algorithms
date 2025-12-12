// Game.h
#pragma once
#include <iostream>
#include <random>

#include <fstream>
#include "nlohman/json.hpp"


namespace hamurabi {
using json = nlohmann::json;
class Game {
private:
    // Генераторы случайных чисел
    std::random_device rd;
    std::mt19937 gen;

    // Распределения (часть инициализируется в конструкторе)
    std::uniform_int_distribution<int> acrePriceDist_;
    std::uniform_int_distribution<int> wheatPerAcreDist_;
    std::uniform_int_distribution<int> ratsPercentDist_; // 0..7 %
    std::bernoulli_distribution isPlagueDist_;

    //  Методы для вычисления случайных параметров
    int calcAcrePrice();
    int calcWheatPerAcre();
    int calcEatenByRats();        // теперь считает по проценту
    int calcStarvedPeople();
    int calcNewPeople(); 
    bool calcPlague();
    void updateMainInfo();    

    // Основные параметры
    int population_ = 100;
    int acres_      = 1000;
    int wheat_      = 2800;
    int year_       = 1;

    // Случайно вычисляемые параметры
    int newPeople_      = 5;
    int starvedPeople_  = 0;
    int acrePrice_      = 0;
    int wheatPerAcre_   = 3;
    int eatenByRats_    = 0;
    bool plague_        = false;

    // Вводимые игроком значения
    int plantedAcres_ = 100;
    int eatenWheat_   = 0;

    // Для статистики
    int totalStarvedPercent_ = 0; // суммарный процент умерших по годам

    // Сообщения о некорректно введённых данных
    void negativeInputMsg() const;
    void notEnoughtAcresMsg() const;
    void notEnoughWheatMsg() const;
    void notEnoughPeopleMsg() const;
    void notValidSaveOptionMsg() const;

    // Вспомогательные
    bool readValue(const std::string& prompt, int& value);

public: 
    Game(); // конструктор — инициализация генератора и распределений
    void start();
    void yearInfoMsg(const int& year) const;
    bool playerChoice();
    bool nextYear();

    void Game::saveProgress();
    bool Game::loadProgress();
    
};
}
