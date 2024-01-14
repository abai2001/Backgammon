#include "controller.hxx"
#include "board.cxx"
#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

int main(int argc, char* argv[])
try {
    int board_state;
    switch (argc) {
    case 1:
        board_state = 1;
        break;
    case 2:
        board_state = stoi(argv[1]);
        break;
    default:
        cerr << "Usage: " << argv[0] << "[BOARD INITIALIZER]\n";
        return 1;
    }
    Controller(board_state).run();
}

catch (exception const& e) {
    cerr << argv[0] << ": " << e.what() << "\n";
    return 1;
}
/*int
main()
{
    Controller().run();

    return 0;
}*/

