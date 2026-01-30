#pragma once
#include "colors.h"

constexpr int minDieValue = 1;
constexpr int maxDieValue = 6;

void setTextColor(int color);

// class to generate dice
class Dice {
public:
    int dieRoll     = 0;
    bool dieUsed    = false;

    // override to sort Dice objects by roll value
    bool operator< (const Dice& other) const {
        return dieRoll < other.dieRoll;
    }

    // constructor for die id
    int dieId = 0;
    Dice(int id = 0) : dieId(id) {}

    // set die roll with random number from 1-6
    void setDieRoll() {
        if (dieUsed == false)
            dieRoll = minDieValue + rand() % maxDieValue;
        else
            return;
    }

    // print die roll value if unused
    void printDieRoll() {
        std::cout << dieRoll << "\t";
    }

    // print Xs instead of die values
    void printEmptyRoll() {
        setTextColor(colors::gray);
        std::cout << "X\t";
    }
};