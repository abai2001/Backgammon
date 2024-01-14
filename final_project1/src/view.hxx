#pragma once

#include "model.hxx"
#include <vector>
class View
{
public:
    explicit View(Model const& model);


    //pass in the mouse position of the piece we want to move, and the
    // position we want to move it to
    void draw(ge211::Sprite_set&, int from, int to);

    //who the winner is
    void winner_result(ge211::Sprite_set&);

    ge211::Dims<int> window_dimensions() const;

    std::string window_title() const;

    //maps board position to its respective screen position
    ge211::Posn<int> board_to_screen(int) const;

    //maps screen position to its respective board position
    int screen_to_board(ge211::Posn<int>) const;
private:

    bool win_sprite_updated = false;


    ge211::Posn<int> dice_1_pos = {30, 350};
    ge211::Posn<int> dice_2_pos = {30, 410};

    Model const& model_;

    std::vector<ge211::Posn<int>> outermost_pieces{26, {-2, -2}};
    //helper to place the dice
    void dice_placement(ge211::Sprite_set&, int);

    ge211::Image_sprite
            board_sprite{"MyProject.jpg"};

    ge211::Circle_sprite
            black_sprite{24,{0,0,0}};

    ge211::Circle_sprite
            red_sprite{24,{255,0,0}};

    ge211::Rectangle_sprite
            black_endzone_sprite{{75,15}, {0,0,0}};

    ge211::Rectangle_sprite
            red_endzone_sprite{{75,20},{255,0,0}};

    //to show possible moves
    ge211::Rectangle_sprite
            highlight_sprite{{50,15},{25,255,25}};

    ge211::Image_sprite
            dice_side1_sprite    {"dice_side1.png"};

    ge211::Image_sprite
            dice_side2_sprite    {"dice_side2.png"};

    ge211::Image_sprite
            dice_side3_sprite    {"dice_side3.png"};

    ge211::Image_sprite
            dice_side4_sprite    {"dice_side4.png"};

    ge211::Image_sprite
            dice_side5_sprite    {"dice_side5.png"};

    ge211::Image_sprite
            dice_side6_sprite    {"dice_side6.png"};

    ge211::Rectangle_sprite
            msg_box_sprite {{300, 50},
                            {0,0,0}};

    ge211::Font sans85 {"sans.ttf", 85};

    //sans.ttf is included among ge211's built-in resources
    ge211::Font sans25 {"sans.ttf", 25};

    //from lecture on 5/19 ==> need to create these builders and text_Sprite
    //in view.hxx not cxx so not local variables.
    ge211::Text_sprite::Builder final_outcome_editor;
    ge211::Text_sprite::Builder no_moves_editor;

    ge211::Text_sprite no_moves_outcome;
    ge211::Text_sprite winning_outcome;


    ge211::Text_sprite turn_statement_sprite = ge211::Text_sprite("Turn:",
                                                                  sans25);

};

