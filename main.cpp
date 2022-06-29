#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<stdlib.h>
#include<cmath>

std::map<char, std::vector<char>> links {  // Holds all of the linking lines between points.
	{'A', {'B', 'I'}},
	{'B', {'A', 'C', 'D'}},
	{'C', {'B', 'D', 'E'}},
	{'D', {'B', 'C', 'E', 'F'}},
	{'E', {'C', 'D', 'G', 'N', 'P'}},
	{'F', {'D', 'G'}},
	{'G', {'E', 'F', 'H'}},
	{'H', {'G', 'Q'}},
	{'I', {'A', 'J'}},
	{'J', {'I', 'K', 'L'}},
	{'K', {'J', 'L'}},
	{'L', {'J', 'K', 'M', 'N'}},
	{'M', {'L', 'O'}},
	{'N', {'L', 'E'}},
	{'O', {'M', 'P'}},
	{'P', {'E', 'O', 'Q'}},
	{'Q', {'P', 'H'}},
};

std::map<char, std::vector<int>> coordinates {  // Holds the coordinates of all the points on the grid.
	{'A', {0, 0}},
	{'B', {3, 1}},
	{'C', {4, 6}},
	{'D', {6, 4}},
	{'E', {7, 7}},
	{'F', {9, 2}},
	{'G', {10, 7}},
	{'H', {12, 10}},
	{'I', {-1, 6}},
	{'J', {1, 6}},
	{'K', {0, 7}},
	{'L', {3, 8}},
	{'M', {3, 12}},
	{'N', {5, 10}},
	{'O', {4, 17}},
	{'P', {7, 12}},
	{'Q', {10, 15}}
};

struct path {
	char to;  // Desination point.
	char from;  // Starting point.
	float distance;  // Distance travelled in total.
};

// Edit these two values to set the start and end points of the algorithms.
char startPoint = 'A';
char endPoint = 'Q';

std::vector<char> runAlgorithm(bool AStar) {
	std::vector<path> pathStack;
	std::vector<char> visitedSpots;
	int currentIndex = 0;

	// Define first item:
	pathStack.push_back(path());
	pathStack[0].to = startPoint;
	pathStack[0].from = 'Z';
	pathStack[0].distance = 0;

	// Iterate through list:
	bool pastEnd = false;
	while (!pastEnd) {
		path currentItem = pathStack[currentIndex];
		visitedSpots.push_back(currentItem.to);
		for (int i = 0; i < links[currentItem.to].size(); i++) {
			bool goDownRoute = true;

			for (int j = 0; j < visitedSpots.size(); j++) {
				if (links[currentItem.to][i] == visitedSpots[j]) {
					goDownRoute = false;
					break;
				}
			}

			// Calculate distance:
			if (goDownRoute) {
				float dist = sqrt(pow((coordinates[links[currentItem.to][i]][1] - coordinates[currentItem.to][1]), 2) + pow((coordinates[links[currentItem.to][i]][0] - coordinates[currentItem.to][0]), 2)) + currentItem.distance;
				if (AStar) {
					float extraDist = sqrt(pow((coordinates[endPoint][1] - coordinates[links[currentItem.to][i]][1]), 2) + pow((coordinates[endPoint][0] - coordinates[links[currentItem.to][i]][0]), 2));
					dist += extraDist;
				}
				bool newValue = true;
				for (int j = 0; j < pathStack.size(); j++) {
					if (pathStack[j].to == links[currentItem.to][i]) {
						newValue = false;
						if (pathStack[j].distance > dist) {
							pathStack.erase(std::next(pathStack.begin(), j));

							path newPath;
							newPath.distance = dist;
							newPath.from = currentItem.to;
							newPath.to = links[currentItem.to][i];

							pathStack.push_back(newPath);
							bool highPoint = false;
							int id = pathStack.size() - 1;
							while (!highPoint) {
								if (pathStack[id].distance < pathStack[id - 1].distance) {
									path tempPath = pathStack[id - 1];
									pathStack[id - 1] = pathStack[id];
									pathStack[id] = tempPath;
									id--;
									if (id <= 0) {
										highPoint = true;
									}
								}
								else {
									highPoint = true;
								}
							}
						}
					}
				}
				if (newValue) {
					path newPath;
					newPath.distance = dist;
					newPath.from = currentItem.to;
					newPath.to = links[currentItem.to][i];

					pathStack.push_back(newPath);
					bool highPoint = false;
					int id = pathStack.size() - 1;
					while (!highPoint) {
						if (pathStack[id].distance < pathStack[id - 1].distance) {
							path tempPath = pathStack[id - 1];
							pathStack[id - 1] = pathStack[id];
							pathStack[id] = tempPath;
							id--;
							if (id <= 0) {
								highPoint = true;
							}
						}
						else {
							highPoint = true;
						}
					}
				}
			}
		}
		currentIndex += 1;
		// Check if we're done:
		if (pathStack[currentIndex - 1].to == endPoint) {
			pastEnd = true;
		}
	}
	
	std::vector<char> order;
	bool madePath = false;
	char currentSpot = endPoint;
	order.push_back(currentSpot);
	while (!madePath) {
		std::vector<path> detectedPoints;
		for (int i = 0; i < pathStack.size(); i++) {
			if (pathStack[i].to == currentSpot) {
				detectedPoints.push_back(pathStack[i]);
			}
		}
		if (detectedPoints.size() > 0) {
			float bestDistance = 999999.0;
			int bestPoint = 0;
			for (int i = 0; i < detectedPoints.size(); i++) {
				if (detectedPoints[i].distance < bestDistance) {
					bestPoint = i;
					bestDistance = detectedPoints[i].distance;
				}
			}
			order.insert(order.begin(), detectedPoints[bestPoint].from);
			if (detectedPoints[bestPoint].from == startPoint) {
				madePath = true;
			}
			else {
				currentSpot = detectedPoints[bestPoint].from;
			}
		}
		else {
			std::cout << "Error: No path could be made!" << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	return order;
}

bool pointExists(char point) {
	return !(coordinates.find(point) == coordinates.end());
}

int main() {
	if (pointExists(startPoint) && pointExists(endPoint)) {
		std::vector<char> order1 = runAlgorithm(false);  // Dijkstra
		std::vector<char> order2 = runAlgorithm(true);  // A*
		std::cout << "Dijkstra Algorithm: ";
		for (int i = 0; i < order1.size(); i++) {
			std::cout << order1[i];
			if (i < (order1.size() - 1)) {
				std::cout << " --> ";
			}
		}
		std::cout << std::endl;
		std::cout << "A* Algorithm: ";
		for (int i = 0; i < order2.size(); i++) {
			std::cout << order2[i];
			if (i < (order2.size() - 1)) {
				std::cout << " --> ";
			}
		}
		std::cout << std::endl;
	}
	else {
		std::cout << "Error: Either the start or end point doesn't exist on the graph." << std::endl;
	}
}
