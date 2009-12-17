#include "protocol.h"

#define NAME					"Hex by Krzysiek Stefański"
#define VERSION					"0.1"
#define PROTOCOL_VERSION		"2"

namespace Hex {

Protocol::Protocol() {
	gtp.RegisterStatic ("name",				NAME);
	gtp.RegisterStatic ("version",			VERSION);
	gtp.RegisterStatic ("protocol_version",	PROTOCOL_VERSION);
	gtp.Register ("boardsize",				this, &Protocol::CBoardSize);
	gtp.Register ("clear_board",			this, &Protocol::CClearBoard);
	gtp.Register ("play",					this, &Protocol::CPlay);
	gtp.Register ("genmove",				this, &Protocol::CGenMove);
	gtp.Register ("set_max_tree_depth",		this, &Protocol::CSetMaxTreeDepth);
	gtp.Register ("showboard",				this, &Protocol::CShowBoard);
	gtp.Register ("set_playouts_per_move",	this, &Protocol::CSetPlayoutsPerMove);
	gtp.Register ("showtree",				this, &Protocol::CShowTree);
	gtp.Register ("genmove_noplay",			this, &Protocol::CGenMoveNoPlay);
}

void Protocol::Run(std::istream& in, std::ostream& out) {
	gtp.Run(in, out);
}

void Protocol::CBoardSize(Gtp::Io& inout) {
	uint boardSize = inout.Read<uint>();
	inout.CheckEmpty();
	if (boardSize != kBoardSize) {
		std::stringstream err;
		err << "the only supported board size is " << kBoardSize;
		inout.SetError(err.str());
	}
}

void Protocol::CClearBoard(Gtp::Io& inout) {
	inout.CheckEmpty();
	game.ClearBoard();
}

void Protocol::CPlay(Gtp::Io& inout) {
	std::string player = inout.Read<std::string>();
	std::string location = inout.Read<std::string>();
	inout.CheckEmpty();

	if (!Player::ValidPlayer(player) || !Location::ValidLocation(location)) {
		inout.SetError("invalid parameters");
		return;
	}

	Move move = Move(Player::OfString(player), Location::OfCoords(location));
	if (game.IsValidMove(move)) {
		game.Play(move);
	} else {
		std::stringstream err;
		err << "invalid move: " << location;
		inout.SetError(err.str());
	}
}

void Protocol::CGenMove(Gtp::Io& inout) {
	std::string player = inout.Read<std::string>();
	inout.CheckEmpty();

	if (!Player::ValidPlayer(player)) {
		inout.SetError("invalid parameters");
		return;
	}

	if (game.IsFinished()) {
		inout.SetError("game is finished");
		return;
	}

	Move move = game.GenMove(Player::OfString(player));
	game.Play(move);
	inout.out << move.GetLocation().ToCoords();
}

void Protocol::CSetMaxTreeDepth(Gtp::Io& inout) {
	int depth = inout.Read<int>();
	inout.CheckEmpty();
	game.SetMaxUTCTreeDepth(depth);
}

void Protocol::CShowBoard(Gtp::Io& inout) {
	inout.CheckEmpty();
	inout.out << std::endl;
	std::string ascii_board;
	game.PrintBoard(ascii_board);
	inout.out << ascii_board;
}

void Protocol::CSetPlayoutsPerMove(Gtp::Io& inout) {
	uint playouts = inout.Read<uint>();
	inout.CheckEmpty();
	game.SetPlayoutsPerMove(playouts);
}

void Protocol::CShowTree(Gtp::Io& inout) {
	uint children = inout.Read<uint>(4);
	inout.CheckEmpty();
	std::string ascii_tree;
	game.PrintTree(ascii_tree, children);
	inout.out << std::endl << ascii_tree;
}

void Protocol::CGenMoveNoPlay(Gtp::Io& inout) {
	std::string player = inout.Read<std::string>();
	inout.CheckEmpty();
	
	if (game.IsFinished()) {
		inout.SetError("game is finished");
		return;
	}

	Move move = game.GenMove(Player::OfString(player));
	inout.out << move.GetLocation().ToCoords();
}

} // namespace Hex
