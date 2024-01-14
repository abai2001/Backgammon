//
// Created by Andrew Bai on 5/25/22.
//
#pragma once
#include <ge211.hxx>
#include <random>
//#ifndef GAME_DICE_HXX
//#define GAME_DICE_HXX

//#endif //GAME_DICE_HXX
class Dice
{
public:
    //explicit Dice(ge211::Random&);
    //constructs a dice class by randomly generating two ints between 1 and
    // 6, and setting both to active
    explicit Dice();

    //Dice(ge211::Random& rand, int die_1, int die_2);
    // constructs a dice class with dice with numbers die_1 and die_2. ONLY
    // used in testing.
    //Dice(ge211::Random_source<int> rand, int die_1, int die_2);
    Dice(int die_1, int die_2);

    // produces new random ints between 1 and 6
    void roll_dice();

    //returns the 1st die number
    int number_1() const;

    //returns whether or not the 1st die is active
    bool number_1_active() const;

    //returns the 2nd die number
    int number_2() const;

    //returns whether or not the 2nd die is active
    bool number_2_active() const;

    //returns whether or not the 1st dice value is greater than or equal
    // to second
    bool dice1_is_greater() const;
    //sets the given die (1 or 2) inactive, or both (3)
    void set_inactive(int dice_num);

private:
    struct dice_number {
        int number;
        bool active;
    };
    ge211::Random_source<int> random_dice;
    //randomly generated ints from 1-6
    dice_number number_1_;
    dice_number number_2_;


};