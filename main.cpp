//
//  main.cpp
//  lifetool
//
//  Created by Oleksii Stepanenko on 03/08/2018.
//  Copyright © 2018 Oleksii Stepanenko. All rights reserved.
//

#include <iostream>
#include <stdlib.h>
#include <limits.h>
#include "life.hpp"

int main(int argc, const char * argv[]) {
	
	if (argc < 6)
	{
		printf("Usage: life <width> <height> <count> <filename> <resultFilename>\n");
		return -1;
	}
	
	size_t width = ::strtol(argv[1], NULL, 10);
	size_t height = ::strtol(argv[2], NULL, 10);
	int count = (int)::strtol(argv[3], NULL, 10);
	const char* filename = argv[4];
	const char* resulFilename = argv[5];
	
	life::Game game(width, height);
	game.Load(filename);
	for (int i = 0; i < count; ++i)
		game.GenerateNextTurn();
	
	game.Save(resulFilename);
	game.Load(resulFilename);
	
	return 0;
}
