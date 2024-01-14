//
// Created by Andrew Bai on 5/25/22.
//
#include "dice.hxx"
#include <stdlib.h>

Dice::Dice() :
        random_dice (1,6)
{
    number_1_ = {random_dice.next_between(1,6), true};
    number_2_ = {random_dice.next_between(1,6), true};
}

Dice::Dice(int die_1, int die_2) :
        random_dice(1,6),
        number_1_({die_1,true}),
        number_2_({die_2, true})
{ }

void Dice::roll_dice()
{
    number_1_ = {random_dice.next_between(1,6), true};
    number_2_ = {random_dice.next_between(1,6), true};
}

int Dice::number_1() const
{
    return number_1_.number;
}

int Dice::number_2() const
{
    return number_2_.number;
}

bool Dice::number_1_active() const
{
    return number_1_.active;
}

bool Dice::number_2_active() const
{
    return number_2_.active;
}

bool Dice::dice1_is_greater() const
{
    return number_1_.number >= number_2_.number;
}

void Dice::set_inactive(int dice_num)
{
    if(dice_num == 1) {
        number_1_.active = false;
        return;
    }
    number_2_.active = false;
}