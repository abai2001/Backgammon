#include "view.hxx"
#include <algorithm>

View::View(Model const& model)
        : model_(model),
        final_outcome_editor(sans85),
        no_moves_editor(sans85)
{
    no_moves_editor.color({0,0,255});
    no_moves_editor.add_message("No moves. Skipped");
    no_moves_outcome.reconfigure(no_moves_editor);
}

ge211::Dims<int> View::window_dimensions() const
{
    return {1075,800};
}


std::string View::window_title() const
{
    return "Backgammon";
}

void View::draw(ge211::Sprite_set& set, int from, int to)
{
    set.add_sprite(board_sprite, {0, 0});
    //display who's turn it is
    set.add_sprite(turn_statement_sprite, {994, 337}, 1);
    if (model_.turn() == Player::light) {
        set.add_sprite(red_sprite, {994, 384}, 1);
    }
    if (model_.turn() == Player::dark) {
        set.add_sprite(black_sprite, {994, 384}, 1);
    }
    if (model_.no_more_moves()) {
        set.add_sprite(no_moves_outcome, {210, 325}, 3);
    }

    //bottom row
    int i = 1;
    int j = 0;
    while(i < 13) {
        ge211::Posn<int> screen_place = board_to_screen(i);
        int pieces = model_.board().num_pieces(i);
        if(model_.board().player(i) != Player::neither) {
            if (model_.board().player(i) == Player::light) {
                while (j < pieces) {
                    screen_place.y = 728 - (j * 44) - 1;
                    set.add_sprite(red_sprite, screen_place, 1);
                    j++;
                }
                j = 0;
            }
            if (model_.board().player(i) == Player::dark) {
                while (j < pieces) {
                    screen_place.y = 728 - (j * 44) - 1;
                    set.add_sprite(black_sprite, screen_place, 1);
                    j++;
                }
                j = 0;
            }
        }
        //if there's no player at the location, then fill the vector with the
        // first vacant position, which is what screen_pos is initialized as
        outermost_pieces[i] = screen_place;
        i++;
    }
    //top row
    i = 13;
    j = 0;
    while(i < 25) {
        ge211::Posn<int> screen_place = board_to_screen(i);
        int pieces = model_.board().num_pieces(i);
        if(model_.board().player(i) != Player::neither) {
            if (model_.board().player(i) == Player::light) {
                while (j < pieces) {
                    screen_place.y = 28 + (j * 44) + 1;
                    set.add_sprite(red_sprite, screen_place, 1);
                    j++;
                }
                j = 0;
            }
            if (model_.board().player(i) == Player::dark) {
                while (j < pieces) {
                    screen_place.y = 28 + (j * 44) + 1;
                    set.add_sprite(black_sprite, screen_place, 1);
                    j++;
                }
                j = 0;
            }
        }
        outermost_pieces[i] = screen_place;
        i++;
    }

    int count = 0;
    int total = model_.board().num_endzoned(Player::dark);
    while(count < total) {
        set.add_sprite(black_endzone_sprite, {989,474 + (count*14)},1);
        count++;
    }
    int count2 = 0;
    int total2 = model_.board().num_endzoned(Player::light);
    while(count2 < total2) {
        set.add_sprite(red_endzone_sprite,{989,306-(count2*14)},1);
        count2++;
    }

    //the dice
    if (model_.dice_number_1_status()) {
        dice_placement(set, 1);
    }
    if (model_.dice_number_2_status()) {
        dice_placement(set, 2);
    }

    //display the possible moves for the clicked piece
    if ( !(from == -2) && to == -2) {
        for (int move: model_.find_moves_(from)) {
            if (move > 12 && move < 25) {
                set.add_sprite(highlight_sprite,
                               {board_to_screen(move).x,
                                      board_to_screen(move).y -
                                                   15}, 1);
            }
            if (move >= 1 && move < 13) {
                set.add_sprite(highlight_sprite,
                               {board_to_screen(move).x,
                                     board_to_screen(move).y +
                                                   44}, 1);
            }
            if (move == 25) {
                set.add_sprite(highlight_sprite,
                               {board_to_screen(move).x,
                                   board_to_screen(move - 2+1).y -
                                                   15}, 1);
            }
            if (move == 0) {
                //need next move
                set.add_sprite(highlight_sprite,
                               {board_to_screen(move).x,
                                board_to_screen(move + 1).y +
                                                   44}, 1);
            }
        }
    }
}

