#include "controller.hxx"
#include <iostream>

Controller::Controller(int board_state)
        :
        model_(board_state),
        view_(model_)
{ }

void Controller::draw(ge211::Sprite_set& sprites)
{
    if (model_.winner() == Player::neither) {
        view_.draw(sprites, from_pos_, to_pos_);
    } else {
        view_.winner_result(sprites);
    }
}


ge211::Dims<int> Controller::initial_window_dimensions() const
{
    return view_.window_dimensions();
}


std::string Controller::initial_window_title() const
{
    return view_.window_title();
}


void Controller::on_mouse_up(ge211::Mouse_button, ge211::Posn<int> pos)
{
    int b_pos = view_.screen_to_board(pos);

    if (from_pos_ == -2) {
        if(to_pos_ == -2) {
            if(!model_.find_moves_(b_pos).empty()) {
                from_pos_ = b_pos;
            } else {
                return;
            }
        }
    }
    else if (to_pos_ == -2 ) {
        to_pos_ = b_pos;
        model_.play_move(from_pos_, to_pos_);
        from_pos_ = -2;
        to_pos_ = -2;
    }
}
