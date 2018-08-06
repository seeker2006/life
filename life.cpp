//
//  life.cpp
//  Life
//
//  Created by Oleksii Stepanenko on 03/08/2018.
//  Copyright © 2018 Oleksii Stepanenko. All rights reserved.
//

#include "life.hpp"
#include <fstream>
#include <string>
#include <iostream>

namespace life
{
	Game::Game(size_t inWidth, size_t inHeight, bool isClosed) : width(inWidth), height(inHeight), closed(isClosed) {
		world = std::make_shared<Matrix>();
		world->resize(width);
		for (auto x = 0; x < width; ++x)
			(*world)[x].resize(height);
		nextWorld = std::make_shared<Matrix>(*world);
	}

	bool Game::IsAlive(int x, int y) const {
		return (x < width && y < height) ? (*world)[x][y] : false;
	}
	
	void Game::SetAlive(int x, int y, bool isAlive) {
		if (x < width && y < height)
			(*world)[x][y] = isAlive;
	}

	size_t Game::CountNeightbors(int x, int y) {
		size_t n = 0;
		int left = x > 0 ? x - 1 : closed ? (int)width - 1 : -1;
		int top = y > 0 ? y - 1 : closed ? (int)height - 1 : -1;
		int bottom = y < height - 1 ? y + 1 : closed ? 0 : -1;
		int right = x < width - 1 ? x + 1 : closed ? 0 : -1;
		int neigbors[8][2] = {
			{left, top}, {x, top}, {right, top},
			{left, y}, /* skip self*/ {right, y},
			{left, bottom}, {x, bottom}, {right, bottom}
		};
		for (auto& pt : neigbors) {
			if (IsAlive(pt[0], pt[1]))
				++n;
		}
		return n;
	}
	
	void Game::GenerateNextTurn() {
		// Generate next state
		std::vector<std::thread> pool;
		for (auto y = 0; y < height; ++y) {
			pool.push_back(std::thread([y, this]() {
				for (auto x = 0; x < width; ++x) {
					size_t n = CountNeightbors(x,y);
					bool oldAlive = IsAlive(x, y);
					bool newAlive = oldAlive;
					if (n == 3)
						newAlive = true;
					else if (n < 2 || n > 3)
						newAlive = false;
					(*nextWorld)[x][y] = newAlive;
				}
			}));
		}
		
		// wait for complete
		for (auto& t : pool)
			t.join();
		
		// Swap world
		world.swap(nextWorld);
	}
	
	void Game::Load(const char* filePath)
	{
		std::fstream f(filePath, std::ios::in);
		if (!f.is_open())
			return;

		std::string line;
		for (auto y = height; y > 0 ; --y)
		{
			if (f.good())
				std::getline(f, line);
			else
				break;
			for (auto x = 0; x < width; ++x)
			{
				(*world)[x][y - 1] = ((line.length() > x && line.at(x) == 'X') ? true : false);
			}
			line.clear();
		}
	}
	
	void Game::Save(const char* fileName)
	{
		std::fstream f(fileName, std::ios::out);
		if (!f.is_open())
			return;

		size_t n_spaces;
		for (auto y = height; y > 0 ; --y)
		{
			n_spaces = 0;
			for (auto x = 0; x < width; ++x)
			{
				bool alive = (*world)[x][y - 1];
				if (alive)
				{
					// write spaces only when needed, extra strip
					for (auto i = 0; i < n_spaces; ++i)
						f << " ";
					f << "X";
					n_spaces = 0;
				}
				else
					++n_spaces;
			}
			f << std::endl;
		}
	}
}

