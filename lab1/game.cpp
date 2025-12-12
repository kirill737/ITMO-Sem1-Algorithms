#include "game.h"
#include <sstream>
#include <string>
#include <cctype>
#include <algorithm>

namespace hamurabi {

Game::Game()
    : gen(rd()),
      acrePriceDist_{17, 26},
      wheatPerAcreDist_{1, 6},
      ratsPercentDist_{0, 7},
      isPlagueDist_{0.15}
{
    // начальные расчёты
    acrePrice_ = acrePriceDist_(gen);
    wheatPerAcre_ = wheatPerAcreDist_(gen);
    eatenByRats_ = 0;
}

int Game::calcAcrePrice() {
    acrePrice_ = (acrePriceDist_(gen));
    return acrePrice_;
};

int Game::calcWheatPerAcre() {
    wheatPerAcre_ = wheatPerAcreDist_(gen);
    return wheatPerAcre_;
};

// int Game::calcEatenByRats() {
//     eatenByRats_ = eatenByRatsDist_(gen);
//     return eatenByRats_;
// };
int Game::calcEatenByRats() {
    // ratsPercentDist_ генерирует процент 0..7
    int pct = ratsPercentDist_(gen);
    // считаем процент от имеющихся запасов + будущего урожая? 
    // В этой реализации считаем от запасов **до** уборки (вариант логики можно поменять)
    eatenByRats_ = (wheat_ + plantedAcres_ * wheatPerAcre_) * pct / 100;
    return eatenByRats_;
}

int Game::calcStarvedPeople() { 
    std::cout << "DEBUG " << population_ << " " << eatenWheat_ << std::endl;
    starvedPeople_ = population_ - eatenWheat_ / 20;
    if (starvedPeople_ < 0)
        starvedPeople_ = 0;
    return starvedPeople_;
};

int Game::calcNewPeople() { 
    int newPeople = starvedPeople_ / 2 + (5 - wheatPerAcre_) * wheat_ / 600 + 1;
    if (newPeople < 0)
        newPeople_ = 0;
    else if (newPeople > 50)
        newPeople_ = 50;
    else newPeople_ = newPeople;
    return newPeople_;
};

bool Game::calcPlague() { 
    plague_ = isPlagueDist_(gen);
    return plague_;
};

void Game::updateMainInfo() {
    // Обновляем население и запасы
    population_ = population_ - starvedPeople_ + newPeople_;
    if (plague_)
        population_ /= 2;
    // Собрали урожай и потеряли от крыс
    wheat_ = wheat_ + plantedAcres_ * wheatPerAcre_ - eatenByRats_;
    if (wheat_ < 0)
        wheat_ = 0;
}

// Сообщения о некорректно введённых данных
void Game::negativeInputMsg() const { std::cout << "\nНе могли бы Вы дать более точный приказ?" << std::endl; };
void Game::notEnoughtAcresMsg() const { std::cout << "\nК сожалению у нас всего " << acres_ << " акров, мой повелитель." << std::endl; };
void Game::notEnoughWheatMsg() const { std::cout << "\nМой повелитель, наших запасов не хватит. У нас всего " << wheat_ << " бушелей пшеницы." << std::endl; };
void Game::notEnoughPeopleMsg() const {
    std::cout << "\nО, повелитель, пощади нас! ";
    std::cout << "У нас только " << population_ << " человек, ";
    std::cout << wheat_ << " бушелей пшеницы ";
    std::cout << "и "<< acres_ << " акров земли!" << std::endl;
};
void Game::notValidSaveOptionMsg() const { std::cout << "\nНет такой опции." << std::endl; };

void Game::start() {
    int startYear = 1;
    bool gameEndedSuccessfully = true;

    if (std::ifstream("save.json")) {
        int saveOption = 1;
        while(true) {
            if (!readValue("У вас есть сохранение.\nПродолжить? (1 - да, 0 - нет): ", saveOption) || (saveOption != 0 && saveOption != 1)) {
                notValidSaveOptionMsg();
                continue;
            } else break;
        }
        if (saveOption == 1) {
            if (Game::loadProgress()) {
                // если загрузка прошла успешно, устанавливаем текущий год
                startYear = year_; // если есть поле year_
            }
        } else {
            std::remove("save.json");
        }
    }

    for (int year = startYear; year <= 10; ++year) {
        yearInfoMsg(year);

        // std::cout << "Хотите выйти и сохранить прогресс? (1 - да, 0 - нет): ";
        // int quit;
        // std::cin >> quit;

        int quitOption = 1;
        while(true) {
            if (!readValue("Хотите выйти и сохранить прогресс? (1 - да, 0 - нет): ", quitOption) || (quitOption != 0 && quitOption != 1)) {
                notValidSaveOptionMsg();
                continue;
            } else break;
        }
        if (quitOption == 1) {
            std::cout << "Игра сохранена.\n";
            return;
        }
        saveProgress(year);

        while (!playerChoice()); // Ожидание коректного пользовательского ввода
        if (!nextYear()) {
            gameEndedSuccessfully = false;
            break;
        }
    }
    if (gameEndedSuccessfully) {
        double P = static_cast<double>(totalStarvedPercent_) / 10.0; // средний процент за 10 лет
        double L = static_cast<double>(acres_) / static_cast<double>(population_); 

        if (P > 33 && L < 7) {
            std::cout << "Из-за вашей некомпетентности в управлении,\n";
            std::cout <<" народ устроил бунт, и изгнал вас их города.\n";
            std::cout << "Теперь вы вынуждены влачить жалкое существование в изгнании" << std::endl;
        } else if (P > 10 && L < 9) {
            std::cout << "Вы правили железной рукой, подобно Нерону и Ивану Грозному,\n";
            std::cout << "Народ вздохнул с облегчением, и никто больше не желает видеть вас правителем" << std::endl;
        } else if (P > 3 && L < 10) {
            std::cout << "Вы справились вполне неплохо,\n";
            std::cout << "у вас, конечно, есть недоброжелатели,\n";
            std::cout << "но многие хотели бы увидеть вас во главе города снова" << std::endl;
        } else {
            std::cout << "Фантастика! Карл Великий, Дизраэли и Джефферсон вместе не справились бы лучше" << std::endl;
        }

    }
    
}

void Game::yearInfoMsg(const int& year) const {
    std::cout << "\nМой повелитель, соизволь поведать тебе" << std::endl;
    std::cout << "\tВ году " << year << " твоего высочайшего правления" << std::endl;
    std::cout << "\t" << starvedPeople_ << " человек умерли с голоду, и " << newPeople_ << " человек прибыли в наш великий город;" << std::endl;;
    if (plague_) 
        std::cout << "\tЧума уничтожила половину населения;" << std::endl;
    std::cout << "\tНаселение города сейчас составляет " << population_ << " человек;" << std::endl;
    std::cout << "\tМы собрали " << wheatPerAcre_ * plantedAcres_ << " бушелей пшеницы, по " << wheatPerAcre_ << " бушеля с акра;" << std::endl;
    std::cout << "\tКрысы истребили " << eatenByRats_ << " бушелей пшеницы, оставив " << wheat_ << " бушеля в амбарах;" << std::endl;
    std::cout << "\tГород сейчас занимает " << acres_ << " акров;" << std::endl;
    std::cout << "\t1 акр земли стоит сейчас " << acrePrice_ << " бушель." << std::endl;
}


bool isNumber(const std::string& s) {
    if (s.empty()) return false;
    size_t i = 0;
    if (s[0] == '-' ) { if (s.size() == 1) return false; i = 1; }
    for (; i < s.size(); ++i)
        if (!std::isdigit(static_cast<unsigned char>(s[i]))) return false;
    return true;
}


bool Game::readValue(const std::string& prompt, int& value) {
    std::cout << prompt;
    std::string input;
    if (!std::getline(std::cin, input)) return false;
    // убираем ведущие/трейлинг пробелы
    auto l = input.find_first_not_of(" \t\r\n");
    auto r = input.find_last_not_of(" \t\r\n");
    if (l == std::string::npos) return false;
    input = input.substr(l, r - l + 1);

    if (!isNumber(input)) return false;
    try {
        value = std::stoi(input);
    } catch (...) {
        return false;
    }
    return true;
}

bool Game::playerChoice() {
    std::cout << "Что пожелаешь, повелитель?" << std::endl;

    int futureAcres = acres_;
    int futureWheat = wheat_;

    // ---- ПОКУПКА ЗЕМЛИ ----
    int acresToBuy = 0;
    if (!readValue("Сколько акров земли повелеваешь купить? ", acresToBuy)) {
        negativeInputMsg();
        return false;
    }

    if (acresToBuy < 0) { negativeInputMsg(); return false; }
    if (static_cast<long long>(acresToBuy) * acrePrice_ > futureWheat) {
        notEnoughWheatMsg();
        return false;
    }

    futureAcres += acresToBuy;
    futureWheat -= acresToBuy * acrePrice_;

    // ---- ПРОДАЖА ЗЕМЛИ ----
    int acresToSell = 0;
    if (!readValue("Сколько акров земли повелеваешь продать? ", acresToSell)) {
        negativeInputMsg();
        return false;
    }

    if (acresToSell < 0) { negativeInputMsg(); return false; }
    if (acresToSell > futureAcres) {
        notEnoughtAcresMsg();
        return false;
    }

    futureAcres -= acresToSell;
    futureWheat += acresToSell * acrePrice_;

    // ---- СЪЕСТЬ ПШЕНИЦУ ----
    int wheatToEat = 0;
    if (!readValue("Сколько бушелей пшеницы повелеваешь съесть? ", wheatToEat)) {
        negativeInputMsg();
        return false;
    }

    if (wheatToEat < 0) { negativeInputMsg(); return false; }
    if (wheatToEat > futureWheat) {
        notEnoughWheatMsg();
        return false;
    }

    futureWheat -= wheatToEat;

    // ---- ЗАСЕВ ----
    int acresToSeed = 0;
    if (!readValue("Сколько акров земли повелеваешь засеять? ", acresToSeed)) {
        negativeInputMsg();
        return false;
    }

    if (acresToSeed < 0) { negativeInputMsg(); return false; }
    if (acresToSeed > futureAcres) {
        notEnoughtAcresMsg();
        return false;
    }

    if (acresToSeed * 5 > futureWheat) {
        notEnoughWheatMsg();
        return false;
    }

    if (population_ * 10 < acresToSeed) {
        notEnoughPeopleMsg();
        return false;
    }

    futureWheat -= acresToSeed * 5;

    // ---- ПРИМЕНЕНИЕ ИЗМЕНЕНИЙ ----
    wheat_ = futureWheat;
    acres_ = futureAcres;
    plantedAcres_ = acresToSeed;
    eatenWheat_ = wheatToEat;

    return true;
}


bool Game::nextYear() {
    calcAcrePrice();
    calcWheatPerAcre();
    calcPlague();          // определим, была ли чума
    calcEatenByRats();     // пересчёт по текущим запасам + урожай (см. реализацию)
    calcStarvedPeople();

    // Обновляем статистику: процент умерших (от населения в начале года)
    int percentThisYear = 0;
    if (population_ > 0) percentThisYear = starvedPeople_ * 100 / population_;
    totalStarvedPercent_ += percentThisYear;

    if (starvedPeople_ > static_cast<int>(population_ * 0.45)) {
        std::cout << "За год умерло " << starvedPeople_ << " человек!!!\n";
        std::cout << "Из-за Вашего ужасного управления Вас не только подвергли импичменту и вышвырнули из города,\n";
        std::cout << "но и назначили врагом государства!!!!\n";
        return false;
    }
    calcNewPeople();
    updateMainInfo();
    if (population_ == 0) {
        std::cout << "Ваше правление каким-то образом привело к смерти всего населения...\n";
        return false;
    }
    return true;
}


// Сохранение и загрузка прогресса

void Game::saveProgress(int currentYear) {
    json j;

    j["population"] = population_;
    j["acres"] = acres_;
    j["wheat"] = wheat_;
    j["plantedAcres"] = plantedAcres_;
    j["eatenWheat"] = eatenWheat_;
    j["totalStarvedPercent"] = totalStarvedPercent_;
    j["year"] = currentYear;

    // сохраняем состояние генератора
    // std::ostringstream oss;
    // oss << gen;
    // j["generator"] = oss.str();

    std::ofstream file("save.json");
    if (file.is_open()) {
        file << j.dump(4); // красивое форматирование
        file.close();
        std::cout << "Прогресс сохранён.\n";
    }
}

bool Game::loadProgress() {
    std::ifstream file("save.json");
    if (!file.is_open()) return false;

    json j;
    file >> j;

    population_ = j["population"];
    acres_ = j["acres"];
    wheat_ = j["wheat"];
    plantedAcres_ = j["plantedAcres"];
    eatenWheat_ = j["eatenWheat"];
    totalStarvedPercent_ = j["totalStarvedPercent"];
    year_ = j["year"];

    // восстановление генератора
    // std::istringstream iss(j["generator"].get<std::string>());
    // iss >> gen;

    std::cout << "Прогресс загружен. Продолжаем с года " << year_ << ".\n";
    return true;
}


}