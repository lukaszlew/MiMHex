#include "board.h"
#include "board.cpp"
#include "playout.cpp"

#include <iostream>
#include <cstdlib>

int playouts_number = 100000;

int main(int argc, char* argv[]) {

	if (argc > 1)
		playouts_number = atoi(argv[1]);
	std::cout << "Starting " << playouts_number << " playouts..." << std::endl;
	uint n = Hex::DoPlayouts(Hex::Board::Empty(), playouts_number);
	std::cout << "First player won: " << n << std::endl;
	std::cout << "Second player won: " << playouts_number - n << std::endl;
	return 0;
}
