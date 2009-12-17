#include "board.h"
#include "conditional_assert.h"
#include <cstring>
#include <sstream>

namespace Hex {

uint Rand::_seed;

// -----------------------------------------------------------------------------

inline Player Player::First() { return Player(0); }

inline Player Player::Second() { return Player(1); }

inline Player Player::OfString (std::string player) {
	ASSERT(ValidPlayer(player));
	if (player == "black")
		return Player::First();
	else return Player::Second();
}

inline Player Player::Opponent() const {
	return Player(_val ^ 1);
}

inline bool Player::operator== (const Player& player) {
	return player._val == _val;
}

inline bool Player::operator!= (const Player& player) {
	return player._val != _val;
}

inline Player::Player(uint val) : _val(val) {}

inline uint Player::GetVal() { return _val; }

inline bool Player::ValidPlayer(const std::string& player) {
	return player == "black" || player == "white";
}

// -----------------------------------------------------------------------------

inline Location Location::OfCoords (std::string coords) {
	ASSERT(ValidLocation(coords));
	uint x = coords[0] >= 'a' ? coords[0] - 'a' : coords[0] - 'A';
	uint y = coords[1] - '0';
	if (coords.size() > 2)
		y = y * 10 + coords[2] - '0';
	return Location(++x, y);
}

inline Location::Location(uint x, uint y) : _pos(ToTablePos(x, y)) { }
inline Location::Location(uint pos) : _pos(pos) {}

inline Location::Location() {}

inline uint Location::GetPos() const { return _pos; }

inline std::string Location::ToCoords() const {
	std::stringstream coords;
	coords << static_cast<char>(_pos % kBoardSizeAligned + 'a' - 1);
	coords << _pos / kBoardSizeAligned;
	return coords.str();
}

inline uint Location::ToTablePos(uint x, uint y) {
	ASSERT (ValidLocation(x, y));
	return y * (kBoardSizeAligned) + x;
}

inline bool Location::operator==(Location loc) const {
	return loc._pos == _pos;
}

inline bool Location::operator!=(Location loc) const {
	return loc._pos != _pos;
}

inline bool Location::ValidLocation(const std::string& location) {
	if (location.size() == 0 || location.size() > 3)
		return false;
	uint x = location[0] >= 'a' ? location[0] - 'a' : location[0] - 'A';
	uint y = location[1] - '0';
	if (location.size() > 2)
		y = y * 10 + location[2] - '0';
	return ValidLocation(++x, y);
}

inline bool Location::ValidPosition(uint pos) {
	uint x, y;
	ToCoords(pos, x, y);
	return ValidLocation(x, y);
}

inline bool Location::ValidLocation(uint x, uint y) {
	return x > 0 && y > 0 && x <= kBoardSize && y <= kBoardSize;
}

inline void Location::ToCoords(uint pos, uint& x, uint& y) {
	x = pos % kBoardSizeAligned;
	y = pos / kBoardSizeAligned;
}

// -----------------------------------------------------------------------------

inline Move::Move (const Player& player, const Location& location)
	: _player(player), _location(location) {}

inline Move::Move() :_player(Player::First()), _location(0) {}

inline Location Move::GetLocation() const { return _location; }

inline Player Move::GetPlayer() const { return _player; }

// -----------------------------------------------------------------------------

const uint Board::guarded_board_size = kBoardSize + 2;
const uint Board::table_size = kBoardSizeAligned * kBoardSizeAligned;

const Board Board::Empty() {

	Board board;

	uint counter = 0;
	for (uint i = 1; i <= kBoardSize; ++i) {
		for (uint j = 1; j <= kBoardSize; j++) {
			uint field = i * kBoardSizeAligned + j;
			board._fast_field_map[counter] = field;
			board._reverse_fast_field_map[field] = counter++;
		}
	}

	for (uint i = 0; i < table_size; i++)
		board._board[i] = 0;
	for (uint i = 1; i <= kBoardSize; ++i)
		board._board[i] = 1;
	for (uint i = (guarded_board_size - 1) * kBoardSizeAligned + 1;
			i < (guarded_board_size - 1) * (kBoardSizeAligned + 1); ++i) {
		board._board[i] = (guarded_board_size - 1) * kBoardSizeAligned + 1;
	}

	return board;
}

inline Player Board::CurrentPlayer() const {
	return _current;
}

Move Board::RandomLegalMove (const Player& player) const {
	return Move(player,
			Location(_fast_field_map[Rand::next_rand(_moves_left)]));
}

inline void Board::PlayLegal (const Move& move) {
	ASSERT(IsValidMove(move));
	uint pos = move.GetLocation().GetPos();
	if (move.GetPlayer() == Player::First()) {
		_board[pos] = pos;
		MakeUnion(pos);
	} else {
		_board[pos] = -1;
	}
	uint fast_map_pos = _reverse_fast_field_map[pos];
	uint replace_pos = _fast_field_map[--_moves_left];
	_fast_field_map[fast_map_pos] = replace_pos;
	_reverse_fast_field_map[replace_pos] = fast_map_pos;
	_current = _current.Opponent();
}

inline void Board::MakeUnion(uint pos) {
	uint rep = MakeUnion(pos, pos + 1);
	rep = MakeUnion(rep, pos - 1);
	rep = MakeUnion(rep, pos - kBoardSizeAligned);
	rep = MakeUnion(rep, pos - kBoardSizeAligned + 1);
	rep = MakeUnion(rep, pos + kBoardSizeAligned);
	MakeUnion(rep, pos + kBoardSizeAligned - 1);
}

uint Board::MakeUnion(uint pos1, uint pos2) {
	return _board[pos2] <= 0 ? pos1 : _board[pos1] = Find(pos2);
}

inline uint Board::Find(uint pos) {
	while (static_cast<uint>(_board[pos]) != pos)
		pos = _board[pos] = _board[_board[_board[pos]]];
	return pos;
}

inline uint Board::ConstFind(uint pos) const {
	while (static_cast<uint>(_board[pos]) != pos)
		pos = _board[pos];
	return pos;
}

inline bool Board::IsFull() const {
	return _moves_left == 0;
}

inline Player Board::Winner() const {
	if (ConstFind(1) ==
		ConstFind((guarded_board_size - 1) * kBoardSizeAligned + 1)) {
			return Player::First();
	}
	else return Player::Second();
}

inline void Board::Load (const Board& board) {
	memcpy(this, &board, sizeof(*this));
}

inline Board::Board() : _moves_left(kBoardSize * kBoardSize),
		_current(Player::First()) {
	Rand::init(time(NULL));
}

inline uint Board::MovesLeft() const {
	return _moves_left;
}

inline void Board::GetPossiblePositions(Board::ushort_ptr& locations) {
	locations = _fast_field_map;
}

std::string Board::ToAsciiArt(Location last_move) const {

	std::stringstream s;
	for (unsigned char x = 'a'; x < 'a' + kBoardSize; ++x)
		s << " " << x;
	s << std::endl;
	for (uint i = 1; i <= kBoardSize; ++i) {
		for (uint j = 1; j < (i < 10 ? i : i - 1); ++j)
			s << " ";
		s << i;
		if (i * kBoardSizeAligned + 1 == last_move.GetPos())
			s << "(";
		else s << " ";
		for (uint j = 1; j <= kBoardSize; ++j) {
			uint pos = i * kBoardSizeAligned + j;
			if (_board[pos] == 0)
				s << ".";
			else if (_board[pos] < 0)
				s << "#";
			else s << "O";
			if (pos == last_move.GetPos())
				s << ")";
			else if (pos + 1 == last_move.GetPos())
				s << "(";
			else s << " ";
		}
		s << i << std::endl;
	}
	for (uint j = 0; j <= kBoardSize; ++j)
		s << " ";
	for (unsigned char x = 'a'; x < 'a' + kBoardSize; ++x)
		s << " " << x;

	return s.str();
}

bool Board::IsValidMove(const Move& move) {
	if (!Location::ValidPosition(move.GetLocation().GetPos()))
		return false;
	return _board[move.GetLocation().GetPos()] == 0;
}

// -----------------------------------------------------------------------------

} // namespace Hex
