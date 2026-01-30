/*

Title: YAHTZEE
Author: Diana Roar

*/

#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <limits>
#include "Dice.h"
#include "Scorecard.h"
#include "colors.h"

// function declarations
void setTextColor(int color);
void clearScreen();
void rollDice(std::vector<Dice>& diePool);
void sortDiceByValue(std::vector<Dice>& dice);
void printDiceHeader();
void printRolledDice(std::vector<Dice> diePool);
void printSavedDice(std::vector<Dice> savedDice);
void printSelectMenu();
void selectDice(Scorecard player, std::vector<Dice>& diePool, std::vector<Dice>& savedDice, int& turnCounter);
void printDeselectMenu();
void deselectDie(std::vector<Dice>& diePool, std::vector<Dice>& savedDice, int i, int deselect);
void deselectDice(Scorecard player, std::vector<Dice>& diePool, std::vector<Dice>& savedDice);
void autoSelectDice(std::vector<Dice>& diePool, std::vector<Dice>& savedDice);
void resetDice(std::vector<Dice>& diePool);
std::vector<Dice> playerTurn(Scorecard player, std::vector<Dice> diePool, std::vector<Dice> savedDice);
void pauseConsole();

int main()
{
    bool gameOver               = false;
    bool playAgain              = false;
    std::string input           = "";
    std::vector<Dice> diePool   = { (1), (2), (3), (4), (5) };

    std::vector<Dice> savedDice;
    Scorecard player1(1);
    Scorecard player2(2);

    // game intro
    std::cout << "WELCOME TO..." << std::endl;
    setTextColor(colors::red);
    std::cout << "__    __   _       _   _   _______   _____   _____   _____" << std::endl;
    std::cout << "\\ \\  / /  / \\     | | | | |__   __| |___  | |  ___| |  ___|" << std::endl;
    std::cout << " \\ \\/ /  / _ \\    | |_| |    | |       / /  | |___  | |___" << std::endl;
    std::cout << "  \\  /  / /_\\ \\   |  _  |    | |      / /   |  ___| |  ___|" << std::endl;
    std::cout << "  / /  /  ___  \\  | | | |    | |     / /__  | |___  | |___" << std::endl;
    std::cout << " / /  /_/     \\_\\ |_| |_|    |_|    /_____| |_____| |_____|" << std::endl;
    std::cout << "/_/\n" << std::endl;
    setTextColor(colors::white);
    pauseConsole();

    do {
        clearScreen();
        setTextColor(colors::red);
        std::cout << "\nSTARTING NEW GAME...\n" << std::endl;
        setTextColor(colors::white);

        // alternate turns until all score slots are used for each player
        do {
            // player 1
            if (player1.playerHasTurn() == true) {
                player1.turnBanner();
                player1.printBoard();
                pauseConsole();
                clearScreen();

                savedDice = playerTurn(player1, diePool, savedDice);
                clearScreen();

                sortDiceByValue(savedDice);
                printSavedDice(savedDice);
                player1.chooseScore(savedDice);
                player1.printLastScore(player1.scoreType, player1.turnScore);
                player1.updateTotals();
                std::cout << "\n";
                savedDice.clear();
                clearScreen();
            }
            // player 2
            if (player2.playerHasTurn() == true) {
                player2.turnBanner();
                player2.printBoard();
                pauseConsole();
                clearScreen();

                savedDice = playerTurn(player2, diePool, savedDice);
                clearScreen();

                sortDiceByValue(savedDice);
                printSavedDice(savedDice);
                player2.chooseScore(savedDice);
                player2.printLastScore(player2.scoreType, player2.turnScore);
                player2.updateTotals();
                std::cout << "\n";
                savedDice.clear();
                clearScreen();
            }
            // check if more turns exist
            if (player1.playerHasTurn() == false && player2.playerHasTurn() == false) {
                gameOver = true;
            }
        } while (!gameOver);

        // print game results
        setTextColor(colors::red);
        std::cout << "\n\033[7mGAME OVER\033[0m\n" << std::endl;
        setTextColor(colors::white);
        std::cout << "PLAYER 1 FINAL SCORE: \t" << player1.grandTotal << std::endl;
        std::cout << "PLAYER 2 FINAL SCORE: \t" << player2.grandTotal << "\n" << std::endl;

        // choose a winner
        setTextColor(colors::yellow);
        if (player1.grandTotal > player2.grandTotal)
            std::cout << "PLAYER 1 WINS!" << std::endl;
        else if (player1.grandTotal < player2.grandTotal)
            std::cout << "PLAYER 2 WINS!" << std::endl;
        else
            std::cout << "NOBODY WINS" << std::endl;

        // reset game or end program
        setTextColor(colors::white);
        std::cout << "\nDO YOU WANT TO PLAY AGAIN? Y/N:\t";
        std::cin >> input;
        if (input == "Y" || input == "y") {
            playAgain = true;
            player1.clearScores();
            player2.clearScores();
            gameOver = false;
            std::cout << "\n";
        }
        else {
            playAgain = false;
            setTextColor(colors::red);
            std::cout << "\nTHANK YOU FOR PLAYING!\n" << std::endl;
            setTextColor(colors::white);
        }
    } while (playAgain);
    return 0;
}

