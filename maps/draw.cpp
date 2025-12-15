#include "include/draw.h"
using namespace std;

// reads in the first row of the .dat file, collecting necessary information
MapHeader read_header(ifstream& fin) {
    MapHeader header;
    string field;
    
    // Read pairs: fieldname value
    while (fin >> field) {
        if (field == "rows") {
            fin >> header.rows;
        }
        else if (field == "columns") {
            fin >> header.columns;
        }
        else if (field == "bytesperpixel") {
            fin >> header.bytesperpix;
        }
        else if (field == "secondsperpixel") {
            fin >> header.secondsperpix;
        }
        else if (field == "leftlongseconds") {
            fin >> header.leftLongSeconds;
        }
        else if (field == "toplatseconds") {
            fin >> header.topaLatSeconds;
        }
        else if (field == "min") {
            fin >> header.minValue;
        }
        else if (field == "max") {
            fin >> header.maxValue;
        }
        else if (field == "specialval") {
            fin >> header.water;
            break;  // Last field, done reading header
        }
    }
    return header;
}

// returns color hue based on the elevation
int get_elevation_color(short elev, MapHeader& header)
{
	int c;
	Colors colors = Colors();
	if(elev == header.water)
	{
		return colors.water;
	}

	double hue = 120.0/360.0; 
	double normalized = (double)(elev - header.minValue) / (double)(header.maxValue - header.minValue);

	double lightness = .2 + (.7 * normalized); // .2 -> .9 range

	return make_color_hls(hue,lightness,1.0);
}

// draws the map pixel by pixel
void setup_map(ifstream& fi, MapHeader& header)
{
	enter_fast_mode();
	for(int i=0;i<header.rows;i++)
	{
		for(int j=0;j<header.columns;j++)
		{
			short elevData;
			int color;
			fi.read((char*)&elevData,2);
			if (fi.gcount() < 2) return;
			color = get_elevation_color(elevData, header);
			set_pixel_color(j,i,color);
		}
	}
	catch_up();
}

// draws the shortest path
void draw_path(PathNode* minPath, MapHeader& header)
{
	vector<PathNode*> path;
	PathNode* curPath = minPath;
	int totalW = minPath->weight;
	double pixelPerDegree = (double)((header.secondsperpix / 60.0) / 60.0);
	double leftMostLongCoord = (double)((header.leftLongSeconds / 60.0) / 60.0);
	double northMostLatCoord =(double)((header.topaLatSeconds / 60.0) / 60.0);
	while(curPath != NULL)
	{
		path.push_back(curPath);
		curPath = curPath->parent;
	}
	Colors color = Colors();
	set_pen_color(color.path);
	int i = 0;
	// label start and end points
	int x = (abs(leftMostLongCoord - path[path.size()-1]->intersectionP->longitude)) / pixelPerDegree;
	int y = (abs(northMostLatCoord - path[path.size()-1]->intersectionP->latitude)) / pixelPerDegree;
	move_to(x, y); // start
	write_string(path[path.size()-1]->intersectionP->name);

	x = (abs(leftMostLongCoord - path[0]->intersectionP->longitude)) / pixelPerDegree;
	y = (abs(northMostLatCoord - path[0]->intersectionP->latitude)) / pixelPerDegree;
	move_to(x, y); // end
	write_string(path[0]->intersectionP->name);

	while(path.size() > 0)
	{
		PathNode* cur = path.back();
		path.pop_back();

		Intersection* curInters = cur->intersectionP;
		Road* curRoad = cur->roadP;
		if(path.size() == 0) break;
		// find direction from cur to next stop
		PathNode* next = path.back();
		Intersection* nextInters = next->intersectionP;

		// calculate the start and end point in pixel location (x , y)
		int startx = (abs(leftMostLongCoord - curInters->longitude)) / pixelPerDegree;
		int starty = (abs(northMostLatCoord - curInters->latitude)) / pixelPerDegree;
		int endx = (abs(leftMostLongCoord - nextInters->longitude)) / pixelPerDegree;
		int endy = (abs(northMostLatCoord - nextInters->latitude)) / pixelPerDegree;

		move_to(startx,starty);
		draw_to(endx, endy);
	}
}

void draw_map(PathNode* minPath, const Intersection* startP, const Intersection* endP)
{
	// read binary file
	string path = find_smallest_map(startP, endP);
    ifstream fi(path, ios::in | ios::binary);
	if(!fi.is_open())
	{
		cerr << "Cannot open " << path << " file\n";
		return;
	}
	MapHeader header = read_header(fi);
	fi.seekg(sizeof(short)*header.columns,ios::beg); // go to first line of real data
	 
	// create window and environment
	make_window(header.rows, header.columns);
	set_caption("Oasis Maps");
	setup_map(fi,header);
	draw_path(minPath,header);
}

// returns true if the start and end coords are BOTH in the given range (x1,y1) (x2,y2)
bool compareCoords(const Intersection* startP, const Intersection* endP, float x1, float x2, float y1, float y2)
{
	int lat1 = startP->latitude;
	int long1 = startP->longitude;
	int lat2 = endP->latitude;
	int long2 = endP->longitude;

	if((lat2 >= x2 && lat2 <= x1) && (lat1 <=x1  && lat1 >= x2) && (long1 >= y1 && long1 <= y2) && (long2 <= y2 && long2 >= y1))
	{
		return true;
	}
	return false;
}


string find_smallest_map(const Intersection* startP, const Intersection* endP)
{
	string covPath = "coverage.txt";
	string line;
	float smallestArea = 9999999.0;
	string bestMap = "";

	ifstream fi(covPath);

	if(!fi.is_open())
	{
		cerr << "Cannot find coverage.txt\n";
		return "";
	}

	while(getline(fi,line))
	{
		stringstream lineS = stringstream(line);
		float x1,x2,y1,y2;
		lineS >> x1 >> x2 >> y1 >> y2;
		
		if(compareCoords(startP, endP, x1, x2, y1, y2))
		{
			int width = abs(x2 - x1);
			int height = abs(y2 - y1);
			int area = width*height;
			if(area < smallestArea) 
			{
				smallestArea = area;
				string map;
				lineS >> map;
				bestMap = map;
			}
		}
	}
	bestMap = "data\\" + bestMap;
	return bestMap;
}



