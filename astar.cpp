#include "main.h"
#include <iostream>
#include <set>

using namespace std;

#define map_size_row 10
#define map_size_col 10

typedef pair<int, int> Pair;
typedef pair<int, pair<int, int>> pPair;

struct cell {
	int x;
	int y;
	int f, g, h;
};


bool isValid(int x, int y) {
	if ((0 <= x) && (x < map_size_row) && (0 <= y) && (y < map_size_col)) {
		return true;
	}
	else {
		return false;
	}

}


bool isObstacle(int grid[][map_size_col ], int x, int y) {
	if (grid[x][y] == 0 || !isValid(x,y)) {
		return true;
	}
	else {
		return false;
	}
}

bool isGoal(int x, int y, Pair goal) {
	if (x == goal.first && y == goal.second) {
		return true;
	}
	else {
		return false;
	}
}

int hvalue(int x, int y, Pair dest) {// manhattan distance
	return abs(dest.first - x) + abs(dest.second - y);
}

void drawPath(int grid[][map_size_col], cell map[][map_size_col], Pair start, Pair goal) {
	// Create a copy of the grid
	int visual_grid[map_size_row][map_size_col];
	for (int i = 0; i < map_size_row; i++) {
		for (int j = 0; j < map_size_col; j++) {
			visual_grid[i][j] = grid[i][j];
		}
	}

	// Trace back the path and mark it on the visual grid
	int x = goal.first;
	int y = goal.second;
	while (map[x][y].x != x || map[x][y].y != y) {
		visual_grid[x][y] = 2; // Mark the path
		int temp_x = map[x][y].x;
		int temp_y = map[x][y].y;
		x = temp_x;
		y = temp_y;
	}

	// Mark the start and goal points
	visual_grid[start.first][start.second] = 3; // Start point
	visual_grid[goal.first][goal.second] = 4;   // Goal point

	// Draw the visual grid
	for (int i = 0; i < map_size_row; ++i) {
		for (int j = 0; j < map_size_col; ++j) {
			switch (visual_grid[i][j]) {
			case 0:
				cout << "# "; // Obstacle
				break;
			case 1:
				cout << "  "; // Empty cell
				break;
			case 2:
				cout << "* "; // Path
				break;
			case 3:
				cout << "S "; // Start
				break;
			case 4:
				cout << "G "; // Goal
				break;
			default:
				break;
			}
		}
		cout << endl;
	}
}

