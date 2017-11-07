#pragma once

#include <sstream>
#include <opencv2\core.hpp>

// creo una struct in cui salvare le informazioni per ogni boccia
// coordinate x,y e distanza dal boccino d e colore
struct triple {
	// Coordinate
	int x;
	int y;
	// Distanza
	int d;
	// Stringa che indica il colore
	std::string color;
};

// Funzione che calcola la distanza
int distance(int x1, int y1, int x2, int y2);
float distance(float x1, float y1, float x2, float y2);
float distance(cv::Point2f p1, cv::Point2f p2);

bool sortByDistance(const triple& a, const triple& b);