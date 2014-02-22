#include "Level.h"

std::list<std::shared_ptr<Level>> Level::levels;

Level::Level(int value, int w, int h, int l, const std::string levelData[]) : number(value), width(w), height(h), length(l) {
	data = new bool* [width];

	for (uint i = 0; i < width; ++i)
		data[i] = new bool[length];

	for (uint i = 0; i < width; ++i) {
		for (uint j = 0; j < length; ++j) {
			data[i][j] = levelData[length-j-1][i] == '1';
			//std::cout << data[i][j];
		}

		//std::cout << std::endl;
	}

	//getchar();

	/*int** ary = new int*[sizeX];
	for (int i = 0; i < sizeX; ++i)
		ary[i] = new int[sizeY];*/
}

Level::~Level() {
	for (uint i = 0; i < width; ++i)
		delete[] data[i];
	delete[] data;
}

void Level::createLevels() {
	levels.push_back(std::shared_ptr<Level>(new Level(1, __LEVEL1_WIDTH, __DEFAULT_HEIGHT, __LEVEL1_LENGTH, LEVEL1_DATA)));

}

std::shared_ptr<Level> Level::getNext() {
	if (levels.size() == 0)	// running for first time or someone forgot to check max level...
		createLevels();	// in either cases we promise not to crash but feed u same levels

	std::shared_ptr<Level> level = levels.front();
	levels.erase(levels.begin());
	return level;
}