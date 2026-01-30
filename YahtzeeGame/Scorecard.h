#pragma once
#include "colors.h"

constexpr int scoreTypeCount = 13;

void setTextColor(int color);
void sortDiceByValue(std::vector<Dice>& dice);

// class to hold player scores
class Scorecard {
public:
    // initiate player scores      1s,2s,3s,4s,5s,6s,3k,4k,fh,sm,lg,ch,yh
    int scores[scoreTypeCount] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    // track when score slots are used
    bool scoreUsed[scoreTypeCount] = { false, false, false, false, false, false, false, false, false, false, false, false, false };

    // each score type has a corresponding index
    const int ones          = 0;
    const int twos          = 1;
    const int threes        = 2;
    const int fours         = 3;
    const int fives         = 4;
    const int sixes         = 5;
    const int threeKind     = 6;
    const int fourKind      = 7;
    const int fullHouse     = 8;
    const int smStraight    = 9;
    const int lgStraight    = 10;
    const int chance        = 11;
    const int yahtzee       = 12;

    // constructor for player id
    int playerId = 0;
    Scorecard(int id = 0) : playerId(id) {}

    // additional variables
    const int maxDieValue           = 6;
    const int upperSectionThreshold = 6;
    int turnCounter                 = 0;
    int turnScore                   = 0;
    std::string scoreType           = "";
    int scoreSelect                 = 0;
    int upperTotal                  = 0;
    int bonus                       = 0;
    int yahtzeeBonus                = 0;
    int lowerTotal                  = 0;
    int grandTotal                  = 0;
    int rollSum                     = 0;
    const int bonusThreshold        = 63;
    int bonusCountdown              = bonusThreshold;

    // check if player has turn
    bool playerHasTurn() {
        for (int i = 0; i < scoreTypeCount; i++) {
            if (scoreUsed[i] == false)
                return true;
        }
        return false;
    }

    // print turn banner
    void turnBanner() {
        std::cout << " -------------------" << std::endl;
        std::cout << "|  PLAYER " << playerId << "'s TURN  |" << std::endl;
        std::cout << " -------------------" << std::endl;
    }

    // print scoreboard
    void printBoard() {
        std::cout << "\n\t~PLAYER " << playerId << " SCORECARD~\n" << std::endl;
        std::cout << "-----------------------------------------" << std::endl;
        std::cout << "|\t1s (Ones)\t|\t" << printScore(scores, scoreUsed, ones) << "\t|" << std::endl;
        std::cout << "|\t2s (Twos)\t|\t" << printScore(scores, scoreUsed, twos) << "\t|" << std::endl;
        std::cout << "|\t3s (Threes)\t|\t" << printScore(scores, scoreUsed, threes) << "\t|" << std::endl;
        std::cout << "|\t4s (Fours)\t|\t" << printScore(scores, scoreUsed, fours) << "\t|" << std::endl;
        std::cout << "|\t5s (Fives)\t|\t" << printScore(scores, scoreUsed, fives) << "\t|" << std::endl;
        std::cout << "|\t6s (Sixes)\t|\t" << printScore(scores, scoreUsed, sixes) << "\t|" << std::endl;
        std::cout << "|---------------------------------------|" << std::endl;
        std::cout << "|\tBonus (" << bonusCountdown << " more pts)\t" << bonus << "\t|" << std::endl;
        std::cout << "|---------------------------------------|" << std::endl;
        std::cout << "|\tUPPER TOTAL\t|\t" << upperTotal << "\t|" << std::endl;
        std::cout << "|---------------------------------------|" << std::endl;
        std::cout << "|\t3 of a Kind\t|\t" << printScore(scores, scoreUsed, threeKind) << "\t|" << std::endl;
        std::cout << "|\t4 of a Kind\t|\t" << printScore(scores, scoreUsed, fourKind) << "\t|" << std::endl;
        std::cout << "|\tFull House\t|\t" << printScore(scores, scoreUsed, fullHouse) << "\t|" << std::endl;
        std::cout << "|\tSm. Straight\t|\t" << printScore(scores, scoreUsed, smStraight) << "\t|" << std::endl;
        std::cout << "|\tLg. Straight\t|\t" << printScore(scores, scoreUsed, lgStraight) << "\t|" << std::endl;
        std::cout << "|\tChance\t\t|\t" << printScore(scores, scoreUsed, chance) << "\t|" << std::endl;
        std::cout << "|\tYahtzee\t\t|\t" << printScore(scores, scoreUsed, yahtzee) << "\t|" << std::endl;
        std::cout << "|\tYahtzee Bonus\t|\t" << yahtzeeBonus << "\t|" << std::endl;
        std::cout << "|---------------------------------------|" << std::endl;
        std::cout << "|\tLOWER TOTAL\t|\t" << lowerTotal << "\t|" << std::endl;
        std::cout << "|---------------------------------------|" << std::endl;
        std::cout << "|\tGRAND TOTAL\t|\t" << grandTotal << "\t|" << std::endl;
        std::cout << "-----------------------------------------\n" << std::endl;
    }

