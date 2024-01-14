#include "model.hxx"
#include "board.cxx"
#include <catch.hxx>
#include <ge211.hxx>

//
// TODO: Write preliminary model tests.
// **** COMMENTS ******
// These tests should demonstrate your functional requirements.
// ==> 11 functional requirements. They can be met by either playing the
// game or reading a model test. We need at least 5 significant model tests.
// ==> functions #1,2,3,4,5,6,8,9,10,11  can easily be seen by just playing
// the game
// ==> function #7 can still be seen by playing the game but it requires more
//     effort since you need to set up the board in a certain manner, in order
//     to get no possible moves. The Test Case "no possible moves" tests this

struct Test_access
{
    Model& model_;

    Board& board()
    {
        return model_.board_;
    }

    Player& turn()
    {
        return model_.turn_;
    }

    bool evaluate_position(int position_from, int position_to)
    {
        return model_.evaluate_position_(position_from, position_to);
    }

    bool all_in_final()
    {
        return model_.all_in_final_();
    }

    void play_move(int pos_from, int pos_to) {
        model_.play_move(pos_from, pos_to);
    }
};

//This checks to see if the board is initialized correctly
TEST_CASE("Board initialization")
{
    Board board = Board(1);
    //makes sure the board is properly initialized
    CHECK( board.num_pieces(6) == 5 );
    CHECK( board.num_pieces(8) == 3 );
    CHECK( board.num_pieces(13) == 5 );
    CHECK( board.num_pieces(24) == 2 );

    CHECK( board.num_pieces(1) == 2 );
    CHECK( board.num_pieces(12) == 5 );
    CHECK( board.num_pieces(17) == 3 );
    CHECK( board.num_pieces(19) == 5 );

    int num_black = 0;
    int num_red = 0;
    int count = 0;
    while(count < 26) {
        if(board.player(count) != Player::neither) {
            if(board.player(count) == Player::light) {
                num_red += board.num_pieces(count);
            } else {
                num_black += board.num_pieces(count);
            }
        }
        count++;
    }
    CHECK( num_black == 15 );
    CHECK( num_red == 15 );
}


//This checks to see if evaluate_position is working correctly.
//It makes sure that each player can only move their checkers in
//only one direction, among others
TEST_CASE("Check if evaluate_position works for checkers")
{
    Model model(1, 5, Player::dark);
    Test_access t{model};

    //dark goes Counter ClockWise, light goes CLockWise

    //make sure that black can only go counter clock wise
    CHECK_FALSE( t.evaluate_position(13, 14 ));
    //black plays where dark already has a  piece, which is a valid move
    CHECK( t.evaluate_position(13, 8));
    //black tries to play at a position not indicated on the dice, should
    // return false
    CHECK_FALSE( t.evaluate_position(6, 4));
    //black tries to play where light already has a piece,  this should be an
    // invalid move
    CHECK_FALSE( t.evaluate_position(13, 12));

    //light only Clock wise
    t.turn() = Player::light;
    //White tries to move checker to a position that's not allowed by the
    // dice value rolled
    CHECK_FALSE( t.evaluate_position(12, 14));
    //White tries to play a move that's occupied by black
    CHECK_FALSE( t.evaluate_position(12, 13));
    //White plays a move which is unoccupied
    CHECK( t.evaluate_position(17, 18));
}


//This checks to make sure our no_more_moves works. Which recognizes when the
//given player has no possible moves.
TEST_CASE("no possible moves")
{
    //Model model(1, 6, Player::dark, 2);
    Model model(3,4, Player::light, 2);
    Test_access t{model};

    CHECK( t.turn() == Player::light );
    t.play_move(1,4);
    //after ^ this play_move, it should still be light player because
    //can't move from 1 to 4 since position 4 is occupied by black
    CHECK( t.turn() == Player::light);
    //Given the board setup, there should be no possible moves for red (light)
    //player. Thus, the CHECK below should return false which indeed is the case
    //So our no_more_moves is working.
    CHECK(model.no_more_moves() == false);
}

//This tests to see if the model recognizes when a player wins
TEST_CASE("moving checkers off end zone, then identifies winner")
{
    Model model(5, 3, Player::dark, 0);
    Test_access t{model};

    t.play_move(2, 0);
    CHECK_FALSE ( model.board().num_pieces(0) == 15);
    CHECK ( model.board().num_pieces(0) == 14);
    t.play_move(2,0);
    //correctly moves piece into endzone
    CHECK( model.board().num_pieces(0) == 15 );
    //correctly identifies dark as winner
    CHECK( model.winner() == Player::dark );
}

//This checks to see if moving checkers for a given player works properly
TEST_CASE("play move / advance turn")
{
    Model model(1, 4, Player::dark);
    Test_access t{model};

    //not a valid move (white has a piece there)
    t.play_move(13, 12);
    CHECK( t.turn() == Player::dark);

    //not a valid move since black moves counter clockwise
    t.play_move(13, 14);
    CHECK( t.turn() == Player::dark );

    //not a valid move since 4 is not a dice number rolled
    t.play_move(13, 9);
    CHECK( t.turn() == Player::dark );

    //a valid move for black
    t.play_move(24, 20);
    CHECK( t.turn() == Player::dark );
    t.play_move(21, 20);
    CHECK( t.turn() == Player::dark);
}






