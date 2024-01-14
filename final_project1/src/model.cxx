#include "model.hxx"
#include <stdlib.h>

Model::Model() :
        board_(1),
        dice_()
{
    if(dice_.dice1_is_greater()) {
        turn_ = Player::dark;
    } else {
        turn_ = Player::light;
    }
}

Model::Model(int board_state) :
        board_(board_state),
        dice_()
{
    if(board_state == 2) {
        turn_ = Player::dark;
    }
    if (dice_.dice1_is_greater()) {
        turn_ = Player::dark;
    }
    if (!dice_.dice1_is_greater()) {
        turn_ = Player::light;
    }
}

Model::Model(int die_1, int die_2, Player player) :
        turn_(player),
        board_(1),
        dice_(die_1, die_2)
{
}

Model::Model(int die_1, int die_2, Player player, int board_state) :
        turn_(player),
        board_(board_state),
        dice_(die_1, die_2)
{
    if(board_state == 2) {
        turn_ = Player::dark;
    }
    if(dice_.dice1_is_greater()) {
        turn_ = Player::dark;
    }
    if(!dice_.dice1_is_greater()) {
        turn_ = Player::light;
    }
}

bool Model::is_game_over() const
{
    if(board().num_pieces(0) == 15 || board().num_pieces(25) == 15) {
        return true;
    }
    return false;
}


Board Model::board() const
{
    return board_;
}

Player Model::turn() const
{
    return turn_;
}

int Model::dice_number_1() const
{
    return dice_.number_1();
}

int Model::dice_number_2() const
{
    return dice_.number_2();
}

bool Model::dice_number_1_status() const
{
    return dice_.number_1_active();
}

bool Model::dice_number_2_status() const
{
    return dice_.number_2_active();
}


Player Model::winner() const {
    return winner_;
}

bool Model::no_more_moves() const
{
    return no_more_moves_;
}

std::vector<int> Model::find_moves_(int pos) const
{
    if(board_.player(pos) == turn_) {
        if(turn_ != Player::neither) {
            return find_moves_helper_(pos);
        }
    }
    return {};
}

void Model::play_move(int pos_from, int pos_to)
{
    if(winner_ != Player::neither) {
        return;
    } else {
        if(evaluate_position_(pos_from, pos_to)) {
            really_play_move_(pos_from, pos_to);
            inactivate_die(pos_from, pos_to);
            advance_turn_();
        }
    }
}

// HELPER FUNCTIONS

// I think that the runtime for this all_in_final_ method is O(N) because the
// runtime depends on the size of our vector pos_vec. We iterate through
// every element in our vector so thus, we have O(N).
bool Model::all_in_final_() const
{
    std::vector<int> pos_vec = board_.pos_final(turn_);
    int final_count = 0;
    int size = pos_vec.size();
    int count = 0;
    while(count < size) {
        int value = pos_vec[count];
        int piece_count = board_.num_pieces(value);
        final_count += piece_count;
        count++;
    }

    final_count += board_.num_endzoned(turn_);

    return final_count == 15;
}

bool Model::endzone_die_(int dice_number) const
{
    std::vector<int> pos_vector = board_.pos_final(turn_);

    int number_on_die;
    int compare = 0;

    if (dice_number == 1) {
        number_on_die = dice_.number_1() + compare;
    }
    if (dice_number == 2) {
        number_on_die = dice_.number_2() + compare;
    }


    if (turn_ != Player::neither && turn_ == Player::dark) {
        if(!pos_vector.empty()) {
            //the last element in vector is the checker farthest away from
            // end zone
            return (number_on_die + compare) > pos_vector.back();
        }
        return true;
    }
    if (turn_ != Player::neither && turn_ == Player::light) {
        // map 19 to 6, 24 to 1. This way, can correctly compare the dice to
        // the positions
        if(!pos_vector.empty()) {
            int number = std::abs(pos_vector.back() - 25);
            return (number_on_die + compare) > (number + compare);
        }
        return true;
    }
    return false;
}


