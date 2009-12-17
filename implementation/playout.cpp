#include "board.h"

namespace Hex {

uint DoPlayouts (const Board& start_board, uint n) {
  Board board;
  uint first_wins = 0;

  for (uint i = 0; i < n; i += 1) {
    board.Load (start_board);

    while (!board.IsFull()) {
      Player pl = board.CurrentPlayer();
      Move move = board.RandomLegalMove (pl);
      board.PlayLegal (move);
    }

    if (board.Winner() == Player::First()) {
      first_wins += 1;
    }
  }

  return first_wins;
}

} // namespace Hex
