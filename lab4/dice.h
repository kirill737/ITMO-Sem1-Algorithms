#include <vector>
#include <random>
#include <string>
#include <iostream>
#include <sstream>
#include <stdexcept>

struct ThrowInfo {
    int amount;     // Сколко кубиков бросить
    int sides;      // Сколько у них граней
    int modifier;   // Какой модификаттор

    void print() {
        std::cout << "amount: " << amount << ", sides: " << sides << ", modifier: " << modifier << std::endl;
    }
};

typedef std::vector<ThrowInfo> DiceSet;

// Конвертации для одного броска
ThrowInfo strToThrowInfo(const std::string& rawDiceInfo) {
    ThrowInfo resThrowInfo{0, 0, 0};

    size_t dPos = rawDiceInfo.find('d');
    // Проверяем что верный формат
    if (dPos == std::string::npos)
        throw std::invalid_argument("No 'd' in input string");

    resThrowInfo.amount = std::stoi(rawDiceInfo.substr(0, dPos));

    size_t modifierPos = rawDiceInfo.find_first_of("+-", dPos + 1);
    if (modifierPos == std::string::npos) {
        resThrowInfo.sides = std::stoi(rawDiceInfo.substr(dPos + 1));
    } else {
        resThrowInfo.sides = std::stoi(rawDiceInfo.substr(dPos + 1, modifierPos - dPos - 1));
        resThrowInfo.modifier = std::stoi(rawDiceInfo.substr(modifierPos));
    }

    return resThrowInfo;
}

std::string throwInfoToStr(const ThrowInfo& throwInfo) {
    std::string res = std::to_string(throwInfo.amount) + "d" + std::to_string(throwInfo.sides);

    if (throwInfo.modifier > 0)
        res += "+" + std::to_string(throwInfo.modifier);
    else if (throwInfo.modifier < 0)
        res += std::to_string(throwInfo.modifier);

    return res;
}

// Конвертации для сразу нескольких бросков
std::string diceSetToStr(const DiceSet& fewDices) {
    std::string res;
    for (const auto& d : fewDices) {
        res.append(throwInfoToStr(d));
    }
    return res;
}

DiceSet strToDiceSet(const std::string& fewDicesRaw) {
    DiceSet res;
    
    std::stringstream ss(fewDicesRaw);
    std::string throwInfoRaw;

    while (std::getline(ss, throwInfoRaw, ',')) {
        res.push_back(strToThrowInfo(throwInfoRaw));
    }

    return res;
}

// Функции для бросков
int dice(const DiceSet& fewDices) {
    static std::mt19937 gen(std::random_device{}());
    int result = 0;

    for (const auto& d : fewDices) {
        std::uniform_int_distribution<> dist(1, d.sides);
        for (int i = 0; i < d.amount; ++i)
            result += dist(gen);
        result += d.modifier;
    }
    return result;
}

int dice(const std::string& fewDices) {
    DiceSet diceSet = strToDiceSet(fewDices);
    static std::mt19937 gen(std::random_device{}());
    int result = 0;
    
    for (const auto& d : diceSet) {
        std::uniform_int_distribution<> dist(1, d.sides);
        for (int i = 0; i < d.amount; ++i)
            result += dist(gen);
        result += d.modifier;
    }
    return result;
}

// БНФ 
// <diceString> ::= <number> "d" <number> [ ( "+" | "-" ) <number> ]
// <number>  ::= <digit> { <digit> }
// <digit> ::= "0" | ... | "9"