// set console text color using ANSI escape codes
void setTextColor(int color) {
    std::cout << "\033[" << color << "m";
}

// re-center screen for readability
void clearScreen() {
    std::cout << "\x1B[2J\x1B[H";
}

// cross-platform pause function
void pauseConsole() {
    std::cout << "Press Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

// roll available dice
void rollDice(std::vector<Dice>& diePool) {
    srand(time(0));
    setTextColor(colors::red);
    std::cout << "\n\nROLLING DICE...\n" << std::endl;

    // set dice with random numbers between 1-6
    for (int i = 0; i < diePool.size(); i++) {
        if (diePool[i].dieUsed == false)
            diePool[i].setDieRoll();
    }
}

// sort dice by die value
void sortDiceByValue(std::vector<Dice>& dice) {
    std::sort(dice.begin(), dice.end());
}

// header for d1 - d5
void printDiceHeader() {
    std::cout << "D1\tD2\tD3\tD4\tD5" << std::endl;
    std::cout << "----|-------|-------|-------|------" << std::endl;
}

// print rolled dice
void printRolledDice(std::vector<Dice> diePool) {
    for (int i = 0; i < diePool.size(); i++) {
        if (diePool[i].dieUsed == false) {
            setTextColor(colors::red);
            diePool[i].printDieRoll();
        }
        else
            diePool[i].printEmptyRoll();
    }
}

// sort and print player's current saved dice
void printSavedDice(std::vector<Dice> savedDice) {
    setTextColor(colors::yellow);
    std::cout << "\n\nSAVED DICE:\t";

    if (savedDice.empty()) {
        std::cout << "[NONE]";
    }
    else {
        for (int i = 0; i < savedDice.size(); i++) {
            savedDice[i].printDieRoll();
        }
    }
    setTextColor(colors::white);
}

// selection menu
void printSelectMenu() {
    const int dashCount = 147;
    setTextColor(colors::white);
    std::cout << "\n\n ";

    for (int i = 0; i < dashCount; i++) {
        std::cout << "-";
    }
    std::cout << "\n| 1. Keep D1\t2. Keep D2\t3.Keep D3\t4. Keep D4\t5. Keep D5\t";
    std::cout << "6. Keep ALL\t7. VIEW SCORECARD\t8. DESELECT\t\033[93;4m9. CONTINUE\033[0m | \n ";

    for (int i = 0; i < dashCount; i++) {
        std::cout << "-";
    }
    std::cout << "\n" << std::endl;
}

// choose which dice to save
void selectDice(Scorecard player, std::vector<Dice>& diePool, std::vector<Dice>& savedDice, int& turnCounter) {
    int dieSelect       = 0;
    std::string pInput  = "";
    bool inputValid     = true;

    do {
        std::cout << "ENTER A NUMBER:\t";
        std::cin >> dieSelect;

        // validate user input is an integer
        while (1) {
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                setTextColor(colors::red);
                std::cout << "\nERROR: INVALID INPUT\n" << std::endl;
                setTextColor(colors::white);
                std::cout << "ENTER A NUMBER:\t";
                std::cin >> dieSelect;
            }
            if (!std::cin.fail())
                break;
        }
        int dieIndex = dieSelect - 1;

        switch (dieSelect) {
        case 1:
            savedDice.push_back(diePool[dieIndex]);
            diePool[dieIndex].dieUsed = true;
            break;
        case 2:
            savedDice.push_back(diePool[dieIndex]);
            diePool[dieIndex].dieUsed = true;
            break;
        case 3:
            savedDice.push_back(diePool[dieIndex]);
            diePool[dieIndex].dieUsed = true;
            break;
        case 4:
            savedDice.push_back(diePool[dieIndex]);
            diePool[dieIndex].dieUsed = true;
            break;
        case 5:
            savedDice.push_back(diePool[dieIndex]);
            diePool[dieIndex].dieUsed = true;
            break;
        case 6:
            // keep all & skip remaining rolls
            setTextColor(colors::yellow);
            std::cout << "\nARE YOU SURE YOU WANT TO KEEP ALL? Y/N:\t";
            std::cin >> pInput;
            setTextColor(colors::white);
            if (pInput == "Y" || pInput == "y") {
                autoSelectDice(diePool, savedDice);
                turnCounter = 2;
                break;
            }
            else
                break;
        case 7:
            // view scorecard
            clearScreen();
            player.printBoard();
            pauseConsole();
            clearScreen();
            printSavedDice(savedDice);
            std::cout << "\n\n";
            setTextColor(colors::red);
            printDiceHeader();
            printRolledDice(diePool);
            printSelectMenu();
            break;
        case 8:
            // deselect kept dice to include in next roll
            deselectDice(player, diePool, savedDice);
            clearScreen();
            sortDiceByValue(savedDice);
            printSavedDice(savedDice);
            setTextColor(colors::red);
            std::cout << "\n\n";
            printDiceHeader();
            printRolledDice(diePool);
            printSelectMenu();
            break;
        case 9:
            // continue to next roll
            clearScreen();
            sortDiceByValue(savedDice);
            printSavedDice(savedDice);
            break;
        default:
            setTextColor(colors::red);
            std::cout << "\nERROR: INVALID INPUT\n" << std::endl;
            setTextColor(colors::white);
            break;
        }
    } while (dieSelect != 6 && dieSelect != 9);
}

