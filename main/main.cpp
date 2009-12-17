#define INSERT_ASSERTIONS

#include "board.cpp"
#include "playout.cpp"
#include "gtp.cpp"
#include "mcts_node.cpp"
#include "mcts_tree.cpp"
#include "game.cpp"
#include "protocol.h"
#include "protocol.cpp"
#include "params.h"

#include <iostream>
#include <cstdlib>
#include <boost/lexical_cast.hpp>

int main(int argc, char* argv[]) {

	if ((argc & 1) == 0) {
		std::cerr << "Usage: ./hex -param val -param val ..." << std::endl;
		return 1;
	}

	std::map<std::string, std::string> params;
	for (int i = 1; i < argc; i += 2)
		params[argv[i]] = argv[i + 1];

	std::map<std::string, std::string>::const_iterator it;
	if ((it = params.find("-alpha")) != params.end())
		Hex::Params::alpha = boost::lexical_cast<float>(it->second);
	if ((it = params.find("-beta")) != params.end())
		Hex::Params::beta = boost::lexical_cast<float>(it->second);
	if ((it = params.find("-initialization")) != params.end())
		Hex::Params::initialization = boost::lexical_cast<unsigned>(it->second);

	Hex::Protocol protocol;
	protocol.Run(std::cin, std::cout);

	return 0;
}
