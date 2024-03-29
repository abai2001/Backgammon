//
// Created by Andrew Bai on 5/25/22.
//
#include "player.hxx"

// This comes from the HWs that we've done
Player
other_player(Player player)
{
    switch (player) {
    case Player::light:
        return Player::dark;
    case Player::dark:
        return Player::light;
    default:
        return player;
    }
}

std::ostream&
operator<<(std::ostream& os, Player p)
{
    switch (p) {
    case Player::light:
        return os << "L";
    case Player::dark:
        return os << "D";
    default:
        return os << "_";
    }
}