#include "../game/tieziboard.h"
#include "../game/board.h"

#include <cassert>
#include <cstring>
#include <iostream>

using namespace std;


//CONSTRUCTORS AND INITIALIZATION----------------------------------------------------------

TieZiBoard::TieZiBoard()
{
  init(19,19);
}

TieZiBoard::TieZiBoard(int x, int y)
{
  init(x,y);
}

TieZiBoard::TieZiBoard(const TieZiBoard& other) {
  x_size = other.x_size;
  y_size = other.y_size;

  memcpy(rules, other.rules, sizeof(TieZiRule) * Board::MAX_ARR_SIZE);
}

void TieZiBoard::init(int xS, int yS)
{
  if(xS < 0 || yS < 0 || xS > Board::MAX_LEN || yS > Board::MAX_LEN)
    throw StringError("TieZiBoard::init - invalid board size: " + Global::intToString(xS) + " x " + Global::intToString(yS));

  x_size = xS;
  y_size = yS;

  for(int i = 0; i < Board::MAX_ARR_SIZE; i++)
    rules[i] = TZ_NONE;

  for(int y = 0; y < y_size; y++)
  {
    for(int x = 0; x < x_size; x++)
    {
      Loc loc = (x+1) + (y+1)*(x_size+1);
      rules[loc] = TZ_NONE;
    }
  }

}

bool TieZiBoard::setRule(Loc loc, TieZiRule tieziType)
{
  if(loc < 0 || loc >= Board::MAX_ARR_SIZE)
    return false;
  if(tieziType != TZ_NONE && tieziType != TZ_BLACK && tieziType != TZ_WHITE && tieziType != TZ_ANY)
    return false;
  
  rules[loc] = tieziType;
  return true;
}

TieZiRule TieZiBoard::getRuleOfLocation(Loc loc) const
{
  if(loc < 0 || loc >= Board::MAX_ARR_SIZE)
    return TZ_ANY;
  
  return rules[loc];
}

bool TieZiBoard::canPlaceBlack(Loc loc) const
{
  if(loc < 0 || loc >= Board::MAX_ARR_SIZE)
    return false;
  return rules[loc] != TZ_BLACK && rules[loc] != TZ_ANY;
}

bool TieZiBoard::canPlaceWhite(Loc loc) const
{
  if(loc < 0 || loc >= Board::MAX_ARR_SIZE)
    return false;
  return rules[loc] != TZ_WHITE && rules[loc] != TZ_ANY;
}

bool TieZiBoard::canPlace(Loc loc, Player movePla) const
{
  if(loc < 0 || loc >= Board::MAX_ARR_SIZE)
    return false;
  if (movePla == P_BLACK && canPlaceBlack(loc))
    return true;
  if (movePla == P_WHITE && canPlaceWhite(loc))
    return true;
  return true; // default shouldn't happen
}

void TieZiBoard::copyFrom(TieZiBoard& other) {
  init(other.x_size, other.y_size);
  for(int y = 0; y < other.y_size; y++) {
    for(int x = 0; x < other.x_size; x++) {
      Loc loc = Location::getLoc(x, y, other.x_size);
      rules[loc] = other.rules[loc];
    }
  }
}

void TieZiBoard::initAvoidMoveUntilByLoc() {
  avoidMoveUntilByLocBlack.resize(Board::MAX_ARR_SIZE);
  avoidMoveUntilByLocWhite.resize(Board::MAX_ARR_SIZE);
  for(int y = 0; y < y_size; y++) {
    for(int x = 0; x < x_size; x++) {
      Loc loc = Location::getLoc(x, y, x_size);
      if(!canPlaceBlack(loc))
        avoidMoveUntilByLocBlack[loc] = 0xFFFF;
      if(!canPlaceWhite(loc))
        avoidMoveUntilByLocWhite[loc] = 0xFFFF;
    }
  }
}

TieZiBoard TieZiBoard::loadFile(int xSize, int ySize, const string& file)
{
  return TieZiBoard::parseBoard(xSize, ySize, Global::readFile(file));
}


TieZiBoard TieZiBoard::parseBoard(int xSize, int ySize, const string& s) {
  return parseBoard(xSize,ySize,s,'\n');
}

TieZiBoard TieZiBoard::parseBoard(int xSize, int ySize, const string& s, char lineDelimiter) {
  TieZiBoard board(xSize,ySize);
  vector<string> lines = Global::split(Global::trim(s),lineDelimiter);

  //Throw away coordinate labels line if it exists
  if(lines.size() == ySize+1 && Global::isPrefix(lines[0],"A"))
    lines.erase(lines.begin());

  if(lines.size() != ySize)
    throw StringError("TieZiBoard::parseBoard - string has different number of board rows than ySize");

  for(int y = 0; y<ySize; y++) {
    string line = Global::trim(lines[y]);
    //Throw away coordinates if they exist
    size_t firstNonDigitIdx = 0;
    while(firstNonDigitIdx < line.length() && Global::isDigit(line[firstNonDigitIdx]))
      firstNonDigitIdx++;
    line.erase(0,firstNonDigitIdx);
    line = Global::trim(line);

    if(line.length() != xSize && line.length() != 2*xSize-1)
      throw StringError("TieZiBoard::parseBoard - line length not compatible with xSize");

    for(int x = 0; x<xSize; x++) {
      char c;
      if(line.length() == xSize)
        c = line[x];
      else
        c = line[x*2];

      Loc loc = Location::getLoc(x,y,board.x_size);
      if(c == '.' || c == ' ' || c == '*' || c == ',' || c == '`' || c == '+')
        board.setRule(loc,TZ_NONE);
      else if(c == 'o' || c == 'O')
        board.setRule(loc,TZ_WHITE);
      else if(c == 'x' || c == 'X')
        board.setRule(loc,TZ_BLACK);
      else if(c == 'a' || c == 'A' || c == u'⊕' || c == '@')
        board.setRule(loc,TZ_ANY);
      else
        throw StringError(string("TieZiBoard::parseBoard - could not parse board character: ") + c);
    }
  }
  return board;
}

void TieZiBoard::printBoard(ostream& out, const TieZiBoard& board) {
  for(int y = 0; y < board.y_size; y++) {
    for(int x = 0; x < board.x_size; x++) {
      Loc loc = Location::getLoc(x,y,board.x_size);
      out << TieZiBoard::ruleToChar(board.rules[loc]);
    }
    out << endl;
  }
}

std::string TieZiBoard::toStringSimple(const TieZiBoard& board, char lineDelimiter) {
  string s;
  for(int y = 0; y < board.y_size; y++) {
    for(int x = 0; x < board.x_size; x++) {
      Loc loc = Location::getLoc(x, y, board.x_size);
      s += TieZiBoard::ruleToChar(board.rules[loc]);
    }
    s += "\n";
  }
  return s;
}

char TieZiBoard::ruleToChar(TieZiRule rule){
  switch(rule) {
    case TZ_BLACK: return 'x';
    case TZ_WHITE: return 'o';
    case TZ_NONE: return '.';
    case TZ_ANY: return 'A';
    default:  return '#';
  }
}