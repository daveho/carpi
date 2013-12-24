#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>

#include "play_sound.h"

int main(int argc, char **argv)
{
	if (argc != 2) {
		std::cerr << "Usage: CarPi <path to sound file>" << std::endl;
		exit(1);
	}
	
	PlaySound ps;
	ps.play(argv[1]);
	
	bool done = false;
	while (!done) {
		std::string cmd;
		std::getline(std::cin, cmd);
		if (cmd == "pause") {
			ps.pause();
		} else if (cmd == "resume") {
			ps.resume();
		} else if (cmd == "quit") {
			ps.stop();
			ps.waitForIdle();
			done = true;
		}
	}
	
	std::cout << "Done!" << std::endl;
	
	return 0;
}
