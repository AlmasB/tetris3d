#include "Level.h"

std::list<std::shared_ptr<Level>> Level::LEVELS;

Level::Level(int w, int h, int l, const std::string levelData[]) : width(w), height(h), length(l) {
	data = new bool* [width];

	for (uint i = 0; i < width; ++i)
		data[i] = new bool[length];

	for (uint i = 0; i < width; ++i) {
		for (uint j = 0; j < length; ++j) {
			data[i][j] = levelData[j][i] == '1';
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

void initLevels() {
	//Level level1(5, 3, 25, LEVEL1_DATA);
	//Level::LEVELS[1] = level1;
	Level::LEVELS.push_back(std::make_shared<Level>(5, 3, 25, LEVEL1_DATA));






	/*Level level1;
	level1.width = 5;
	level1.height = 3;
	level1.length = 25;
	for (unsigned int i = 0; i < __LEVEL1_LENGTH; ++i)
		level1.data.push_back(LEVEL1_DATA[i]);

	Level::LEVELS[1] = level1;*/
}