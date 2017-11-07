#include "utils.h"

// Calcola la distanza tra 2 punti
int distance(int x1, int y1, int x2, int y2) {
	int dis;
	int xd = x1 - x2;
	int yd = y1 - y2;

	if (xd < 0)
		xd = -xd;
	if (yd < 0)
		yd = -yd;

	int temp = pow(xd, 2) + pow(yd, 2);
	dis = sqrt(temp);

	return dis;
}

float distance(float x1, float y1, float x2, float y2) {
	float dis;
	float xd = x1 - x2;
	float yd = y1 - y2;

	if (xd < 0)
		xd = -xd;
	if (yd < 0)
		yd = -yd;

	int temp = pow(xd, 2) + pow(yd, 2);
	dis = sqrt(temp);

	return dis;
}

float distance(cv::Point2f p1, cv::Point2f p2) {
	float dis;
	
	float xd = p1.x - p2.x;
	float yd = p1.y - p2.y;

	if (xd < 0)
		xd = -xd;
	if (yd < 0)
		yd = -yd;

	float temp = pow(xd, 2) + pow(yd, 2);
	dis = sqrt(temp);

	return dis;

}

bool sortByDistance(const triple& a, const triple& b) {
	return a.d < b.d;
}