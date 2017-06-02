#include<opencv2\opencv.hpp>
#include <opencv2\highgui.hpp>
#include <iostream>
#include <sstream>
#include <vector>
#include <string.h>


// Valori di soglia dei colori in hsv
cv::Scalar THRESH_YELLOW[2] = { cv::Scalar(15, 210, 20),  cv::Scalar(35, 255, 255) };
cv::Scalar THRESH_GREEN[2] = { cv::Scalar(31, 125, 125), cv::Scalar(75, 255, 255) };
cv::Scalar THRESH_BLUE[2] = { cv::Scalar(100, 125, 125), cv::Scalar(130, 255, 255) };
cv::Scalar THRESH_RED[2] = { cv::Scalar(170, 125, 125), cv::Scalar(179, 255, 255) };

// creo una struct in cui salvare le informazioni per ogni boccia
// coordinate x,y e distanza dal boccino d
struct triple {
	int x;
	int y;
	int d;
	std::string color;
};

/* ritorna un vector con tutte le immagini per ogni colore
   in questo modo è possibile elaborare ogni boccia
*/
std::vector<cv::Mat> colors(cv::Mat &original){
	std::vector<cv::Mat> vec(5);
	cv::Mat hsv;
	cv::Mat red, yellow, blue, green, black;

	cv::cvtColor(original, hsv, CV_BGR2HSV);

	cv::inRange(hsv, THRESH_YELLOW[0], THRESH_YELLOW[1], yellow);
	cv::inRange(hsv, THRESH_BLUE[0], THRESH_BLUE[1], blue);
	cv::inRange(hsv, THRESH_GREEN[0], THRESH_GREEN[1], green);
	cv::inRange(hsv, THRESH_RED[0], THRESH_RED[1], red);
	cv::inRange(hsv, cv::Scalar(0, 0, 0), cv::Scalar(0, 0, 0), black);

	vec[0] = yellow;
	vec[1] = blue;
	vec[2] = green;
	vec[3] = red;
	vec[4] = black;

	return vec;
}

// Trova il centro delle bocce utilizzando i momenti delle immagini
std::vector<triple> find_centers(cv::Mat &img, cv::Mat &output) {
	std::vector<triple> centers(2);
	cv::Mat blur, edges;
	std::vector< std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	int x, y;

	cv::GaussianBlur(img, blur, cv::Size(3, 3), 3);
	cv::findContours(blur, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

	for (int i = 0; i < contours.size(); i++) {
		cv::Moments M = cv::moments((cv::Mat)contours[i]);
		x = M.m10 / M.m00;
		y = M.m01 / M.m00;
		cv::circle(output, cv::Point(x, y), 5, cv::Scalar(0, 255, 0), -1);
		centers[i].x = x;
		centers[i].y = y;
	}

	return centers;
}

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

bool sortByDistance(const triple& a, const triple& b) {
	return a.d < b.d;
}

// Calcola la distanza delle bocce
// Metto tutto in un vector che verrà poi ordinato
std::vector<triple> getDistance(std::vector<triple>& input, std::vector<triple> boccino) {
	std::vector<triple> bocceSorted(8);

	for (int i = 0; i < input.size(); i++) {
		input[i].d = distance(boccino[0].x, boccino[0].y, input[i].x, input[i].y);
	}

	std::sort(std::begin(input), std::end(input), sortByDistance);
	bocceSorted = input;

	return bocceSorted;
}


int main() {
	cv::Mat original;
	std::vector<cv::Mat> bocce(5);
	std::vector<triple> yellow(2);
	std::vector<triple> blue(2);
	std::vector<triple> red(2);
	std::vector<triple> green(2);
	std::vector<triple> black(2);
	std::vector<triple> bocceTot;
	std::vector<triple> bocceSort;
	

	original = cv::imread("Images/bocce4.png", 1);
	cv::imshow("Test", original);
	cv::waitKey(0);

	bocce = colors(original);
	/*
	cv::imshow("Giallo", bocce[0]);
	cv::imshow("Blu", bocce[1]);
	cv::imshow("Verde", bocce[2]);
	cv::imshow("Rosso", bocce[3]);
	cv::imshow("Boccino", bocce[4]);
	*/


	yellow = find_centers(bocce[0], original);
	yellow[0].color = "Giallo";
	yellow[1].color = "Giallo";
	blue = find_centers(bocce[1], original);
	blue[0].color = "Blu";
	blue[1].color = "Blu";
	green = find_centers(bocce[2], original);
	green[0].color = "Verde";
	green[1].color = "Verde";
	red = find_centers(bocce[3], original);
	red[0].color = "Rosso";
	red[1].color = "Rosso";

	black = find_centers(bocce[4], original);

	for (int i = 0; i < 2; i++) {
		bocceTot.push_back(yellow[i]);
		bocceTot.push_back(blue[i]);
		bocceTot.push_back(green[i]);
		bocceTot.push_back(red[i]);
	}

	bocceSort = getDistance(bocceTot, black);

	std::cout << "Boccino: " << black[0].x << " " << black[0].y << std::endl;

	for (int i = 0; i < bocceSort.size(); i++) {
		int x = bocceSort[i].x;
		int y = bocceSort[i].y;
		cv::putText(original, std::to_string(i + 1), cv::Point(x, y), CV_FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 2);
	}
	cv::putText(original, ("Vincitore: " + bocceSort[0].color), cv::Point(20, 25), CV_FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 2);
	std::cout << "Il vincitore e': " << bocceSort[0].color << std::endl;
	std::cout << "Distante dal boccino: " << bocceSort[0].d << "px" << std::endl;
	cv::imshow("Test", original);
	cv::waitKey(0);
	cv::destroyAllWindows();
	
	return 0;
}
