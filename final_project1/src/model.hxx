#pragma once

#include <ge211.hxx>
#include "dice.hxx"
#include "board.hxx"
#include "player.hxx"
#include <vector>


class Model
{
public:
    //default
    explicit Model();

    //gives us a particular board setup
    Model(int board_state);

    //takes in 2 dice numbers and the given player.
    Model(int dice1, int dice2, Player yo);

    //takes in 2 dice numbers, given player, and board_state
    Model(int dice1, int dice2, Player, int board_state);

    //returns whether the game is finished. This is true when one player has
    // all their pieces removed from the endzone.
    bool is_game_over() const;

    //returns the current board
    Board board() const;

    //returns the current player's turn
    Player turn() const;

    //returns the number on dice 1
    int dice_number_1() const;

    //returns the number on dice 2
    int dice_number_2() const;

    //returns whether dice 1 is active (i.e. has been used or not)
    bool dice_number_1_status() const;

    //returns whether dice 2 is active (i.e. has been used or not)
    bool dice_number_2_status() const;

    //returns who the winner of the game is
    Player winner() const;

    //returns whether the current player has any possible moves
    bool no_more_moves() const;

    //this vector will return a list of the possible that the current player
    //can make at the checker position that he clicks on.
    std::vector<int> find_moves_(int pos) const;

    //plays the move, if possible.
    void play_move(int pos_from, int pos_to);

private:
    Player turn_;
    Player winner_ = Player::neither;
    Board board_;
    Dice dice_;

    bool no_more_moves_ = false;

    // ******
    //Look at HW 6 about Helpers. Can use it as a general outline for how the
    //helpers are associated with each other and to the public methods.
    // *******

    //This returns whether all the current player's checkers
    //are in the end zone. If so, then they can proceed to move checkers
    //off the board.
    bool all_in_final_() const;

    //This is used for the end zone. It returns whether the dice value rolled
    //is greater than or equal to the current player' piece location in the
    //end zone. Helper to evaluate_position
    bool endzone_die_(int dice_num) const;

    //Returns whether the current player can move a checker from its current
    //location (pos_from) to the desired location (pos_to). Helper to
    // find_moves_ method.
    bool evaluate_position_(int position_from, int position_to) const;

    bool evaluate_position_main_helper(int dice_state, std::vector<bool>
    comparisons) const;

    //Helper method to evaluate_position. It returns a vector of booleans
    // which determines what moves are feasible for a checker, given the
    // numbers rolled by the dice.
    std::vector<bool> evaluate_position_helper(int position_from, int
    position_to) const;



    //This is a helper for the find_moves_ method. Searches all the possible
    //moves in the given direction. If direction = 1, that means search
    // clockwise. Direction = -1, means counter-clockwise
    std::vector<int> find_moves_helper_(int position_start) const;

    //This is a helper for advance_turn_. Looks to see if the checker
    //has no moves
    bool no_next_moves_();

    //This is used to change the turn to the other player, if the game isn't
    // over. It will also roll the dice for the next player
    void advance_turn_();


    //This is a helper to play_move.Plays the current player's desired move.
    void really_play_move_(int position_from, int position_to);

    //This is a helper to play_move. After a move is played
    // (really_play_move_), we must set the dice, associated to this move, to
    // false/inactive so that it can't be used again by the current player.
    void inactivate_die(int position_from, int position_to);

    //Helper to inactivate_die. It determines what die to set inactivate
    // given the desired move by the player
    void inactivate_die_helper(int dice, int position_from, int position_to);

    friend struct Test_access;

};

