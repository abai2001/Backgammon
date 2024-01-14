//
// Created by Andrew Bai on 5/25/22.
//

#ifndef BACKGAMMON_PLAYER_HXX
  #define BACKGAMMON_PLAYER_HXX

#endif //BACKGAMMON_PLAYER_HXX
#pragma once
#include <ge211.hxx>



#pragma once

#include <iostream>

//This comes from the HWs that we've done
// A player or lack thereof.
enum class Player
{
    dark,
    light,
    neither,
};

// Returns the other player.
Player other_player(Player);

// Prints a player in a manner suitable for debugging.
std::ostream& operator<<(std::ostream&, Player);