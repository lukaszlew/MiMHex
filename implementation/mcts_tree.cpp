#include "mcts_tree.h"
#include "conditional_assert.h"

namespace Hex {

const uint MCTSTree::default_max_depth = -1;
const uint MCTSTree::default_playouts_per_move = 100000;
const uint MCTSTree::ultimate_depth = kBoardSize * kBoardSize;
const uint MCTSTree::visits_to_expand = 10;

inline MCTSTree::MCTSTree() : current_player(Player::First()) {
	Reset();
	max_depth = default_max_depth;
	playouts_per_move = default_playouts_per_move;
}

inline void MCTSTree::ClearTree() {
	root = new MCTSNode();
	root_children_number = 0;
}

inline void MCTSTree::Reset() {
	ClearTree();
}

Move MCTSTree::BestMove(Player player, Board& board) {

	ClearTree();

	ASSERT (!board.IsFull());
	ASSERT (root != NULL);

	MCTSNode* current_node;
	uint current_level;
	typedef MCTSNode* mcts_node_ptr;
	mcts_node_ptr path[ultimate_depth + 1];
	uint full_path[ultimate_depth + 1];
	path[0] = root.GetPointer();
	Board brd;

	if (root->children == NULL) {
		root->Expand(board);
		root_children_number = board.MovesLeft();
	}

	for (uint i = 0; i < playouts_per_move; ++i) {

		current_level = 0;
		current_node = root.GetPointer();
		brd.Load(board);
		while (current_node->children != NULL) {
			if (max_depth == 0)
				current_node = &current_node->children[Rand::next_rand(brd.MovesLeft())];
			else current_node = current_node->FindChildToSelect(brd.MovesLeft());
			brd.PlayLegal(Move(brd.CurrentPlayer(), current_node->loc));
			path[++current_level] = current_node;
			full_path[current_level] = current_node->loc.GetPos();
		}

		if (current_level < max_depth && brd.MovesLeft() > 0 &&
				current_node->uct_stats.played >= visits_to_expand +
				2 * Params::initialization) {
			current_node->Expand(brd);
			current_node = current_node->FindChildToSelect(brd.MovesLeft());
			brd.PlayLegal(Move(brd.CurrentPlayer(), current_node->loc));
			path[++current_level] = current_node;
			full_path[current_level] = current_node->loc.GetPos();
		}

		Player current = brd.CurrentPlayer();
		Player winner = RandomFinish(brd, full_path, current_level);

		for (int level = current_level; level >= 0; --level) {
			if (winner != current)
				path[level]->uct_stats.won++;
			path[level]->uct_stats.played++;
			path[level]->SetInvalidUCB();
			current = current.Opponent();
		}

		for (int level = board.MovesLeft(); level > 0; --level) {
			uint pos = full_path[level];
			int tree_level = current_level - 1;
			if (tree_level >= level)
				tree_level = level - 1;
			else if (((level + tree_level) & 1) == 0)
				tree_level--;
			if ((tree_level & 1) == 0)
				current = player;
			else current = player.Opponent();
			while (tree_level >= 0) {
				MCTSNode* updated = path[tree_level]->pos_to_children_mapping[pos];
				if (winner == current)
					updated->rave_stats.won++;
				updated->rave_stats.played++;
				updated->SetInvalidRAVE();
				tree_level -= 2;
			}
		}
	}

	MCTSNode* best = root->FindBestChild(board.MovesLeft());
	current_player = current_player.Opponent();

	return Move(player, best->loc);
}

Player MCTSTree::RandomFinish(Board& board, uint* path,
		uint current_level) {

	while (!board.IsFull()) {
	  Player pl = board.CurrentPlayer();
	  Move move = board.RandomLegalMove(pl);
	  path[++current_level] = move.GetLocation().GetPos();
	  board.PlayLegal(move);
	}

	return board.Winner();
}

void MCTSTree::SetMaxDepth(uint depth) {
	if (depth > ultimate_depth)
		max_depth = ultimate_depth;
	else max_depth = depth;
}

void MCTSTree::SetPlayoutsPerMove(uint playouts) {
	playouts_per_move = playouts;
}

std::string MCTSTree::ToAsciiArt(uint children) {
	std::stringstream stream;
	root->RecursivePrint(stream, children, 0, root_children_number, current_player);
	return stream.str();
}

} // namespace Hex
