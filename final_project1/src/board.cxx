//
// Created by Andrew Bai on 5/25/22.
//
#include "board.hxx"
#include "player.hxx"


Board::Board(int board_state)
{
    //tests no_moves_ messages
    if (board_state == 2) {
        positions_[1] = {5, Player::light,true};
        positions_[2] = {5, Player::light,true};
        positions_[3] = {5, Player::light, true};

        positions_[4] = {2, Player::dark,true};
        positions_[5] = {2, Player::dark,true};
        positions_[6] = {2, Player::dark,true};
        positions_[7] = {2, Player::dark,true};
        positions_[8] = {2, Player::dark,true};
        positions_[9] = {5, Player::dark,true};
    } else if (board_state == 0) {
        // for testing logic at end of game

        positions_[0] = {13, Player::dark,true};
        positions_[2] = {2, Player::dark, true};

        positions_[24] = {5, Player::light, true};
        positions_[23] = {10, Player::light, true};

    } else {
        //default setup

        positions_[6]  = {5, Player::dark, true};
        positions_[8]  = {3, Player::dark, true};
        positions_[13] = {5, Player::dark, true};
        positions_[24] = {2, Player::dark, true};

        positions_[19] = {5, Player::light, true};
        positions_[17] = {3, Player::light, true};
        positions_[12] = {5, Player::light, true};
        positions_[1]  = {2, Player::light, true};
    }
}


int Board::num_pieces(int pos) const
{
    return positions_[pos].number_pieces;
}


Player Board::player(int pos) const
{
    return positions_[pos].player;
}


void Board::remove_piece(int pos)
{
    if (num_pieces(pos) <= 1) {
        positions_[pos] = {0, Player::neither, false};
    } else {
        positions_[pos].number_pieces -= 1;
    }
}


void Board::add_piece(int pos, Player thisplayer)
{
    if (player(pos) == thisplayer && positions_[pos].pieces_present) {
        positions_[pos].number_pieces += 1;
    }
    else if (player(pos) == Player::neither && !positions_[pos]
            .pieces_present) {
        positions_[pos].player = thisplayer;
        positions_[pos].number_pieces = 1;
        // NEW
        positions_[pos].pieces_present = true;
    } else {
        throw ge211::Client_logic_error("Can't add piece here");
    }

}

int Board::num_endzoned(Player thisplayer) const
{
    if(thisplayer == Player::neither) {
        return 0;
    }
    if(thisplayer == Player::light && thisplayer != Player::neither) {
        return num_pieces(25);
    }
    if(thisplayer == Player::dark && thisplayer != Player::neither) {
        return num_pieces(0);
    }
    return 0;
}

std::vector<int> Board::pos_final(Player thisplayer) const
{
    std::vector<int> result{};
    if (thisplayer != Player::neither && thisplayer == Player::dark) {
        int step = 1;
        while(step < 7) {
            if(player(step) == thisplayer && num_pieces(step) >= 1) {
                result.push_back(step);
            }
            step++;
        }

    }
    if (thisplayer != Player::neither && thisplayer == Player::light) {
        int step = 24;
        while (step > 18) {
            if(player(step) == thisplayer && num_pieces(step) >= 1) {
                result.push_back(step);
            }
            step--;
        }
    }

    return result;
}