bool Model::evaluate_position_(int pos_from, int pos_to) const
{
    //can't move a piece out of an end zone
    int one = 1;
    if (pos_from == 0) {
        return false;
    }
    if (pos_from == 25) {
        return false;
    }

    if(turn_ == Player::dark && pos_to >= pos_from) {
        return false;
    }
    if (turn_ == Player::light && pos_to <= pos_from) {
        return false;
    }
    //check if we're even clicking on a valid piece for the given player
    if (board_.player(pos_from) == turn_) {

        //if the other player has more than once piece in pos_to, return false
        if ((board_.num_pieces(pos_to) > 0) &&
            board_.player(pos_to) == other_player(turn_)) {
            return false;
        }
        //not in final
        if (!all_in_final_()) {
            if (pos_to == 0) {
                return false;
            }
            if (pos_to == 25) {
                return false;
            }
            int direction;
            if (turn_ != Player::neither) {
                if (turn_ == Player::dark) {
                    direction = -1 * one;
                }
                if (turn_ == Player::light) {
                    direction = one;
                }
            }
            if (dice_.number_1_active()) {
                int desired1 = pos_from + (dice_number_1() * direction);
                if (dice_.number_2_active()) {
                    int desired2 = pos_from + (dice_number_2() * direction);
                    return (desired1 == pos_to) || (desired2 == pos_to);
                } else {
                    return (desired1 == pos_to);
                }
            }
            else if (dice_.number_2_active()) {
                int desired2 = pos_from + (dice_number_2() * direction);
                return (desired2 == pos_to);
            }
        }
        //******
        //IN FINAL CASE
        //******
        if (all_in_final_()) {
            //both dice active
            std::vector<bool> const result = evaluate_position_helper(pos_from,
                                                                      pos_to);
            if (dice_.number_1_active() && dice_.number_2_active()) {
                if (endzone_die_(1) && endzone_die_(2)) {
                    return evaluate_position_main_helper(4, result);
                } else if (endzone_die_(1)) {
                    return evaluate_position_main_helper(1, result);

                } else if (endzone_die_(2)) {
                    //both active, only dice 2 leq
                    return evaluate_position_main_helper(2, result);
                }
                return evaluate_position_main_helper(3, result);
            } else if (dice_.number_1_active()) {
                //only dice 1 active
                if (endzone_die_(1)) {
                    //only dice 1 active and leq
                    return evaluate_position_main_helper(4, result);
                }
                return evaluate_position_main_helper(2, result);
            } else if (dice_.number_2_active()) {
                //only dice 2 active
                if (endzone_die_(2)) {
                    //only dice 2 active and leq
                    return evaluate_position_main_helper(4, result);
                }
                return evaluate_position_main_helper(1, result);
            } else {
                return false;
            }
        }
    }
    return false;
}

bool Model::evaluate_position_main_helper(int dice_state, std::vector<bool>
result) const {
    if(dice_state == 1) {
        if (turn_ == Player::dark) {
            return result[2];
        }
        if (turn_ == Player::light) {
            return result[3];
        }
    }
    if(dice_state == 2) {
        if (turn_ == Player::dark) {
            return result[0];
        }
        if (turn_ == Player::light) {
            return result[1];
        }
    }
    if(dice_state == 3) {
        if (turn_ == Player::dark) {
            return result[0] || result[2];
        }
        if (turn_ == Player::light) {
            return result[1] || result[3];
        }
    }
    if (dice_state == 4) {
        if(turn_ == Player::dark) {
            return result[4];
        }
        if(turn_ == Player::light) {
            return result[5];
        }
    }
    return false;
}



std::vector<bool> Model::evaluate_position_helper(int position_from, int
position_to) const {
    std::vector<bool> result;
    //result[0]
    bool add = (position_to - position_from == -dice_.number_1());
    result.push_back(add);
    //result[1]
    add = position_to - position_from == dice_.number_1();
    result.push_back(add);
    //result[2]
    add = position_to - position_from == -dice_.number_2();
    result.push_back(add);
    //result[3]
    add = position_to - position_from == dice_.number_2();
    result.push_back(add);
    //result[4]
    bool one = position_to == 0 ;
    bool two = position_from == board_.pos_final(turn_).back();
    add = one && two;
    result.push_back(add);
    //result[5]
    one = position_to == 25;
    add = one && two;
    result.push_back(add);

    return result;
}

