#include "Level.h"

std::list<std::shared_ptr<Level>> Level::levels;
int Level::numberOfLevels;

Level::Level(int value, int w, int h, int l, const std::string levelData[]) : number(value), width(w), height(h), length(l) {
	data = new bool* [width];

	for (int i = 0; i < width; ++i)
		data[i] = new bool[length];

	for (int i = 0; i < width; ++i) {
		for (int j = 0; j < length; ++j) {
			data[i][j] = levelData[length-j-1][i] == '1';
		}
	}
}

Level::~Level() {
	for (int i = 0; i < width; ++i)
		delete[] data[i];
	delete[] data;

#ifdef __DEBUG
	debug("Level::~Level() finished");
#endif
}

void Level::createLevels() {
	levels.push_back(std::shared_ptr<Level>(new Level(1, __LEVEL1_WIDTH, __DEFAULT_HEIGHT, __LEVEL1_LENGTH, LEVEL1_DATA)));
	levels.push_back(std::shared_ptr<Level>(new Level(2, __LEVEL2_WIDTH, __DEFAULT_HEIGHT, __LEVEL2_LENGTH, LEVEL2_DATA)));
	levels.push_back(std::shared_ptr<Level>(new Level(3, __LEVEL3_WIDTH, __DEFAULT_HEIGHT, __LEVEL3_LENGTH, LEVEL3_DATA)));
	levels.push_back(std::shared_ptr<Level>(new Level(4, __LEVEL4_WIDTH, __DEFAULT_HEIGHT, __LEVEL4_LENGTH, LEVEL4_DATA)));
	//levels.push_back(std::shared_ptr<Level>(new Level(5, __LEVEL5_WIDTH, __DEFAULT_HEIGHT, __LEVEL5_LENGTH, LEVEL5_DATA)));

	numberOfLevels = levels.size();
}

std::shared_ptr<Level> Level::getNext() {
	if (levels.empty())	// running for first time or someone forgot to check max level...
		createLevels();	// in either cases we promise not to crash but feed u same levels

	std::shared_ptr<Level> level = levels.front();
	levels.erase(levels.begin());
	return level;
}

//TODO: maybe count levels, certainly don't return that
int Level::getNumberOfLevels() {
	return numberOfLevels;
}