void astarsearch(int grid[][map_size_col], Pair start, Pair goal) {
	if (isObstacle(grid,start.first, start.second) || isObstacle(grid,goal.first, goal.second)) {
		cout<<"not Valid start or goal";
		return;
	}

	if (isGoal(start.first, start.second, goal)) {
		cout << "start==goal";
		return;
	}

	cell map[map_size_row][map_size_col];
	for (int i = 0; i < map_size_row; i++) {
		for (int j = 0; j < map_size_col; j++) {
			map[i][j].f = map_size_row * map_size_col;
			map[i][j].g = map_size_row * map_size_col;
			map[i][j].h = map_size_row * map_size_col;
			map[i][j].x = -1;
			map[i][j].y = -1;
		}
	}

	map[start.first][start.second].x = start.first;
	map[start.first][start.second].y = start.second;
	map[start.first][start.second].g = 0;
	map[start.first][start.second].h = hvalue(start.first,start.second,goal);
	map[start.first][start.second].f = hvalue(start.first,start.second,goal);

	bool closed_list[map_size_row][map_size_col];
	memset(closed_list, false, sizeof(closed_list));

	set<pPair> openlist;
	
	openlist.insert(make_pair(0, make_pair(start.first, start.second)));

	while (!openlist.empty()) {
		pPair f_openlist = *openlist.begin();
		openlist.erase(openlist.begin());

		int i = f_openlist.second.first;
		int j = f_openlist.second.second;

		closed_list[i][j] = true;
		int fnew, gnew, hnew;

		if (!isObstacle(grid, i - 1, j)) {
			if (isGoal(i - 1, j, goal)) {
				map[i - 1][j].x = i;
				map[i - 1][j].y = j;

				cout << "found goal "<<endl;
				cout << "f : " << map[i][j].g + 1<<endl;
				drawPath(grid, map, start, goal);
				return;
			}

			if (!closed_list[i - 1][j]) {
				gnew = map[i][j].g + 1;
				hnew = hvalue(i - 1, j, goal);
				fnew = gnew + hnew;
				if (map[i - 1][j].x == -1) {
					map[i - 1][j].g = gnew;
					map[i - 1][j].h = hnew;
					map[i - 1][j].f = fnew;
					map[i - 1][j].x = i;
					map[i - 1][j].y = j;
					openlist.insert(make_pair(map[i - 1][j].f, make_pair(i - 1, j)));
				}
				else {
					if (fnew < map[i - 1][j].f) {
						openlist.erase(make_pair(map[i - 1][j].f, make_pair(i - 1, j)));
						map[i - 1][j].g = gnew;
						map[i - 1][j].h = hnew;
						map[i - 1][j].f = fnew;
						map[i - 1][j].x = i;
						map[i - 1][j].y = j;
						openlist.insert(make_pair(map[i - 1][j].f, make_pair(i - 1, j)));
					}
				}
			}
		}

		if (!isObstacle(grid, i + 1, j)) {
			if (isGoal(i + 1, j, goal)) {
				map[i + 1][j].x = i;
				map[i + 1][j].y = j;

				cout << "found goal"<<endl;
				cout << "f : " << map[i][j].g + 1<<endl;
				drawPath(grid, map, start, goal);
				return;
			}

			if (!closed_list[i + 1][j]) {
				gnew = map[i][j].g + 1;
				hnew = hvalue(i + 1, j, goal);
				fnew = gnew + hnew;
				if (map[i + 1][j].x == -1) {
					map[i + 1][j].g = gnew;
					map[i + 1][j].h = hnew;
					map[i + 1][j].f = fnew;
					map[i + 1][j].x = i;
					map[i + 1][j].y = j;
					openlist.insert(make_pair(map[i + 1][j].f, make_pair(i + 1, j)));
				}
				else {
					if (fnew < map[i + 1][j].f) {
						openlist.erase(make_pair(map[i + 1][j].f, make_pair(i + 1, j)));
						map[i + 1][j].g = gnew;
						map[i + 1][j].h = hnew;
						map[i + 1][j].f = fnew;
						map[i + 1][j].x = i;
						map[i + 1][j].y = j;
						openlist.insert(make_pair(map[i + 1][j].f, make_pair(i + 1, j)));
					}
				}
			}
		}

		if (!isObstacle(grid, i , j-1)) {
			if (isGoal(i, j-1, goal)) {
				map[i][j-1].x = i;
				map[i][j-1].y = j;

				cout << "found goal "<<endl;
				cout << "f : " << map[i][j].g+1<<endl;
				drawPath(grid, map, start, goal);
				return;
			}

			if (!closed_list[i][j-1]) {
				gnew = map[i][j].g + 1;
				hnew = hvalue(i, j-1, goal);
				fnew = gnew + hnew;
				if (map[i][j-1].x == -1) {
					map[i][j-1].g = gnew;
					map[i][j-1].h = hnew;
					map[i][j-1].f = fnew;
					map[i][j-1].x = i;
					map[i][j-1].y = j;
					openlist.insert(make_pair(map[i][j-1].f, make_pair(i, j-1)));
				}
				else {
					if (fnew < map[i][j-1].f) {
						openlist.erase(make_pair(map[i][j-1].f, make_pair(i, j-1)));
						map[i][j-1].g = gnew;
						map[i][j-1].h = hnew;
						map[i][j-1].f = fnew;
						map[i][j-1].x = i;
						map[i][j-1].y = j;
						openlist.insert(make_pair(map[i][j-1].f, make_pair(i, j-1)));
					}
				}
			}
		}

		if (!isObstacle(grid, i, j + 1)) {
			if (isGoal(i, j + 1, goal)) {
				map[i][j+1].x = i;
				map[i][j+1].y = j;


				cout << "found goal "<<endl;
				cout << "f : " << map[i][j].g + 1<<endl;
				drawPath(grid, map, start, goal);
				return;
			}

			if (!closed_list[i][j + 1]) {
				gnew = map[i][j].g + 1;
				hnew = hvalue(i, j + 1, goal);
				fnew = gnew + hnew;
				if (map[i][j + 1].x == -1) {
					map[i][j + 1].g = gnew;
					map[i][j + 1].h = hnew;
					map[i][j + 1].f = fnew;
					map[i][j + 1].x = i;
					map[i][j + 1].y = j;
					openlist.insert(make_pair(map[i][j + 1].f, make_pair(i, j + 1)));
				}
				else {
					if (fnew < map[i][j + 1].f) {
						openlist.erase(make_pair(map[i][j + 1].f, make_pair(i, j + 1)));
						map[i][j+1].g = gnew;
						map[i][j+1].h = hnew;
						map[i][j+1].f = fnew;
						map[i][j+1].x = i;
						map[i][j+1].y = j;
						openlist.insert(make_pair(map[i][j + 1].f, make_pair(i, j + 1)));
					}
				}
			}
		}

	}

	cout << "no path found";
	return;
}




int main() {
	int grid[map_size_row][map_size_col]
		= { { 1, 0, 1, 1, 1, 1, 0, 1, 1, 1 },
			{ 1, 1, 1, 0, 1, 1, 1, 0, 1, 1 },
			{ 1, 1, 1, 0, 1, 1, 1, 1, 0, 1 },
			{ 0, 0, 1, 0, 1, 0, 0, 1, 1, 1 },
			{ 1, 1, 1, 0, 1, 1, 1, 0, 1, 0 },
			{ 1, 0, 1, 1, 1, 1, 0, 0, 1, 0 },
			{ 1, 0, 0, 1, 0, 1, 0, 0, 1, 1 },
			{ 1, 0, 1, 1, 1, 0, 1, 1, 1, 1 },
			{ 1, 1, 1, 0, 1, 0, 1, 0, 0, 1 },
			{ 1, 1, 1, 0, 1, 1, 1, 0, 0, 1 } };


	Pair start = make_pair(4, 0);
	Pair goal = make_pair(9, 9);
	astarsearch(grid, start, goal);
}