std::vector<int> Model::find_moves_helper_(int position_start) const
{
    std::vector<int> result;
    if(turn_ == Player::dark) {
        int count = position_start;
        if (position_start == -1) { //-1 = counter-clockwise
            count = 24;
        }
        for(int i = count; i >=0; i--) {
            if(evaluate_position_(position_start, i)) {
                result.push_back(i);
            }
        }
    }
    if(turn_ == Player::light) {
        int count = position_start;
        while (count < 26) {
            if(evaluate_position_(position_start, count)) {
                result.push_back(count);
            }
            count++;
        }
    }
    if(turn_ == Player::neither) {
        return {};
    }
    return result;
}


bool Model::no_next_moves_()
{
    int count = 1;
    while(count < 25) {
        bool empty = find_moves_(count).empty();
        if(!empty) {
            return false;
        }
        count++;
    }
    return true;
}

void Model::advance_turn_()
{
    int zero = 0;
    no_more_moves_ = false;
    if (is_game_over()) {
        if (board_.num_endzoned(Player::dark) == 15 + zero) {
            winner_ = Player::dark;
        } else if (board_.num_endzoned(Player::light) == 15 + zero) {
            winner_ = Player::light;
        }
        turn_ = Player::neither;

    } else if (!(dice_.number_1_active()) && !(dice_.number_2_active())) {
        turn_ = other_player(turn_);
        dice_.roll_dice();
    } else if (no_next_moves_()) {
        no_more_moves_ = true;
        turn_ = other_player(turn_);
        dice_.roll_dice();
    }
    //for some reason adding this fixes the skipping problem
    if (no_next_moves_()) {
        no_more_moves_ = true;
        turn_ = other_player(turn_);
        dice_.roll_dice();
    }
}


void Model::really_play_move_(int pos_from, int pos_to)
{
    if (board_.player(pos_to) == turn_ || board_.player(pos_to) ==
                                          Player::neither) {
        board_.remove_piece(pos_from+0);
        board_.add_piece(pos_to+0, turn_);
    }
}

void Model::inactivate_die(int position_from, int position_to)
{
    if(all_in_final_()) {
        if (dice_.number_1_active() && dice_.number_2_active()) {
            if (endzone_die_(1) && endzone_die_(2)) {
                dice_.set_inactive(1);
            } else if (endzone_die_(1)) {
                inactivate_die_helper(1, position_from, position_to);
            } else if (endzone_die_(2)) {
                inactivate_die_helper(2, position_from, position_to);
            } else {
                inactivate_die_helper(3, position_from, position_to);
            }
        } else if (dice_.number_1_active() && !dice_.number_2_active()) {
            dice_.set_inactive(1);
        } else if (dice_.number_2_active() && !dice_.number_1_active()) {
            dice_.set_inactive(2);
        }
    }
    if(!all_in_final_()){
        if (dice_.number_1_active() && dice_.number_2_active()) {
            inactivate_die_helper(3, position_from, position_to);
        } else if (dice_.number_2_active() && !dice_.number_1_active()) {
            dice_.set_inactive(2);
        } else if (dice_.number_1_active() && !dice_.number_2_active()) {
            dice_.set_inactive(1);
        }
    }
}

void Model::inactivate_die_helper(int dice, int position_from, int
position_to)
{
    std::vector<bool> compare = evaluate_position_helper(position_from,
                                                         position_to);
    if(dice == 1) {
        if (turn_ == Player::dark) {
            if (compare[2]) {
                dice_.set_inactive(2);
            } else {
                dice_.set_inactive(1);
            }
        }
        if (turn_ == Player::light) {
            if (compare[3]) {
                dice_.set_inactive(2);
            } else {
                dice_.set_inactive(1);
            }
        }
    }
    if(dice == 2) {
        if (turn_ == Player::dark) {
            if (compare[0]) {
                dice_.set_inactive(1);
            } else {
                dice_.set_inactive(2);
            }
        } else if (turn_ == Player::light) {
            if (compare[1]) {
                dice_.set_inactive(1);
            } else {
                dice_.set_inactive(2);
            }
        }
    }
    if(dice == 3) {
        if (turn_ == Player::dark) {
            if (compare[0]) {
                dice_.set_inactive(1);
            } else if (compare[2]) {
                dice_.set_inactive(2);
            }
        } else if (turn_ == Player::light) {
            if (compare[1]) {
                dice_.set_inactive(1);
            } else if (compare[3]) {
                dice_.set_inactive(2);
            }
        }
    }
}





