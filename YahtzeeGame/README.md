# Console-based Yahtzee in C++
![yahtzee](https://github.com/anaroar/Console-Based-Yahtzee/assets/129563507/b514b0e3-9e5e-479f-bbcc-cab0f1282f8d)
## Overview
The original Yahtzee is a multi-player dice game of both strategy and luck, created by Milton Bradley. It involves rolling a set of dice to try to get a certain combination of numbers in order to obtain the highest score. This program is a two-player console-based version of that, built with C++ in Visual Studio. 
## How to Play
### Objective
The goal is to collect as many points as you can by achieving certain die roll combinations. Each player has their own scorecard, which is used to track which roll combinations have been used up, and if so, how many points were then scored.
### Taking a Turn
The active player will roll a set of five dice, up to three times per turn. All five dice are rolled on the first iteration of rolls for each turn, but a player may choose to "keep" any number of dice, rather than re-roll them, for each roll after that. The player chooses which dice to keep by entering the number that corresponds with the die number they want to keep, followed by the Enter button on their keyboard.

![Screenshot 2023-06-16 113651](https://github.com/anaroar/Yahtzee/assets/129563507/48a9ed34-08df-4449-b09a-07a647a8884f)

![Screenshot 2023-06-16 113712](https://github.com/anaroar/Yahtzee/assets/129563507/9300f206-f1a8-47f5-ad0a-f61aa19a9a1b)

The player may also choose to de-select any number of dice that they previously kept, in order to pursue a different roll combination. They can do so by entering the number that corresponds with the "DESELECT" option shown in that same selection menu. They will then be redirected to a deselection menu. 

![Screenshot 2023-06-16 113730](https://github.com/anaroar/Yahtzee/assets/129563507/87ac0ffa-c8b3-4624-8785-0fcec4df58e6)

The player will be prompted to choose a score after completing their third roll. They will enter a number that corresponds with an unused score slot, which is listed out for the player, and hit Enter. The program calculates how many points were scored and reflects it back to the player, signifying the end of their turn. 

![Screenshot 2023-06-16 113750](https://github.com/anaroar/Yahtzee/assets/129563507/a271ff5f-5f91-4c54-a277-7e438c6da4bf)

If the player chooses a score slot for which they do not meet the criteria to earn points, then they will earn a zero for that slot instead. Sometimes it is more advantageous to opt for a zero in one slot to avoid scoring a zero in another.
## Scoring Points
The scorecards for traditional Yahtzee and for this program are divided into an upper and a lower section. 
### Upper Section
Score slots in the upper section are reserved for combinations of the same number, for a total score of that number.
For example, if after their final roll, the player ends up with *3 6 5 6 6*, they can choose to score 18 points for the "Sixes" category because the three sixes that they rolled will add up to 18.

The upper section also includes an opportunity for bonus points. If the player's upper section totals 63 points or more, they will receive an additional 35 bonus points.
### Lower Section
Score slots in the lower section require the player to obtain specific dice combinations, and give the player varying amounts of points, as shown below:

| Combination | Score Criteria | Point Value | Example |
| ----------- |:--------------:|:-----------:| -------:|
| 3 of a Kind | Get at least 3 dice of the same number | Total of all dice | *3 3 3 4 1* |
| 4 of a Kind | Get at least 4 dice of the same number | Total of all dice | *5 5 5 5 2* |
| Full House | Get 2 dice of one number & 3 dice of another | 25pts | *4 4 6 6 6* |
| Small Straight | Get 4 dice in sequential order | 30pts | *2 3 4 5 5* |
| Large Straight | Get all dice in sequential order | 40pts | *2 3 4 5 6* |
| Yahtzee | Get all dice of the same number | 50pts | *1 1 1 1 1* |
| Chance | Get any combination of numbers | Total of all dice | *2 5 3 1 6* |

If the player has already rolled one Yahtzee, they will be able to earn an additional Yahtzee bonus of 100pts for each Yahtzee scored after the first. However, if the player has already scored a zero for Yahtzee, any future Yahtzee bonuses will be forfeited.
## How to Win
The game ends after every score slot in each scorecard is used. The program alternates turns between players until this is completed, before calculating the players' grand totals and announcing a winner. 

Whichever player scored the most points overall, after adding up both their upper and lower sections, wins the game.
## Building
You can build the solution and try the game yourself using [Visual Studio 2019/2022 Community Edition](https://visualstudio.microsoft.com/downloads/).
