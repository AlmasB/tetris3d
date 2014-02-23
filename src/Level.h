#ifndef __LEVEL_H__
#define __LEVEL_H__

#include <string>
#include <list>
#include <utility>
#include <memory>

#include "GameMath.h"
#include "Debug.h"

#define _NL "\n"

#define __MAX_LEVELS 5
#define __DEFAULT_HEIGHT 3

#define __LEVEL1_WIDTH 5
#define __LEVEL1_LENGTH 25

#define __LEVEL2_WIDTH 9
#define __LEVEL2_LENGTH 40

static const std::string LEVEL1_DATA[__LEVEL1_LENGTH] = {
	"00000",
	"00000",
	"00000",
	"00000",
	"00000",

	"00000",
	"00000",
	"10001",
	"00000",
	"10101",

	"00100",
	"01110",
	"00100",
	"01010",
	"00000",

	"00000",
	"00000",
	"00000",
	"00000",
	"00000",

	"00000",
	"10001",
	"11011",
	"11011",
	"11011"
};

class Level {
	private:
		static std::list<std::shared_ptr<Level>> levels;
		Level(int, int, int, int, const std::string[]);
		static void createLevels();
		static uint numberOfLevels;
	public:
		~Level();
		int number, width, height, length;
		bool ** data;

		static std::shared_ptr<Level> getNext();
		static uint getNumberOfLevels();
};

#endif