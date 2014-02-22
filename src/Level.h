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

#define __LEVEL1_WIDTH 5
#define __LEVEL1_LENGTH 25

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
	"00000",
	"00000",
	"00000",
	"00000"
};

struct LevelNode {
	int x, y;
	bool used;
};

class Level {
	private:
	public:
		Level(int, int, int, const std::string[]);
		~Level();
		int width, height, length;
		//std::string data[];
		//std::list<std::string> data;

		std::list<std::pair<Point2, bool>> nodes;

		bool ** data;

		static std::list<std::shared_ptr<Level>> LEVELS;
		static void createLevels();
};

void initLevels();

#endif