// deselection menu
void printDeselectMenu() {
    const int dashCount = 129;
    setTextColor(colors::gray);
    std::cout << "\n\n ";

    for (int i = 0; i < dashCount; i++) {
        std::cout << "-";
    }
    std::cout << "\n| 1. Drop D1\t2. Drop D2\t3.Drop D3\t4. Drop D4\t5. Drop D5\t6. Drop ALL\t7. VIEW SCORECARD\t8. RETURN |\n ";

    for (int i = 0; i < dashCount; i++) {
        std::cout << "-";
    }
    std::cout << "\n" << std::endl;
}

// deselect saved dice
void deselectDie(std::vector<Dice>& diePool, std::vector<Dice>& savedDice, int i, int deselect) {
    int dieIdNum = savedDice[i].dieId;
    for (int j = 0; j < diePool.size(); j++) {
        if (diePool[j].dieId == dieIdNum)
            diePool[j].dieUsed = false;
    }
    savedDice.erase(savedDice.begin() + i);
}

// deselect kept die rolls
void deselectDice(Scorecard player, std::vector<Dice>& diePool, std::vector<Dice>& savedDice) {
    int deselect = 0;
    int xCounter;

    do {
        clearScreen();

        // if there are no saved dice, return to main menu
        if (savedDice.empty()) {
            setTextColor(colors::red);
            std::cout << "\n\nTHERE ARE NO MORE SAVED DICE TO DESELECT!";
            setTextColor(colors::yellow);
            std::cout << "\n\nRETURNING TO MAIN MENU...\n" << std::endl;
            setTextColor(colors::white);
            pauseConsole();
            deselect = 8;
            break;
        }
        // print current saved dice that player can deselect
        xCounter = 0;
        sortDiceByValue(savedDice);
        setTextColor(colors::yellow);
        std::cout << "\n\nSAVED DICE:\n" << std::endl;
        printDiceHeader();

        for (int i = 0; i < savedDice.size(); i++) {
            savedDice[i].printDieRoll();
            ++xCounter;
        }
        setTextColor(colors::gray);

        for (int j = xCounter; j < savedDice.size(); j++) {
            std::cout << "X\t";
        }
        printDeselectMenu();
        std::cout << "ENTER A NUMBER:\t";
        std::cin >> deselect;

        // validate user input is an integer
        while (1) {
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                setTextColor(colors::red);
                std::cout << "\nERROR: INVALID INPUT\n" << std::endl;
                setTextColor(colors::white);
                std::cout << "ENTER A NUMBER:\t";
                std::cin >> deselect;
            }
            if (!std::cin.fail())
                break;
        }
        int dieIndex = deselect - 1;

        switch (deselect) {
        case 1:
            deselectDie(diePool, savedDice, dieIndex, deselect);
            break;
        case 2:
            deselectDie(diePool, savedDice, dieIndex, deselect);
            break;
        case 3:
            deselectDie(diePool, savedDice, dieIndex, deselect);
            break;
        case 4:
            deselectDie(diePool, savedDice, dieIndex, deselect);
            break;
        case 5:
            deselectDie(diePool, savedDice, dieIndex, deselect);
            break;
        case 6:
            // deselect all
            savedDice.clear();
            resetDice(diePool);
            setTextColor(colors::white);
            break;
        case 7:
            // view scorecard
            clearScreen();
            setTextColor(colors::white);
            player.printBoard();
            pauseConsole();
            clearScreen();
            printSavedDice(savedDice);
            printDeselectMenu();
            break;
        case 8:
            // return to main
            setTextColor(colors::white);
            break;
        default:
            setTextColor(colors::red);
            std::cout << "\nERROR: INVALID INPUT\n" << std::endl;
            setTextColor(colors::white);
            break;
        }
    } while (deselect != 6 && deselect != 8);
}

