#ifndef DRAW_H
#define DRAW_H

#include <sstream>
#include "library.h"
#include "PriorityQueue.h"

struct Colors
{
	int water;
	int path;
	int land;

	Colors()
	{
		water = make_color_int(0, 64, 128);
		path = make_color_int(220, 20, 60);
		land = make_color_int(0, 0, 0);
	}
};

struct MapHeader {
    int rows;
    int columns;
    int bytesperpix;
    int secondsperpix;
    int leftLongSeconds;
    int topaLatSeconds;
    int minValue;
    int maxValue;
    int water;
};

void drawPath(PathNode*,MapHeader&);
int get_elevation_color(int);
void setup_map(ifstream&,MapHeader&);
MapHeader read_header(ifstream& fin);
bool compareCoords(const Intersection*, const Intersection*);
string find_smallest_map(const Intersection*, const Intersection*);
void draw_map(PathNode* minPath, const Intersection*, const Intersection*);
#endif