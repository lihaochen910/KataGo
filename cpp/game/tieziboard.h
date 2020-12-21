/*
 * tieziboard.h
 * 定义了用于铁子规则的棋盘
 * Authors: Kanbaru (original).
 */

#ifndef GAME_TIEZI_BOARD_H_
#define GAME_TIEZI_BOARD_H_

#include "../core/global.h"
#include "../game/board.h"

//TYPES AND CONSTANTS-----------------------------------------------------------------
struct TieZiBoard;

//Color of a point on the board
typedef int8_t TieZiRule;
static constexpr TieZiRule TZ_NONE = 0; // 黑子/白子皆可放置 对应字符: .
static constexpr TieZiRule TZ_BLACK = 1; // 黑子不能放置 对应字符: x
static constexpr TieZiRule TZ_WHITE = 2; // 白子不能放置 对应字符: o
static constexpr TieZiRule TZ_ANY = 3; // 黑子/白子皆不能在指定位置放置 对应字符: a


struct TieZiBoard {

  // Constructors---------------------------------
  TieZiBoard();  //Create TieZiBoard of size (19,19)
  TieZiBoard(int x, int y); //Create TieZiBoard of size (x,y)
  TieZiBoard(const TieZiBoard& other);

  TieZiBoard& operator=(const TieZiBoard&) = default;


  // Functions------------------------------------

  // 设置指定位置的铁子规则
  bool setRule(Loc loc, TieZiRule rule);
  // 获取指定位置的铁子规则
  TieZiRule getRuleOfLocation(Loc loc) const;

  // 指定位置可以放置黑子??
  bool canPlaceBlack(Loc loc) const;
  // 指定位置可以放置白子??
  bool canPlaceWhite(Loc loc) const;
  bool canPlace(Loc loc, Player movePla) const;

  void copyFrom(TieZiBoard& other);
  void initAvoidMoveUntilByLoc();

  static TieZiBoard loadFile(int xSize, int ySize, const std::string& file);
  static TieZiBoard parseBoard(int xSize, int ySize, const std::string& s);
  static TieZiBoard parseBoard(int xSize, int ySize, const std::string& s, char lineDelimiter);
  static void printBoard(std::ostream& out, const TieZiBoard& board);
  static std::string toStringSimple(const TieZiBoard& board, char lineDelimiter);
  static char ruleToChar(TieZiRule rule);

  // Data--------------------------------------------
  int x_size;                  // Horizontal size of board
  int y_size;                  // Vertical size of board
  TieZiRule rules[Board::MAX_ARR_SIZE];  // TieZi type of each location on the board.

  std::vector<int> avoidMoveUntilByLocBlack;
  std::vector<int> avoidMoveUntilByLocWhite;

private:
  void init(int xS, int yS);

};

#endif // GAME_TIEZI_BOARD_H_