#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#include "gtp.hpp"
#include "game.h"

namespace Hex {

class Protocol {
public:
	Protocol();
	void Run(std::istream& in, std::ostream& out);

private:
	Gtp::Repl gtp;
	Game game;

private:
	void CBoardSize(Gtp::Io& inout);
	void CClearBoard(Gtp::Io& inout);
	void CPlay(Gtp::Io& inout);
	void CGenMove(Gtp::Io& inout);
	void CSetMaxTreeDepth(Gtp::Io& inout);
	void CShowBoard(Gtp::Io& inout);
	void CSetPlayoutsPerMove(Gtp::Io& inout);
	void CShowTree(Gtp::Io& inout);
	void CGenMoveNoPlay(Gtp::Io& inout);
};

} // namespace Hex

#endif /* PROTOCOL_H_ */