    // print scores onto scoreboard, using a dash for unused scores
    std::string printScore(int scores[], bool scoreUsed[], int num) {
        std::string score = std::to_string(scores[num]);
        if (scores[num] == 0 && scoreUsed[num] == false)
            return "-";
        else
            return score;
    }

    // re-open yahtzee score slot if bonus is scored
    void checkYahtzeeBonus(std::vector<Dice> savedDice) {
        for (int i = 1; i < savedDice.size(); i++) {
            if (savedDice[0].dieRoll != savedDice[i].dieRoll)
                return;
        }
        if (scoreUsed[yahtzee] != true)
            return;
        else
            scoreUsed[yahtzee] = false;
    }

    // choose score from available score slots
    void chooseScore(std::vector<Dice> savedDice) {
        bool scoreConditionMet = true;
        bool selectionValid = true;
        std::string scoreMenu[scoreTypeCount] = {
            "\n1.  Ones",
            "\n2.  Twos",
            "\n3.  Threes",
            "\n4.  Fours",
            "\n5.  Fives",
            "\n6.  Sixes",
            "\n7.  3 of a Kind",
            "\n8.  4 of a Kind",
            "\n9.  Full House",
            "\n10. Sm Straight",
            "\n11. Lg Straight",
            "\n12. Chance",
            "\n13. Yahtzee"
        };
        // if another yahtzee is scored, yahtzee is available again to select
        checkYahtzeeBonus(savedDice);

        // print available score slots
        std::cout << "\n\nCHOOSE A SCORE:\n";
        for (int i = 0; i < scoreTypeCount; i++) {
            if (scoreUsed[i] == false) {
                std::cout << scoreMenu[i];
            }
        }

        // player chooses a score
        do {
            std::cout << "\n\nENTER A NUMBER:\t";
            std::cin >> scoreSelect;
            selectionValid = validateSelection(scoreSelect);

            // validate user input is an integer
            while (1) {
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    setTextColor(colors::red);
                    std::cout << "\nERROR: INVALID INPUT\n" << std::endl;
                    setTextColor(colors::white);
                    std::cout << "ENTER A NUMBER:\t";
                    std::cin >> scoreSelect;
                }
                if (!std::cin.fail())
                    break;
            }

            // insert selected score into player scorecard
            switch (scoreSelect) {
            case 1:
                scoreType = "ONES";
                rollSum = sumOfOne(savedDice);
                scores[ones] = rollSum;
                turnScore = scores[ones];
                scoreUsed[ones] = true;
                break;
            case 2:
                scoreType = "TWOS";
                rollSum = sumOfOne(savedDice);
                scores[twos] = rollSum;
                turnScore = scores[twos];
                scoreUsed[twos] = true;
                break;
            case 3:
                scoreType = "THREES";
                rollSum = sumOfOne(savedDice);
                scores[threes] = rollSum;
                turnScore = scores[threes];
                scoreUsed[threes] = true;
                break;
            case 4:
                scoreType = "FOURS";
                rollSum = sumOfOne(savedDice);
                scores[fours] = rollSum;
                turnScore = scores[fours];
                scoreUsed[fours] = true;
                break;
            case 5:
                scoreType = "FIVES";
                rollSum = sumOfOne(savedDice);
                scores[fives] = rollSum;
                turnScore = scores[fives];
                scoreUsed[fives] = true;
                break;
            case 6:
                scoreType = "SIXES";
                rollSum = sumOfOne(savedDice);
                scores[sixes] = rollSum;
                turnScore = scores[sixes];
                scoreUsed[sixes] = true;
                break;
            case 7:
                scoreType = "3 OF A KIND";
                scoreConditionMet = validateScore(scoreSelect, savedDice);
                if (scoreConditionMet == true) {
                    rollSum = sumOfAll(savedDice);
                    scores[threeKind] = rollSum;
                }
                else
                    scores[threeKind] = 0;
                turnScore = scores[threeKind];
                scoreUsed[threeKind] = true;
                break;
            case 8:
                scoreType = "4 OF A KIND";
                scoreConditionMet = validateScore(scoreSelect, savedDice);
                if (scoreConditionMet == true) {
                    rollSum = sumOfAll(savedDice);
                    scores[fourKind] = rollSum;
                }
                else
                    scores[fourKind] = 0;
                turnScore = scores[fourKind];
                scoreUsed[fourKind] = true;
                break;
            case 9:
                scoreType = "FULL HOUSE";
                scoreConditionMet = validateScore(scoreSelect, savedDice);
                if (scoreConditionMet == true) {
                    scores[fullHouse] = 25;
                }
                else
                    scores[fullHouse] = 0;
                turnScore = scores[fullHouse];
                scoreUsed[fullHouse] = true;
                break;
            case 10:
                scoreType = "SMALL STRAIGHT";
                scoreConditionMet = validateScore(scoreSelect, savedDice);
                if (scoreConditionMet == true) {
                    scores[smStraight] = 30;
                }
                else
                    scores[smStraight] = 0;
                turnScore = scores[smStraight];
                scoreUsed[smStraight] = true;
                break;
            case 11:
                scoreType = "LARGE STRAIGHT";
                scoreConditionMet = validateScore(scoreSelect, savedDice);
                if (scoreConditionMet == true) {
                    scores[lgStraight] = 40;
                }
                else
                    scores[lgStraight] = 0;
                turnScore = scores[lgStraight];
                scoreUsed[lgStraight] = true;
                break;
            case 12:
                scoreType = "CHANCE";
                rollSum = sumOfAll(savedDice);
                scores[chance] = rollSum;
                turnScore = scores[chance];
                scoreUsed[chance] = true;
                break;
            case 13:
                scoreType = "YAHTZEE";
                scoreConditionMet = validateScore(scoreSelect, savedDice);
                if (scoreConditionMet == true) {
                    if (scores[yahtzee] == 0)
                        scores[yahtzee] = 50;
                    else
                        yahtzeeBonus = yahtzeeBonus + 100;
                }
                else
                    scores[yahtzee] = 0;
                turnScore = scores[yahtzee];
                scoreUsed[yahtzee] = true;
                break;
            default:
                setTextColor(colors::red);
                std::cout << "\nERROR: INVALID INPUT\n" << std::endl;
                setTextColor(colors::white);
                break;
            }
        } while (!selectionValid);
    }

    // check if score selection is valid
    bool validateSelection(int selection) {
        --selection;
        if (scoreUsed[selection] == true) {
            setTextColor(colors::red);
            std::cout << "\nERROR: INVALID SELECTION";
            setTextColor(colors::white);
            return false;
        }
        else
            return true;
    }

    // calculate sum of one type of number (upper section: 1-6)
    int sumOfOne(std::vector<Dice> savedDice) {
        int scoreSum = 0;
        for (int i = 0; i < savedDice.size(); i++) {
            if (savedDice[i].dieRoll == scoreSelect)
                scoreSum = scoreSum + scoreSelect;
        }
        return scoreSum;
    }

    // calculate sum of all dice (3k, 4k, ch)
    int sumOfAll(std::vector<Dice> savedDice) {
        int scoreSum = 0;
        for (int i = 0; i < savedDice.size(); i++) {
            scoreSum = scoreSum + savedDice[i].dieRoll;
        }
        return scoreSum;
    }

    // check if score condition is met
    bool validateScore(int scoreType, std::vector<Dice> savedDice) {
        int conditionCounter = 0;

        if (scoreType == threeKind + 1) {
            // check for 3 of a kind
            for (int i = 1; i < maxDieValue + 1; i++) {
                for (int j = 0; j < savedDice.size(); j++) {
                    if (savedDice[j].dieRoll == i)
                        ++conditionCounter;
                }
                if (conditionCounter > 2)
                    break;
                else
                    conditionCounter = 0;
            }
            if (conditionCounter > 2)
                return true;
            else
                return false;
        }
        else if (scoreType == fourKind + 1) {
            // check for 4 of a kind
            for (int i = 1; i < maxDieValue + 1; i++) {
                for (int j = 0; j < savedDice.size(); j++) {
                    if (savedDice[j].dieRoll == i)
                        ++conditionCounter;
                }
                if (conditionCounter > 3)
                    break;
                else
                    conditionCounter = 0;
            }
            if (conditionCounter > 3)
                return true;
            else
                return false;
        }
        else if (scoreType == fullHouse + 1) {
            // check for full house
            int firstValue  = savedDice[0].dieRoll;
            int secondValue = 0;
            int condCounter = 0;

            // look for second pair value
            for (int i = 0; i < savedDice.size(); i++) {
                if (savedDice[0].dieRoll != savedDice[i].dieRoll) {
                    secondValue = savedDice[i].dieRoll;
                    break;
                }
            }
            // check that pair values are sets of 2 and 3
            for (int j = 0; j < savedDice.size(); j++) {
                if (firstValue == savedDice[j].dieRoll)
                    ++conditionCounter;
                if (secondValue == savedDice[j].dieRoll)
                    ++condCounter;
            }
            if ((conditionCounter == 2 && condCounter == 3) || (conditionCounter == 3 && condCounter == 2))
                return true;
            else
                return false;
        }
        else if (scoreType == smStraight + 1) {
            // check for small straight
            sortDiceByValue(savedDice);
            for (int i = 1; i < savedDice.size(); i++) {
                int j = i - 1;
                if (savedDice[i].dieRoll == savedDice[j].dieRoll + 1)
                    ++conditionCounter;
            }
            if (conditionCounter < 3)
                return false;
            else
                return true;
        }
        else if (scoreType == lgStraight + 1) {
            // check for large straight
            sortDiceByValue(savedDice);
            for (int i = 1; i < savedDice.size(); i++) {
                int j = i - 1;
                if (savedDice[i].dieRoll != savedDice[j].dieRoll + 1)
                    return false;
            }
            return true;
        }
        else if (scoreType == yahtzee + 1) {
            // check for yahtzee
            for (int i = 1; i < savedDice.size(); i++) {
                if (savedDice[0].dieRoll != savedDice[i].dieRoll)
                    return false;
            }
            return true;
        }
        else
            std::cout << "ERROR, INVALID SCORE.";
        return false;
    }

    // print back what player scored each turn
    void printLastScore(std::string score, int value) {
        setTextColor(colors::red);
        std::cout << "\nPLAYER " << playerId << " SCORED " << value << " POINTS FOR " << score << "\n" << std::endl;
        setTextColor(colors::white);
        system("pause");
    }

    // Update score totals
    void updateTotals() {
        int lowerSum = 0;
        int upperSum = 0;

        // update upper section
        for (int i = 0; i < upperSectionThreshold; i++) {
            upperSum = upperSum + scores[i];
        }
        upperTotal = upperSum;
        bonusCountdown = bonusThreshold - upperSum;

        // check for bonus
        if (upperTotal >= bonusThreshold) {
            bonus = 35;
            upperTotal = upperTotal + bonus;
        }

        // update lower section
        for (int j = upperSectionThreshold; j < scoreTypeCount; j++) {
            lowerSum = lowerSum + scores[j];
        }
        lowerTotal = lowerSum + yahtzeeBonus;

        // update grand total
        grandTotal = upperTotal + lowerTotal;
    }

    // clear all scores
    void clearScores() {
        for (int i = 0; i < scoreTypeCount;) {
            scores[i] = 0;
            i++;
        }
        for (int j = 0; j < scoreTypeCount;) {
            scoreUsed[j] = false;
            j++;
        }
        upperTotal      = 0;
        bonus           = 0;
        yahtzeeBonus    = 0;
        lowerTotal      = 0;
        grandTotal      = 0;
        rollSum         = 0;
        bonusCountdown  = bonusThreshold;
    }
};