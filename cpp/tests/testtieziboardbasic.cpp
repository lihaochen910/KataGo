#include "../tests/tests.h"

using namespace std;
using namespace TestCommon;

void Tests::runTieZiBoardBasicTests() {
  cout << "Running tiezi board basic tests" << endl;
  ostringstream out;

  //============================================================================
  {
    const char* name = "TieZiBoard";
    TieZiBoard board = TieZiBoard::parseBoard(9,9,R"%%(
........x
.........
....o....
.........
.........
.........
.........
.........
........A
)%%");

    out << endl;
    for(int y = 0; y<board.y_size; y++) {
      for(int x = 0; x<board.x_size; x++) {
        Loc loc = Location::getLoc(x,y,board.x_size);
        if(board.rules[loc] != TZ_NONE)
          out << Global::intToString(board.getRuleOfLocation(loc));
        else
          out << ".";
      }
      out << endl;
    }
    out << endl;

    string expected = R"%%(
........1
.........
....2....
.........
.........
.........
.........
.........
........3
)%%";
    expect(name,out,expected);
  }
}