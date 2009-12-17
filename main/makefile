CXX = g++
OPTIONS = -Wall -W -O3 -fomit-frame-pointer
INCLUDE_DIR = -I ../lib -I ../implementation
DEST_DIR = .

all: benchmark main

benchmark:
	$(CXX) $(OPTIONS) $(INCLUDE_DIR) -o $(DEST_DIR)/hex_benchmark main_benchmark.cpp

main:
	$(CXX) $(OPTIONS) $(INCLUDE_DIR) -o $(DEST_DIR)/hex main.cpp
	
clean:
	rm -f hex hex_benchmark