// auto-select dice on last roll
void autoSelectDice(std::vector<Dice>& diePool, std::vector<Dice>& savedDice) {
    // automatically add unused dice into saved dice pool
    for (int i = 0; i < diePool.size(); i++) {
        if (diePool[i].dieUsed == false) {
            savedDice.push_back(diePool[i]);
            diePool[i].dieUsed = true;
        }
    }
}

// reset dice
void resetDice(std::vector<Dice>& diePool) {
    for (int i = 0; i < diePool.size(); i++)
        diePool[i].dieUsed = false;
}

// player turn
std::vector<Dice> playerTurn(Scorecard player, std::vector<Dice> diePool, std::vector<Dice> savedDice) {
    // 3 rolls per turn
    const int rollsPerTurn      = 3;
    const int maxDieCapacity    = 5;

    for (int i = 0; i < rollsPerTurn; i++) {
        if (savedDice.size() == maxDieCapacity)
            i = rollsPerTurn - 1;
        else {
            rollDice(diePool);
            printDiceHeader();
            printRolledDice(diePool);
        }
        // User chooses which dice to keep
        if (i < rollsPerTurn - 1) {
            printSelectMenu();
            selectDice(player, diePool, savedDice, i);
        }
        else {
            // automatically keep all dice on 3rd roll
            autoSelectDice(diePool, savedDice);
        }
    }
    return savedDice;
}