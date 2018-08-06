//
//  life.hpp
//  Life
//
//  Created by Oleksii Stepanenko on 28/07/2018.
//  Copyright © 2018 Oleksii Stepanenko. All rights reserved.
//

#pragma once

#include <vector>
#include <thread>
#include <memory>

namespace life
{
	using MatrixRow = std::vector<bool>;
	using Matrix = std::vector<MatrixRow>;
	
	class Game
	{
	public:
		Game(size_t inWidth, size_t inHeight, bool isClosed = true);
		
		bool IsAlive(int x, int y) const;
		void SetAlive(int x, int y, bool isAlive);
		size_t GetWidth() const { return width; }
		size_t GetHeight() const { return height; }

		void GenerateNextTurn();
		
		void Save(const char* filePath);
		void Load(const char* filePath);
		
	protected:
		size_t CountNeightbors(int x, int y);
	private:
		size_t width;
		size_t height;
		std::shared_ptr<Matrix> world;
		std::shared_ptr<Matrix> nextWorld;
		
		bool closed;
	};
}
