//
// Created by Andrew Bai on 5/25/22.
//

#ifndef GAME_BOARD_HXX
  #define GAME_BOARD_HXX

#endif //GAME_BOARD_HXX

#pragma once
#include "player.hxx"
#include <ge211.hxx>
#include <vector>

class Board
{
public:
    //Constructs the default board, with all positions having 0 pieces
    // if init is false, and with the proper initial setup if init is
    // true
    Board(int board_state);

    //returns the number of pieces at the given position on the board.
    int num_pieces(int pos) const;

    //returns the player at the given position on the board.
    Player player(int pos) const;


    //removes a piece from the given position on the board.
    void remove_piece(int pos);

    //adds a piece to the given position for the given player.
    void add_piece(int pos, Player player);

    //returns number of end zone pieces for given player
    int num_endzoned(Player player) const;

    //returns a vector of the positions of the pieces in the player's final
    // stretch for the given player
    std::vector<int> pos_final(Player) const;

private:
    //stores the number of pieces and player
    struct pos_info
    {
        int number_pieces;
        Player player;
        bool pieces_present;
    };

    //a vector which stores a pos_info struct in each position. The array
    // index corresponds to the position on the board, 0-23
    // dark's endzone is position 0, light's is 25
    std::vector<pos_info> positions_{26,
                                     {0, Player::neither, false}};
};
