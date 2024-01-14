#pragma once

#include "model.hxx"
#include "view.hxx"

#include <ge211.hxx>


class Controller : public ge211::Abstract_game
{
public:
    explicit Controller(int board_state);


protected:
    void draw(ge211::Sprite_set& set) override;
    ge211::Dims<int> initial_window_dimensions() const override;
    std::string initial_window_title() const override;
    void on_mouse_up(ge211::Mouse_button, ge211::Posn<int>) override;


private:
    Model model_;
    View view_;

    int from_pos_ = -2;
    int to_pos_ = -2;
};