void View::winner_result(ge211::Sprite_set& set)
{
    set.add_sprite(board_sprite, {0,0});
    if(win_sprite_updated == false) {
        if(model_.winner() != Player::neither) {
            if (model_.winner() == Player::dark) {
                final_outcome_editor.color({0, 0, 0});
                final_outcome_editor.add_message("Black Wins!");
            }
            if (model_.winner() == Player::light) {
                final_outcome_editor.color({255, 0, 0});
                final_outcome_editor.add_message("Red Wins!");
            }
        }
    }
    winning_outcome.reconfigure(final_outcome_editor);
    set.add_sprite(winning_outcome, {364,325}, 1);
    win_sprite_updated = true;
}


ge211::Posn<int> View::board_to_screen(int board_pos) const
{
    //each quadrant of the board has a different iteration
    if(board_pos == 0) {
        return {989, 474};
    }
    if(board_pos == 25) {
        return {989, 306};
    }
    if (board_pos > 0 && board_pos < 7) {
        return {561 + (70 * (6-board_pos+0)), 728};
    }
    if (board_pos > 6 && board_pos < 13) {
        return {117 + (70 * (12-board_pos)), 728};
    }
    if (board_pos > 12 && board_pos < 19) {
        return {117 + (70 * (board_pos-13)), 28};
    }
    if (board_pos > 18 && board_pos < 25) {
        return {561+ (70 * (board_pos-19)), 28};
    }
    return {-2, -2};
}

// screen_to_board function runtime is O(25) = O(1), so runs in constant time
// . I think it runs in constant time because the while loop iterates for a
// total of 25 times. Then, getting piece_pos is also constant time since we
// just get the specific element in the vector. Everything else runs in
// constant time as well. Thus, we arrive at 0(1) for this function.
int View::screen_to_board(ge211::Posn<int> s_pos) const
{

    int result = -2;

    int i = 1;
    while(i < 25) {
        ge211::Posn<int> piece_pos = outermost_pieces[i];
        //checks if screen position is within the bounds of the token sprite
        if (s_pos.x >= piece_pos.x && s_pos.x <= piece_pos.x + 44
            && s_pos.y >= piece_pos.y && s_pos.y <= piece_pos.y + 44) {
            return i;
        }
        i++;
    }
    //red endzone
    if (s_pos.x >= 989 && s_pos.x <= 1039 && s_pos.y <= 320 && s_pos.y >= 124){
        return 25;
    }
    //black endzone
    if (s_pos.x >= 989 && s_pos.x <= 1039 && s_pos.y >= 480 && s_pos.y <= 690) {
        return 0;
    }

    return result;
}

void View::dice_placement(ge211::Sprite_set& set, int dice_num)
{
    ge211::Posn<int> dice_pos = {-1, -1};
    int actual_dice_number[1];

    if (dice_num == 1) {
        actual_dice_number[0] = model_.dice_number_1();
        dice_pos = dice_1_pos;
    }
    if (dice_num == 2) {
        actual_dice_number[0] = model_.dice_number_2();
        dice_pos = dice_2_pos;
    }

    //dice too large
    ge211::Transform dice_transform = ge211::Transform {}.set_scale(0.4);
    if (actual_dice_number[0] == 6) {
        set.add_sprite(dice_side6_sprite, dice_pos, 1, dice_transform);
    }
    if (actual_dice_number[0] == 5) {
        set.add_sprite(dice_side5_sprite, dice_pos, 1, dice_transform);
    }
    if (actual_dice_number[0] == 4) {
        set.add_sprite(dice_side4_sprite, dice_pos, 1, dice_transform);
    }
    if (actual_dice_number[0] == 3) {
        set.add_sprite(dice_side3_sprite, dice_pos, 1, dice_transform);
    }
    if (actual_dice_number[0] == 2) {
        set.add_sprite(dice_side2_sprite, dice_pos, 1, dice_transform);
    }
    if (actual_dice_number[0] == 1) {
        set.add_sprite(dice_side1_sprite, dice_pos, 1, dice_transform);
    }
}
